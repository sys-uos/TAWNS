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



#ifndef __INET_INITEXTENDEDSTAGES_H
#define __INET_INITEXTENDEDSTAGES_H

#include "inet/common/InitStageRegistry.h"
#include "inet/common/InitStages.h"

// Extension of the INET' stages
// Staging process of INET is used here, so we use the namespace inet although we are in the tawns project

namespace inet {

extern INET_API InitStage INITSTAGE_LOAD_AUDIOS;

extern INET_API InitStage INITSTAGE_PRE_SOUND_SIMULATION;

extern INET_API InitStage INITSTAGE_SOUND_SIMULATION;


} //namespae inet

#endif

