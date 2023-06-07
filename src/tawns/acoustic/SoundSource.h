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

#ifndef ACOUSTIC_A_SOUNDSOURCE_H_
#define ACOUSTIC_A_SOUNDSOURCE_H_

#include "inet/common/INETDefs.h"
#include <omnetpp.h>
#include "inet/common/geometry/common/Coord.h"

namespace tawns {

class SoundSource : public inet::cModule
{
private:
    const char* id;
    std::vector<inet::simtime_t> event_times;

protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual std::string exec(const char* cmd);

public:
    virtual const char* getId();

    virtual std::string getEventTimes();

    virtual const char* getAudioLabel();
    virtual const char* getAudioAbsPath();
    virtual double getAudioSourceTime();
    virtual int getAudioMaxSamples();

    virtual inet::Coord getLastPosition();

    virtual const char* getMobilityTypename();
    virtual const char* getMobilityTrace();
    virtual int getMobilityNodeId();

    virtual const char* getAttenuationModel();
    virtual std::vector<double> getAttenuationModel_Params();
};

} //tawns namespace


#endif /* ACOUSTIC_A_SOUNDSOURCE_H_ */
