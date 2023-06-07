//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU  General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU  General Public License for more details.
//
// You should have received a copy of the GNU  General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include <filesystem>

#include "FtpTcpSimple.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/networklayer/common/L3AddressResolver.h"

namespace tawns {

Define_Module(FtpTcpSimple);

#define MSGKIND_CONNECT    1
#define MSGKIND_SEND       2
#define MSGKIND_CLOSE      3

Recording FtpTcpSimple::popRecording(Disc *disc)
{
    std::pair<Recording,bool> rec = disc->getNextRecoding();
    if(rec.second)
    {
        return rec.first;
    }
    return Recording("none",0,0);
}

FtpTcpSimple::~FtpTcpSimple()
{
    cancelAndDelete(ftpTimeout);
}

void FtpTcpSimple::initialize(int stage)
{
    TcpAppBase::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        activeOpen = par("active");
        tOpen = par("tOpen");
        tSend = par("tSend");
        tClose = par("tClose");
        ftpPacketSize = par("ftpPacketSize");
        generalTimeOut = par("generalTimeOut");

        const char *discModulePath = par("discModulePath");
        disc = (Disc*) inet::cModule::getModuleByPath(discModulePath); // if module is not found, exception is thrown

        //TODO: replace commands
        //skips empty command --> crashes, if there is no Command in commands
        commandIndex = 1;
        commands.push_back(Command());
        ftpTimeout = new inet::cMessage("ftptimer");
    }
}

void FtpTcpSimple::handleMessageWhenUp(inet::cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleTimer(msg);
    }
    else
    {
        socket.processMessage(msg);
    }
}

void FtpTcpSimple::handleStartOperation(inet::LifecycleOperation *operation)
{
    if (inet::simTime() <= tOpen) {
        ftpTimeout->setKind(MSGKIND_CONNECT);
        scheduleAt(tOpen, ftpTimeout);
    }
}

void FtpTcpSimple::handleStopOperation(inet::LifecycleOperation *operation)
{
    cancelEvent(ftpTimeout);
    if (socket.isOpen())
        close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void FtpTcpSimple::handleCrashOperation(inet::LifecycleOperation *operation)
{
    cancelEvent(ftpTimeout);
    if (operation->getRootModule() != getContainingNode(this))
        socket.destroy();
}

void FtpTcpSimple::handleTimer(inet::cMessage *msg)
{
    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            if (activeOpen)
                connect(); // sending will be scheduled from socketEstablished()
            else
                throw inet::cRuntimeError("TODO");
            break;

        case MSGKIND_SEND:
            sendData();
            break;

        case MSGKIND_CLOSE:
            close();
            break;

        default:
            throw inet::cRuntimeError("Invalid timer msg: kind=%d", msg->getKind());
    }
}

void FtpTcpSimple::sendData()
{
    // used for naming audio files, mainly debugging
    auto *pmod = getParentModule();
    auto nodename = pmod->getFullName();
    EV_WARN << "PACKET SIZE SENDER: " << ftpPacketSize << std::endl;
    if(disc != nullptr)
    {

        // get new recordings from disc
        Recording rec = popRecording(disc);
        while(rec.valid())
        {
            std::pair<float,float> times = rec.getRecTimes();
            std::string path = rec.getPath();
            std::string old_fn = path.substr(path.find_last_of("/") + 1);
            std::string name = inet::simTime().str()+"_"+ std::to_string(counter++) +"_" + nodename +"_"+old_fn;

            auto starttime = times.first;
            auto duration = times.second - times.first;
            // temp files will be deleted after they've been received by a sink
            auto copypath = "./temp/";
            std::filesystem::create_directories(copypath);
            std::string new_path = copypath + name;

            //TODO: find solution that does not involve std::system()
            std::string exec = "ffmpeg  -y -i " + path + " -ss "+ std::to_string(starttime) +
                    " -t " + std::to_string(duration) + " " + copypath + name;
            EV_WARN << "ffmpeg: " << exec << std::endl;
            const char * c = exec.c_str();

            std::system(c);

            commands.push_back(Command(tSend,name,new_path,times.first,times.second));

            rec = popRecording(disc);
        }
    }

    // if there are commands available, start working on them
    if(commandIndex < (int)commands.size())
    {
        auto announce = announceDataPacket();

        createDataPacket(announce);

        if (++commandIndex < (int)commands.size()) {;
            scheduleAt(std::max(tSend, inet::simTime()), ftpTimeout);
            return;
        }
    }
    // checks every few second, depending on generalTimeOut, whether a new track is available
    scheduleAt(std::max(tSend, inet::simTime() + generalTimeOut), ftpTimeout);
}

