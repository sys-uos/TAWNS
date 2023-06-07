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

#include "Ambient.h"

#include "base/ScaperAudio.h"
#include "tawns/acoustic/Noise.h"
#include "tawns/common/InitStages.h"
#include <sstream>

namespace tawns {

Define_Module(Ambient);

void Ambient::initialize(int stage)
{

}

const char* Ambient::getAudioLabel()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getLabel();
}

const char* Ambient::getAudioAbsPath()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getAbsPath();
}

double Ambient::getAudioSourceTime()
{
    return ((ScaperAudio*) this->getSubmodule("audio"))->getSourceTime();
}

} // tawns namespace
