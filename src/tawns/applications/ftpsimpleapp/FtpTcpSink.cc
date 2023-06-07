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

#include "FtpTcpSink.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/Message.h"
#include "inet/common/socket/SocketTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/stlutils.h"

#include <chrono>
#include <ctime>
#include <filesystem>

namespace tawns {

Define_Module(FtpTcpSink);
Define_Module(FtpTcpSinkThread);

//FIXME: this has no use in this app, still has to be included for it to become an app
Recording FtpTcpSink::popRecording(Disc *disc)
{
    return Recording("",0,0);
}

FtpTcpSink::FtpTcpSink()
{
}

FtpTcpSink::~FtpTcpSink()
{
}

void FtpTcpSink::initialize(int stage)
{
    TcpServerHostApp::initialize(stage);

    if (stage == inet::INITSTAGE_LOCAL) {
        std::filesystem::create_directories("./testserver/");
        bytesRcvd = 0;
        ftpPacketSize = par("ftpPacketSize");
        WATCH(bytesRcvd);
    }
}

void FtpTcpSink::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

    char buf[160];
    sprintf(buf, "threads: %d\nrcvd: %ld bytes", socketMap.size(), bytesRcvd);
    getDisplayString().setTagArg("t", 0, buf);
}

void FtpTcpSink::finish()
{
    TcpServerHostApp::finish();

    recordScalar("bytesRcvd", bytesRcvd);
}

void FtpTcpSink::handleMessageWhenUp(inet::cMessage *msg)
{
    auto owner = msg->getSenderModule();
    std::string owner_name = owner->getName();
    if (msg->isSelfMessage()) {
        inet::TcpServerThreadBase *thread = (inet::TcpServerThreadBase *)msg->getContextPointer();
        if (!inet::contains(threadSet, thread))
            throw inet::cRuntimeError("Invalid thread pointer in the timer (msg->contextPointer is invalid)");
        thread->timerExpired(msg);
    }
    else if (owner_name.compare("disc") == 0)
    {
        delete msg;
    }
    else {
        inet::TcpSocket *socket = inet::check_and_cast_nullable<inet::TcpSocket *>(socketMap.findSocketFor(msg));
        if (socket)
            socket->processMessage(msg);
        else if (serverSocket.belongsToSocket(msg))
            serverSocket.processMessage(msg);
        else {
//            throw cRuntimeError("Unknown incoming message: '%s'", msg->getName());
            EV_ERROR << "message " << msg->getFullName() << "(" << msg->getClassName() << ") arrived for unknown socket \n";
            delete msg;
        }
    }
}

void FtpTcpSinkThread::initialize(int stage)
{
    inet::TcpServerThreadBase::initialize(stage);

    if (stage == inet::INITSTAGE_LOCAL) {
        bytesRcvd = 0;
        WATCH(bytesRcvd);
    }
}

void FtpTcpSinkThread::established()
{
    bytesRcvd = 0;
}

void FtpTcpSinkThread::dataArrived(inet::Packet *pk, bool urgent)
{
    //TODO: for --> lese bis buffer voll --> interpretier ganzes paket --> lese weiter oder warte
    auto *pmod = getParentModule()->getParentModule();
    auto nodename = pmod->getFullName();

    long packetLength = pk->getByteLength();
    bytesRcvd += packetLength;
    sinkAppModule->bytesRcvd += packetLength;
    emit(inet::packetReceivedSignal, pk);
    const auto& bytesChunk = pk->peekDataAsBytes();
    std::vector<uint8_t> data = bytesChunk->getBytes();
    for(const auto &d : data)
    {

        buffer.push_back(d);
        if(buffer.size() == sinkAppModule->ftpPacketSize) //TODO: parameter, minimum size is header size is 100
        {

            // 1: expect announcement of data packet
            if(!receiving)
            {
                uint8_t code = buffer[0];
                if (111 == code)
                {
                    uint8_t fnsize = buffer[1];

                    uint8_t s[4];
                    uint32_t s32;
                    for(int i = 0; i < 4; i++)
                    {
                        s[i] = buffer[2+i];
                    }
                    memcpy(&s32,s,sizeof(s32));
                    size = s32;
                    std::vector<uint8_t> bytefn(buffer.begin()+ 6, buffer.begin() + 6 + fnsize);
                    filename = std::string(bytefn.begin(),bytefn.end());

                    auto time = std::chrono::system_clock::now();
                    std::time_t ttime = std::chrono::system_clock::to_time_t(time);
                    sink_path = "./testserver/" + std::string(std::ctime(&ttime)) + "_"
                            + nodename + "_" + filename;

                    receiving = true;
                    buffer = std::vector<uint8_t>(buffer.begin() + 6 + fnsize, buffer.end());
                }
            }

            // 2: read bytes until file is complete
            if(receiving)
            {

                std::ofstream outstream(sink_path, std::ios::app);
                if(outstream)
                {

                    for(const auto &e : buffer)
                    {

                        if(size == 0)
                        {
                            buffer.clear();
                            break;
                        }
                        else
                        {
                            outstream << e;
                            size = size-1;
                        }
                    }
                }

                // 3: skip empty bytes and go back to 1
                if(size == 0)
                {
                    std::filesystem::remove("./temp/" + filename);
                    receiving = false;
                }
            }
            buffer.clear();
        }
    }
    delete pk;
}

void FtpTcpSinkThread::refreshDisplay() const
{
    std::ostringstream os;
    os << (sock ? inet::TcpSocket::stateName(sock->getState()) : "NULL_SOCKET") << "\nrcvd: " << bytesRcvd << " bytes";
    getDisplayString().setTagArg("t", 0, os.str().c_str());
}

} // namespace inet

