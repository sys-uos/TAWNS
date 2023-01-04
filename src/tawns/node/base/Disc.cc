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

#include "tawns/node/base/Disc.h"
#include "tawns/node/base/MicrophoneControlMsg_m.h"
#include "tawns/node/base/DiscInitMsg_m.h"
#include "tawns/common/InitStages.h"

namespace tawns {

Define_Module(Disc);
//TODO: misses destructor?

void Disc::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        initMsg = new inet::cMessage("init");
        scheduleAt(inet::simTime(),initMsg);
    }
}

void Disc::handleMessage(inet::cMessage *msg)
{
    if (msg->isSelfMessage())
    {
//        int gates = this->gateSize("dataOut");
//
//        for(int i = 0; i < gates; i++)
//        {
//
//            auto cgate = this->gate("dataOut",i);
//            auto ngate = cgate->getNextGate();
//            std::string owner = ngate->getOwner()->getName();
//
//            if(owner.compare("app") == 0)
//            {
//                DiscInitMsg* mmsg = new DiscInitMsg();
//                send(mmsg, "dataOut", i);
//            }
//        }
    }
    else
    {
        MicrophoneControlMsg *mcmsg = inet::check_and_cast<MicrophoneControlMsg *>(msg);
        Recording rec(mcmsg->getPath(), mcmsg->getStart(), mcmsg->getEnd());
        recordings.push_back(rec);
    }
}

std::pair<Recording,bool> Disc::getNextRecoding()
{
    if(recordings.empty())
    {
        return std::pair<Recording,bool>(Recording(NULL,0,0),false);
    }

    auto ret = std::pair<Recording,bool>(recordings.front(),true);
    recordings.erase(recordings.begin());
    return ret;
}

} //namespace tawns
