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

#ifndef TAWNS_PHYSICALLAYER_ENERGYCONSUMER_SimpleEnergyConsumer_H_
#define TAWNS_PHYSICALLAYER_ENERGYCONSUMER_SimpleEnergyConsumer_H_

#include "../../power/contract/ISimpleEnergyConsumer.h"
#include "inet/power/contract/IEpEnergySource.h"

namespace tawns{

namespace physicallayer {

class INET_API A_SimpleEnergyConsumer : public inet::cSimpleModule, public power::ISimpleEnergyConsumer, public inet::cListener
{
protected:
    // parameters
    inet::units::values::W offPowerConsumption = inet::units::values::W (NaN);
    inet::units::values::W onPowerConsumption = inet::units::values::W (NaN);
    inet::cMessage *timer = nullptr;

    // environment
    inet::power::IEpEnergySource *energySource = nullptr;

    // state
    inet::units::values::W  powerConsumption = inet::units::values::W (NaN);

protected:
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual inet::units::values::W  computePowerConsumption() const;
    virtual void handleMessage(inet::cMessage *message) override;

    virtual void updatePowerConsumption();
    virtual void scheduleIntervalTimer();

public:
    virtual ~A_SimpleEnergyConsumer();

    virtual inet::power::IEnergySource *getEnergySource() const override { return energySource; }
    virtual inet::units::values::W  getPowerConsumption() const override { return powerConsumption; }

};

} // namespace physicallayer

} // namespace tawns
#endif // ifndef TAWNS_PHYSICALLAYER_ENERGYCONSUMER_SimpleEnergyConsumer_H_

