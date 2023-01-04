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

#include "tawns/common/InitStages.h"

#include <algorithm>

namespace inet {

Define_InitStage(PRE_SOUND_SIMULATION);
Define_InitStage_Dependency(PRE_SOUND_SIMULATION, LAST);


Define_InitStage(SOUND_SIMULATION);
Define_InitStage_Dependency(SOUND_SIMULATION, PRE_SOUND_SIMULATION);

} // namespace inet

