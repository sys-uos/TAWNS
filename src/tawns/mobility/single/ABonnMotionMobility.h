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

#ifndef MOBILITY_SCAPERBONNMOTIONMOBILITY_H_
#define MOBILITY_SCAPERBONNMOTIONMOBILITY_H_

#include "inet/mobility/single/BonnMotionMobility.h"


namespace tawns {

/**
 * Access to coordinates is required when using in soundscape generation.
 */

class ABonnMotionMobility : public inet::BonnMotionMobility
{
private:
    const char * trace;
    int nodeId;

public:
    virtual void initialize(int stage) override;
    virtual const char* getTrace();
    virtual int getNodeId();


};

} //tawns namespace




#endif /* MOBILITY_SCAPERBONNMOTIONMOBILITY_H_ */
