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

#include <filesystem>

#include "tawns/acoustic/base/ScaperAudio.h"
#include "tawns/common/InitStages.h"

namespace tawns {

Define_Module(ScaperAudio);
Define_Module(ScaperSourceAudio);

void ScaperAudio::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOAD_AUDIOS)
    {
        absPath = par("absPath");
        std::filesystem::path aux(absPath);
        label = aux.parent_path().parent_path().filename().string();
        if (not std::filesystem::exists(absPath))
        {
            throw inet::cRuntimeError("File %s does not exist!", absPath);
        }
        sourceTime = par("sourceTime");
    }
}

const char* ScaperAudio::getLabel ()
{
    return label.c_str();
}

const char* ScaperAudio::getAbsPath ()
{
    return absPath;
}

double ScaperAudio::getSourceTime ()
{
    return sourceTime;
}

void ScaperSourceAudio::initialize(int stage)
{
    if (stage == inet::INITSTAGE_LOAD_AUDIOS)
    {
        ScaperAudio::initialize(stage);
        maxSamples = par("maxSamples");
    }
}

int ScaperSourceAudio::getMaxSamples ()
{
    return maxSamples;
}

} //tawns namespace
