//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "tawns/node/base/Microphone.h"

#include "tawns/node/base/ApplicationLayerNodeBase.h"
#include "tawns/node/base/MicrophoneControlMsg_m.h"
#include "tawns/common/InitStages.h"

namespace tawns {

Define_Module(Microphone);

void Microphone::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        id = par("id");
        if (strlen(par("id")) == 0)
        {
            id = this->getFullName();
        }
        filename = par("filename"); // TODO check for validity

        // recording params
        audioFileType = par("audioFileType");
        audioSampleFrequency = par("audioSampleFrequency");
        audioSampleSize = par("audioSampleSize");
        numChannels = par("numChannels");
        audioEncoding = par("audioEncoding");
        audioDuration = par("audioDuration");

        // remember the current audio duration and update continuously
        recStart = 0.0;
        recEnd = audioDuration;

        // init procedure to generate "recordings"
        timeoutMsg = new inet::cMessage("timer");

        scheduleAt(inet::simTime()+audioDuration, timeoutMsg);

        recordingsCtr = 0;
        recordingSignal = registerSignal("recordings");
    }
}

 MicrophoneControlMsg* Microphone::generateMsg()
{
    MicrophoneControlMsg *msg = new MicrophoneControlMsg();

    ApplicationLayerNodeBase *pmod = inet::check_and_cast<ApplicationLayerNodeBase*>(getParentModule());
    std::string aux(pmod->getDirectory());

    aux.append("/");
    aux.append(filename);
    aux.append(".");
    aux.append(audioFileType);
    msg->setPath(aux.c_str());
    msg->setStart(recStart);
    msg->setEnd(recEnd);

    // update ptr of next audio recording
    recStart = recEnd;
    recEnd += audioDuration;

    emit(recordingSignal, recordingsCtr);
    recordingsCtr++;
    return msg;
}

void Microphone::handleMessage(inet::cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        MicrophoneControlMsg *msg = generateMsg();
        send(msg, "dataOut"); // submodule disks receive event that a recording was made (audioDuration passed)
    }
    scheduleAt(inet::simTime()+audioDuration, timeoutMsg);
}

int Microphone::getNumChannels()
{
    return numChannels;
}

const char* Microphone::getAudioFileType()
{
    return audioFileType;
}

double Microphone::getAudioSampleFrequency()
{
    return audioSampleFrequency;
}

const char* Microphone::getMicId()
{
    return id;
}

int Microphone::getAudioSampleSize()
{
    return audioSampleSize;
}

const char* Microphone::getAudioEncoding()
{
    return audioEncoding;
}

double Microphone::getAudioDuration()
{
    return audioDuration;
}

void Microphone::setMicId(const char* newId)
{
    id = newId;
}

} //namespace tawns
