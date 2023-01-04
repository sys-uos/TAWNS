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

#ifndef TANS_NODE_BASE_AApplicationLayerNodeBase_H_
#define TANS_NODE_BASE_AApplicationLayerNodeBase_H_

#include "inet/common/INETDefs.h"
#include "Microphone.h"
#include "inet/common/geometry/common/Coord.h"


namespace tawns {

class ApplicationLayerNodeBase : public inet::cSimpleModule
{
    protected:
        int numMicrophones;
        int numApps;
        std::string directory;

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }


    public:
        virtual std::vector<Microphone*> getMicrophones();

        virtual int getNumApps();
        virtual const char* getDirectory();

        virtual const char* getMobilityTypename();
        virtual inet::Coord getLastPosition();
        virtual const char* getMobilityTrace();
        virtual int getNodeId();
};

} //tawns namespace



#endif /* TANS_NODE_BASE_A_ApplicationLayerNodeBase_H_ */
