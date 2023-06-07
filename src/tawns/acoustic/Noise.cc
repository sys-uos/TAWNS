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

#include "tawns/acoustic/Noise.h"
#include "tawns/common/InitStages.h"
#include <omnetpp.h>

namespace tawns {

Define_Module(NoneNoiseAndInterferenceModel);
Define_Module(AWGNNoiseAndInterferenceModel);

void NoneNoiseAndInterferenceModel::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        type = par("type");
    }
}

const char* NoneNoiseAndInterferenceModel::getTypename()
{
    return type;
}

void AWGNNoiseAndInterferenceModel::initialize(int stage)
{
    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        type = par("type");
        volume = par("volume");

    }
}

const char* AWGNNoiseAndInterferenceModel::getTypename()
{
    return type;
}

double AWGNNoiseAndInterferenceModel::getVolume()
{
    return volume;
}

} // tawns namespace
