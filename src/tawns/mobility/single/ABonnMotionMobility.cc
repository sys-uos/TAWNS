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

#include "ABonnMotionMobility.h"

namespace tawns {

Define_Module(ABonnMotionMobility);

void ABonnMotionMobility::initialize(int stage)
{
    BonnMotionMobility::initialize(stage);
    trace = par("traceFile");
    nodeId = par("nodeId");
}

const char * ABonnMotionMobility::getTrace()
{
    return trace;
}

int ABonnMotionMobility::getNodeId()
{
    return nodeId;
}

} // tawns namespace