std::vector<uint8_t> FtpTcpSimple::announceDataPacket()
{
    auto currentComm = commands[commandIndex];
    std::string name = currentComm.name;

    std::string path = currentComm.path;

    std::ifstream instream(path, std::ifstream::in | std::ifstream::binary);

    std::vector<uint8_t> message;

    // set code according to protocol
    uint8_t code = 111;
    message.push_back(code);

    uint8_t fnlen = name.length();
    message.push_back(fnlen);

    // get and set file size
    instream.seekg(0,instream.end);
    uint32_t fsize = instream.tellg();

    instream.seekg(0,instream.beg);
    uint8_t s[4];
    memcpy(s,&fsize,sizeof(fsize));
    for(uint8_t i : s)
    {
        message.push_back(i);
    }

    // insert file name
    std::vector<uint8_t> fn(name.begin(), name.end());
    message.insert(message.end(), fn.begin(), fn.end());

    return message;
}

void FtpTcpSimple::createDataPacket(std::vector<uint8_t> announce)
{
    auto currentComm = commands[commandIndex];
    auto path = currentComm.path;

    inet::Ptr<inet::Chunk> payload;
    std::ifstream instream(path, std::ios::in | std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    std::vector<uint8_t> message = announce;

    for(auto const& v: data) {

        if(message.size() == ftpPacketSize)
        {
            inet::Packet *packet = new inet::Packet("new-Packet");
            const auto& bytesChunk = inet::makeShared<inet::BytesChunk>();
            bytesChunk->setBytes(message);
            payload = bytesChunk;
            payload->addTag<inet::CreationTimeTag>()->setCreationTime(inet::simTime());
            packet->insertAtBack(payload);
            sendPacket(packet);

            message = std::vector<uint8_t>();
        }

        message.push_back(v);
    }

    if((message.size() > 0) && (message.size() < ftpPacketSize))
    {
        // send anyways but pad with zeros

        for(int i = message.size(); i < ftpPacketSize; i++)
            message.push_back(0);
        inet::Packet *packet = new inet::Packet("new-Packet");
        const auto& bytesChunk = inet::makeShared<inet::BytesChunk>();
        bytesChunk->setBytes(message);
        payload = bytesChunk;
        payload->addTag<inet::CreationTimeTag>()->setCreationTime(inet::simTime());
        packet->insertAtBack(payload);
        sendPacket(packet);

    }
}

void FtpTcpSimple::socketEstablished(inet::TcpSocket *socket)
{
    TcpAppBase::socketEstablished(socket);

    //ASSERT(commandIndex == 0);
    ftpTimeout->setKind(MSGKIND_SEND);
    inet::simtime_t tSend = commands[commandIndex].tSend;
    scheduleAt(std::max(tSend, inet::simTime()), ftpTimeout);
}

void FtpTcpSimple::socketDataArrived(inet::TcpSocket *socket, inet::Packet *msg, bool urgent)
{
    TcpAppBase::socketDataArrived(socket, msg, urgent);
}

void FtpTcpSimple::socketClosed(inet::TcpSocket *socket)
{
    TcpAppBase::socketClosed(socket);
    cancelEvent(ftpTimeout);
    if (operationalState == State::STOPPING_OPERATION && !this->socket.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void FtpTcpSimple::socketFailure(inet::TcpSocket *socket, int code)
{
    TcpAppBase::socketFailure(socket, code);
    cancelEvent(ftpTimeout);
}

void FtpTcpSimple::finish()
{
    EV << getFullPath() << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";
    recordScalar("bytesRcvd", bytesRcvd);
    recordScalar("bytesSent", bytesSent);
    // delete all temp files
    if(access("./temp/", F_OK) != -1)
    {
        for (auto& path: std::filesystem::directory_iterator("./temp/")) {
                std::filesystem::remove_all(path);
            }
        std::filesystem::remove("./temp/");
    }

}

void FtpTcpSimple::refreshDisplay() const
{
    TcpAppBase::refreshDisplay();

    std::ostringstream os;
    os << inet::TcpSocket::stateName(socket.getState()) << "\nsent: " << bytesSent << " bytes\nrcvd: " << bytesRcvd << " bytes";
    getDisplayString().setTagArg("t", 0, os.str().c_str());
}

} // namespace inet

