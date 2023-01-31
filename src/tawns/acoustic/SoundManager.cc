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

#include <omnetpp.h>
#include <regex>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include <thread>
#include <queue>

#include "tawns/acoustic/SoundManager.h"
#include "tawns/acoustic/Noise.h"

#include "tawns/mobility/static/AStationaryMobility.h"
#include "tawns/acoustic/base/ScaperAudio.h"
#include "tawns/common/InitStages.h"

namespace tawns {

Define_Module(SoundManager);

void SoundManager::initialize(int stage)
{
    if (stage == inet::INITSTAGE_SOUND_SIMULATION)
    {
        inet::cEnvir* ev = inet::getEnvir();
        simulationTime = (char*) ev->getConfig()->getConfigValue("sim-time-limit");
        if (simulationTime == nullptr or (not std::regex_match(simulationTime, std::regex("[0-9]+[s]"))))
        {
             throw std::runtime_error("Parameter sim-time-limit undeclared or not in unit seconds");
        } else
        {
            simulationTime[strlen(simulationTime)-1] = '\0';
        }

        // create directory data that hold the generated configuration files and more
        if (!std::filesystem::is_directory("results") || !std::filesystem::exists("results"))
        {
            EV_INFO << "SoundManager created directory results\n";
            std::filesystem::create_directory("results");
        }

        outputDir += (std::filesystem::path) par("soundscapeDir");
        if (!std::filesystem::is_directory(outputDir.c_str()) || !std::filesystem::exists(outputDir.c_str()))
        {
            if (std::filesystem::create_directories(outputDir.c_str()))
            {
                EV_INFO << "SoundManager created directory tree: " << outputDir.c_str() << "\n";
            } else
            {
                EV_INFO << "SoundManager could not created directory tree: " << outputDir.c_str() << "\n";
                throw std::runtime_error("SoundManager could not create soundscape directory!");
            }
        }

        speedOfSound = par("speedOfSound");
        scaperSeed = par("scaperSeed");
        scaperAnnotations = par("scaperAnnotations");
        scaperForegroundDir = par("scaperForegroundDir");
        if (not dirExists(scaperForegroundDir))
        {
            throw inet::cRuntimeError("Directory %s does not exist!", scaperForegroundDir);
        }
        scaperBackgroundDir = par("scaperBackgroundDir");
        if (not dirExists(scaperBackgroundDir))
        {
            throw inet::cRuntimeError("Directory %s does not exist!", scaperBackgroundDir);
        }
        scaperRefDb = par("scaperRefDb");
        scaperNormalization = par("scaperNormalization");
        scaperPreventClipping = par("scaperPreventClipping");

        confDir = confDir / ((std::filesystem::path) par("confDir")) / "";
        if (!std::filesystem::is_directory(confDir.c_str()) || !std::filesystem::exists(confDir.c_str())) {
            if (std::filesystem::create_directory(confDir.c_str()))
            {
                EV_INFO << "SoundManager created config directory: " << confDir.c_str() << "\n";
            } else
            {
                EV_INFO << "SoundManager could not created config directory: " << confDir.c_str() << "\n";
                throw std::runtime_error("SoundManager could not create config directory!");
            }
        }

        cModule* pmod = getParentModule();
        for (cModule::SubmoduleIterator it(pmod); !it.end(); it++)
        {
          cModule *submod = *it;
          if ((strcmp(submod->getClassName(), "tawns::ApplicationLayerNodeBase") == 0) or (strcmp(submod->getClassName(), "tawns::A_AdhocNode") == 0))
          {
              listeners.push_back((ApplicationLayerNodeBase*) submod);
          }
          if (strcmp(submod->getClassName(), "tawns::Ambient")== 0)
          {
              ambient = (Ambient*) submod;
          }
          if (strcmp(submod->getClassName(), "tawns::SoundSource") == 0)
          {
              sources.push_back((SoundSource*) submod);
          }
        }
        // same distribution of sound events is assured by scaper extension
        std::list<std::filesystem::path> list = generateScaperConfigPerNode();
        for (auto const& confPath : list)
        {
            if (std::filesystem::exists(confPath))
            {
                std::string cmd = "python3 ../../src/tawns/acoustic/ScaperSoundPropagation/sspMain.py --path";
                cmd  = cmd + " ";
                cmd = cmd + confPath.string();
                exec(cmd.c_str());
            } else {
                throw inet::cRuntimeError("Could not find configuration file for Scaper: %s", confPath.string().c_str());
            }

            // copy config file into the directory of the node, so that all ground-truth information are in one place
            std::filesystem::path dst;
            dst += "./results/soundscapes/";
            dst+= confPath.filename();
            dst+= "/";
            dst+= confPath.filename();
            dst+= "_config.txt";
            try
            {
                std::filesystem::copy_file(confPath, dst, std::filesystem::copy_options::overwrite_existing);
            }
            catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.
            {
                throw inet::cRuntimeError("Could not find copy configuration file to results/soundscapes (node: %s)", confPath.filename().string().c_str());
            }


        }
    }
}


int SoundManager::dirExists(const char *path)
{
    struct stat info;

    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

std::string SoundManager::exec(const char* cmd)
{
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

std::list<std::filesystem::path>  SoundManager::generateScaperConfigPerNode()
{
    std::list<std::filesystem::path> ret = {};

    for (ApplicationLayerNodeBase* listener: listeners)
    {
        std::filesystem::path confPath;
        confPath += confDir;
        confPath+= listener->getFullName();
        std::ofstream scaperConfFile(confPath);
        ret.push_back(confPath);

        if(scaperConfFile.is_open())
        {
            scaperConfFile << "# --- Automatically generated file to start scaper --- #" << std::endl;
            scaperConfFile << "simulationTime:" << simulationTime << std::endl;
            scaperConfFile << "outputDir:" << outputDir.c_str() << std::endl;
            scaperConfFile << "speedOfSound:" << speedOfSound << std::endl;
            scaperConfFile << "seed:" << scaperSeed << std::endl;
            scaperConfFile << "annotations:" << scaperAnnotations << std::endl;
            scaperConfFile << "foregroundDir:" << scaperForegroundDir << std::endl;
            scaperConfFile << "backgroundDir:" << scaperBackgroundDir << std::endl;
            scaperConfFile << "refDb:" << scaperRefDb << std::endl;
            scaperConfFile << "normalization:" << scaperNormalization << std::endl;
            scaperConfFile << "preventClipping:" << scaperPreventClipping << std::endl;
            scaperConfFile << "global_noise:" << getGlobalNoiseModel() << std::endl;
            scaperConfFile << "global_noise_params:";
            std::vector<double> noise_params = getGlobalNoiseModelParams();
            for(double val: noise_params)
            {
                scaperConfFile << val << ";";
            }
            scaperConfFile << std::endl;

            scaperConfFile << "# --- Data about ambient sound --- #" << std::endl;
            scaperConfFile << "Ambient_Name:" << ambient->getFullName() << std::endl;
            scaperConfFile << "Ambient_AudioLabel:" << ambient->getAudioLabel() << std::endl;
            scaperConfFile << "Ambient_AudioAbsPath:" << ambient->getAudioAbsPath() << std::endl;
            scaperConfFile << "Ambient_AudioSourceTimeLabel:" << ambient->getAudioSourceTime() << std::endl;
            scaperConfFile << "# --- Data about sound sources --- #" << std::endl;
            for(SoundSource* source: sources)
            {
                scaperConfFile << "Source_ModuleName:" << source->getFullName() << std::endl;
                scaperConfFile << "Source_Id:" << source->getId() << std::endl;
                scaperConfFile << "Source_EventTimes:" << source->getEventTimes().c_str() << std::endl;
                scaperConfFile << "Source_AudioLabel:" << source->getAudioLabel() << std::endl;
                scaperConfFile << "Source_AudioAbsPath:" << source->getAudioAbsPath() << std::endl;
                scaperConfFile << "Source_AudioSourceTimeLabel:" << source->getAudioSourceTime() << std::endl;
                scaperConfFile << "Source_RandomState:" << scaperSeed << std::endl;
                scaperConfFile << "Source_MaxSamples:" << source->getAudioMaxSamples() << std::endl;
                scaperConfFile << "Source_AttenuationModel:" << source->getAttenuationModel() << std::endl;
                std::vector<double> params = source->getAttenuationModel_Params();
                scaperConfFile << "Source_AttenuationModel_Params:";
                for(double val: params)
                {
                    scaperConfFile << val << ";";
                }
                scaperConfFile << std::endl;
                const char * mobility = source->getMobilityTypename();
                scaperConfFile << "Source_MobilityModel:" << mobility << std::endl;
                if (strcmp(mobility, "A_StationaryMobility") == 0)
                {
                    scaperConfFile << "Source_Coord:" << source->getLastPosition() << std::endl;
                } else if (strcmp(mobility, "A_BonnMotionMobility") == 0)
                {
                    scaperConfFile << "Source_MobilityTrace:" << source->getMobilityTrace() << std::endl;
                    scaperConfFile << "Source_NodeId:" << source->getMobilityNodeId() << std::endl;
                }
            }
            scaperConfFile << "# --- Data about listener --- #" << std::endl;
            scaperConfFile << "Listener_Name:" << listener->getFullName() << std::endl;
            const char * mobility = listener->getMobilityTypename();
            scaperConfFile << "Listener_MobilityModel:" << mobility << std::endl;
            if (strcmp(mobility, "A_StationaryMobility") == 0)
            {
                scaperConfFile << "Listener_Coord:" << listener->getLastPosition() << std::endl;
            } else if (strcmp(mobility, "A_BonnMotionMobility") == 0)
            {
                scaperConfFile << "Listener_MobilityTrace:" << listener->getMobilityTrace() << std::endl;
                scaperConfFile << "Listener_NodeId:" << listener->getNodeId() << std::endl;
            }
            std::vector<Microphone*> mics = listener->getMicrophones();
            std::stringstream ids, offsetsX, offsetsY, offsetsZ, audioFileTypes, audioSampleFrequencies, audioSampleSizes, numChannels, audioEncodings, audioDurations;
            for (Microphone* mic: mics)
            {
                AStationaryMobility* micMob = (AStationaryMobility*) mic->getSubmodule("mobility");
                ids << mic->getMicId() << ";";
                offsetsX << micMob->getOffsetX() << ";";
                offsetsY << micMob->getOffsetY() << ";";
                offsetsZ << micMob->getOffsetZ() << ";";
                audioFileTypes << mic->getAudioFileType() << ";";
                audioSampleFrequencies << mic->getAudioSampleFrequency() << ";";
                audioSampleSizes << mic->getAudioSampleSize() << ";";
                numChannels << mic->getNumChannels() << ";";
                audioEncodings << mic->getAudioEncoding() << ";";
                audioDurations << mic->getAudioDuration() << ";";

            }
            scaperConfFile << "Listener_Ids:" << ids.str() << std::endl;
            scaperConfFile << "Listener_MicsOffsetsX:" << offsetsX.str() << std::endl;
            scaperConfFile << "Listener_MicsOffsetsY:" << offsetsY.str() << std::endl;
            scaperConfFile << "Listener_MicsOffsetsZ:" << offsetsZ.str() << std::endl;
            scaperConfFile << "Listener_MicsFileTypes:" << audioFileTypes.str() << std::endl;
            scaperConfFile << "Listener_MicsSampleFrequencies:" << audioSampleFrequencies.str() << std::endl;
            scaperConfFile << "Listener_MicsSampleSizes:" << audioSampleSizes.str() << std::endl;
            scaperConfFile << "Listener_NumChannels:" << numChannels.str() << std::endl;
            scaperConfFile << "Listener_MicsEncoding:" << audioEncodings.str() << std::endl;
            scaperConfFile << "Listener_MicsDurations:" << audioDurations.str() << std::endl;

            scaperConfFile.close();
        } else {
            std::stringstream ss;
            ss << "Cannot write data into " << confPath;
            throw inet::cRuntimeError("%s", ss.str().c_str());
        }
    }
    return ret;
}

const char* SoundManager::getGlobalNoiseModel()
{
    inet::cModule* submod = this->getSubmodule("globalNoise");
    if (strcmp(submod->getClassName(), "tawns::AWGNNoiseAndInterferenceModel")== 0)
    {
        return "AWGN";
    } else if (strcmp(submod->getClassName(), "tawns::NoneNoiseAndInterferenceModel")== 0)
    {
        return "NONE";
    } else {
        return "UnknownNoiseModel";
    }
}

std::vector<double> SoundManager::getGlobalNoiseModelParams()
{
    inet::cModule* submod = this->getSubmodule("globalNoise");
    std::vector<double> params;
    if (strcmp(submod->getClassName(), "tawns::AWGNNoiseAndInterferenceModel")== 0)
    {
        params = {((AWGNNoiseAndInterferenceModel*) submod)->getVolume()};
    } else if (strcmp(submod->getClassName(), "tawns::NoneNoiseAndInterferenceModel")== 0)
    {
        ;// do nothing
    }
    return params;
}

} //tawns namespace
