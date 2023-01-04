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

#ifndef DEVICES_BASE_MICROPHONE_H_
#define DEVICES_BASE_MICROPHONE_H_

#include "inet/common/INETDefs.h"
#include "tawns/applications/contract/Recording.h"


namespace tawns {


class Disc : public inet::cSimpleModule
{
    protected:
        typedef std::vector<Recording> RecordingVector;
        RecordingVector recordings;
        inet::cMessage *initMsg = nullptr;

    protected:
        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
        virtual void handleMessage(inet::cMessage *msg) override;

        friend class IAcousticApp;

    public:
        virtual std::pair<Recording,bool> getNextRecoding();
};

} //namespace tawns



#endif /* DEVICES_BASE_MICROPHONE_H_ */
