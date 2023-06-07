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

#include "AStationaryMobility.h"
#include "tawns/common/InitStages.h"

namespace tawns {

Define_Module(AStationaryMobility);


void AStationaryMobility::initialize(int stage)
{
    StationaryMobility::initialize(stage);

    if (stage == inet::INITSTAGE_PRE_SOUND_SIMULATION)
    {
        initialX = par("initialX");
        initialY = par("initialY");
        initialZ = par("initialZ");
        relativPosition = par("relativPosition");
        offsetX = par("offsetX");
        offsetY = par("offsetY");
        offsetZ = par("offsetZ");
        initialLatitude = par("initialLatitude");
        initialLongitude = par("initialLongitude");
        initialAltitude = par("initialAltitude");
    }

}

inet::Coord AStationaryMobility::getLastPosition()
{
    return lastPosition;
}

double AStationaryMobility::getOffsetX()
{
    return offsetX;
}

double AStationaryMobility::getOffsetY()
{
    return offsetY;
}

double AStationaryMobility::getOffsetZ()
{
    return offsetZ;
}

double AStationaryMobility::getCoordX()
{
    if (relativPosition)
    {
        return initialX + offsetX;
    } else {
        return initialX;
    }
}

double AStationaryMobility::getCoordY()
{
    if (relativPosition)
    {
        return initialY + offsetY;
    } else {
        return initialY;
    }
}

double AStationaryMobility::getCoordZ()
{
    if (relativPosition)
    {
        return initialZ + offsetZ;
    } else {
        return initialZ;
    }
}


} //tawns namespace


