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

#ifndef MOBILITY_SCAPERSTATIONARYMOBILITY_H_
#define MOBILITY_SCAPERSTATIONARYMOBILITY_H_

#include "inet/mobility/static/StationaryMobility.h"


namespace tawns {

/**
 * Access to coordinates is required when using in soundscape generation.
 */

class AStationaryMobility : public inet::StationaryMobility
{
private:
    double initialLatitude;
    double initialLongitude;
    double initialAltitude;
    double initialX;
    double initialY;
    double initialZ;
    bool relativPosition;
    double offsetX;
    double offsetY;
    double offsetZ;

protected:
    virtual void initialize(int stage) override;

public:
    virtual inet::Coord getLastPosition();

    virtual double getOffsetX();
    virtual double getOffsetY();
    virtual double getOffsetZ();
    virtual double getCoordX();
    virtual double getCoordY();
    virtual double getCoordZ();
};

} //tawns namespace



#endif /* MOBILITY_SCAPERSTATIONARYMOBILITY_H_ */
