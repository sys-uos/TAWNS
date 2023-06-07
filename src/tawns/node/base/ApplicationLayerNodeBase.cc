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

#include "ApplicationLayerNodeBase.h"

#include "tawns/mobility/single/ABonnMotionMobility.h"
#include "tawns/mobility/static/AStationaryMobility.h"
#include "tawns/common/InitStages.h"


namespace tawns {

Define_Module(ApplicationLayerNodeBase);

void ApplicationLayerNodeBase::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        numApps = par("numApps");
        numMicrophones = par("numMicrophones");
        directory = "./results";
        //directory.append("/");
        directory.append("/soundscapes/");
        directory.append(this->getFullName());
    }
}

inet::Coord ApplicationLayerNodeBase::getLastPosition()
{
    const char* mobilityModel = this->getSubmodule("mobility")->getClassName();
    if (strcmp(mobilityModel, "tawns::AStationaryMobility") == 0)
    {
        return ((AStationaryMobility*) this->getSubmodule("mobility"))->getLastPosition();
    } else
    {
        throw inet::cRuntimeError("MobilityModel has no attribute lastPosition");
    }
}

const char* ApplicationLayerNodeBase::getDirectory()
{
    return directory.c_str();
}

int ApplicationLayerNodeBase::getNumApps()
{
    return numApps;
}

std::vector<Microphone*> ApplicationLayerNodeBase::getMicrophones()
{
    std::vector<Microphone*> vec;
    for (int i = 0; i < numMicrophones; ++i)
    {
        Microphone* mod = (Microphone*) getSubmodule("microphones", i);
        if (mod != nullptr)
        {
            vec.push_back(mod);
        }
    }
    return vec;
}

const char* ApplicationLayerNodeBase::getMobilityTypename()
{
    const char* classname = this->getSubmodule("mobility")->getClassName();
    if (strcmp(classname, "tawns::AStationaryMobility") == 0)
            return "A_StationaryMobility";
    else if (strcmp(classname, "tawns::ABonnMotionMobility") == 0)
    {
        return "A_BonnMotionMobility";
    } else {
        throw inet::cRuntimeError("Unknown mobility model");
    }
}

const char* ApplicationLayerNodeBase::getMobilityTrace()
{
    if (strcmp(getMobilityTypename(), "A_BonnMotionMobility") == 0)
    {
        return ((ABonnMotionMobility*) this->getSubmodule("mobility"))->getTrace();
    } else {
        throw inet::cRuntimeError("Model has no attribute: trace");
    }
}

int ApplicationLayerNodeBase::getNodeId()
{
    if (strcmp(getMobilityTypename(), "A_BonnMotionMobility") == 0)
    {
        return ((ABonnMotionMobility*) this->getSubmodule("mobility"))->getNodeId();
    } else {
        throw inet::cRuntimeError("Model has no attribute: trace");
    }
}

} // tawns namespace

