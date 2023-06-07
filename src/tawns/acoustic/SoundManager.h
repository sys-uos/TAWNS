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

#ifndef AUDIO_SOUNDMANAGER_H_
#define AUDIO_SOUNDMANAGER_H_

#include <string.h>
#include <omnetpp.h>
#include <string>
#include <filesystem>
#include <queue>
#include "tawns/node/base/ApplicationLayerNodeBase.h"
#include "tawns/node/base/Microphone.h"
#include "tawns/acoustic/Ambient.h"
#include "tawns/acoustic/base/ScaperAudio.h"
#include "inet/common/INETDefs.h"
#include "tawns/acoustic/SoundSource.h"

namespace tawns {

class SoundManager : public inet::cModule
{
    private:
        std::filesystem::path outputDir = "./results/";
        std::filesystem::path confDir = "./results";

        char* simulationTime;
        double speedOfSound;
        int scaperSeed;
        bool scaperAnnotations;
        const char* scaperForegroundDir;
        const char* scaperBackgroundDir;
        double scaperRefDb;
        const char* scaperNormalization;
        bool scaperPreventClipping;

        std::vector<ApplicationLayerNodeBase*> listeners;
        std::vector<SoundSource*> sources;
        Ambient* ambient;

        virtual int dirExists(const char *path);
        virtual std::string exec(const char* cmd);

    protected:
        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }

        virtual std::list<std::filesystem::path>  generateScaperConfigPerNode();
        virtual const char* getGlobalNoiseModel();
        virtual std::vector<double> getGlobalNoiseModelParams();

};

} // tawns namespace




#endif /* AUDIO_SOUNDENVIRONMENT_H_ */
