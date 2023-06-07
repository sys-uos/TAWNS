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

#include <iostream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <cmath>

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

        std::vector<std::string> timesStr = inet::cStringTokenizer(par("event_times")).asVector();
        for (auto i : timesStr) {
            inet::simtime_t t = inet::simtime_t::parse(i.c_str());
            event_times.push_back(t);
        }

        // check whether the event-times have enough distance to each other
        std::stringstream cmd;
        cmd << "soxi -D " << getAudioAbsPath();
        double audio_duration = std::stod(exec(cmd.str().c_str()));
        for (auto i : event_times)
        {
            for (auto j : event_times)
            {
                if (i != j && (std::abs(i.dbl()-j.dbl()) < audio_duration))
                {
                    throw inet::cRuntimeError("Invalid event_times (minimum distance must be %fseconds)", audio_duration);
                }
            }
        }
    }
}

std::string SoundSource::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
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

std::string SoundSource::getEventTimes()
{
    std::stringstream ss;
    for (int j=0; j<event_times.size(); ++j) {
        ss << event_times[j] << " ";
    }
    return ss.str();
}

} // tawns namespace
