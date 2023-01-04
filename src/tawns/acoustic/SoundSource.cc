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

#include "SoundSource.h"
#include "tawns/mobility/single/ABonnMotionMobility.h"
#include "tawns/mobility/static/AStationaryMobility.h"
#include "tawns/common/InitStages.h"
#include "base/AttenuationModel.h"
#include "base/ScaperAudio.h"
#include "tawns/acoustic/Noise.h"

namespace tawns {

Define_Module(SoundSource);

void SoundSource::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        id = par("id");
    }
}

const char* SoundSource::getId()
{
    return id;
}

inet::Coord SoundSource::getLastPosition()
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

const char* SoundSource::getMobilityTypename()
{
    const char* classname = this->getSubmodule("mobility")->getClassName();
    if (strcmp(classname, "tawns::AStationaryMobility") == 0)
    {
            return "A_StationaryMobility";
    }
    else if (strcmp(classname, "tawns::ABonnMotionMobility") == 0)
    {
        return "A_BonnMotionMobility";
    } else {
        throw inet::cRuntimeError("Unknown mobility model in sound source (%s, %s)", getFullName(), id);
    }
}

const char* SoundSource::getMobilityTrace()
{
    if (strcmp(getMobilityTypename(), "A_BonnMotionMobility") == 0)
    {
        return ((ABonnMotionMobility*) this->getSubmodule("mobility"))->getTrace();
    } else {
        throw inet::cRuntimeError("Model has no attribute: trace");
    }
}

const char* SoundSource::getAudioLabel()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getLabel();
}

const char* SoundSource::getAudioAbsPath()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getAbsPath();
}

double SoundSource::getAudioSourceTime()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getSourceTime();
}

int SoundSource::getAudioMaxSamples()
{
    return ((ScaperSourceAudio*) this->getSubmodule("audio"))->getMaxSamples();
}

int SoundSource::getMobilityNodeId()
{
    if (strcmp(getMobilityTypename(), "A_BonnMotionMobility") == 0)
    {
        return ((ABonnMotionMobility*) this->getSubmodule("mobility"))->getNodeId();
    } else {
        throw inet::cRuntimeError("Model has no attribute: trace");
    }
}

const char* SoundSource::getAttenuationModel()
{
    return ((AttenuationModel_Log*) this->getSubmodule("attenuation"))->getModelId();
}

std::vector<double> SoundSource::getAttenuationModel_Params()
{
    return ((AttenuationModel_Log*) this->getSubmodule("attenuation"))->getParams();
}

} // tawns namespace
