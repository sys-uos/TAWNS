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

#include "SimpleEnergyConsumer.h"

#include "inet/common/ModuleAccess.h"
#include "tawns/node/base/Microphone.h"

namespace tawns {

namespace physicallayer {

Define_Module(A_SimpleEnergyConsumer);

A_SimpleEnergyConsumer::~A_SimpleEnergyConsumer()
{
    cancelAndDelete(timer);
}

void A_SimpleEnergyConsumer::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        offPowerConsumption = inet::units::values::W (par("offPowerConsumption"));
        onPowerConsumption = inet::units::values::W (par("onPowerConsumption"));
        powerConsumption = inet::units::values::W (0);
        timer = new inet::cMessage("timer");
        updatePowerConsumption();
        scheduleIntervalTimer();
        energySource = inet::getModuleFromPar<inet::power::IEpEnergySource>(par("energySourceModule"), this);
    }
    else if (stage == inet::INITSTAGE_POWER)
        energySource->addEnergyConsumer(this);
}

void A_SimpleEnergyConsumer::handleMessage(inet::cMessage *message)
{
    if (message == timer) {
        updatePowerConsumption();
        scheduleIntervalTimer();
    }
    else
        throw inet::cRuntimeError("Unknown message");
}

// TODO: to be honest, this is probably not the correct signal to use and a custom signal should be implemented
void A_SimpleEnergyConsumer::updatePowerConsumption()
{
    powerConsumption = computePowerConsumption();
    emit(powerConsumptionChangedSignal, powerConsumption.get());
}

//TODO: das geht zwar, aber wie stelle ich permanenten energieverbrauch da? gibt es da andere optionen?
// oder ich lese es einfach --> nach x sekunden verbrauchen wir y energie für task z
void A_SimpleEnergyConsumer::scheduleIntervalTimer()
{
    scheduleAt(inet::simTime() + par("timerInterval"), timer);
}

inet::units::values::W A_SimpleEnergyConsumer::computePowerConsumption() const
{
    //TODO: Überprüfen, ob Mikrofon an oder aus ist und setze verbrauch, sobald mikro das unterstüzt
    return onPowerConsumption;
}

} // namespace physicallayer

} // namespace inet
