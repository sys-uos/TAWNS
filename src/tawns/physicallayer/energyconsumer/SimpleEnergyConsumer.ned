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

package tawns.physicallayer.energyconsumer;

import tawns.power.contract.ISimpleEnergyConsumer;

simple SimpleEnergyConsumer like ISimpleEnergyConsumer
{
    parameters:
        string energySourceModule;
        double offPowerConsumption @unit(W) = default(0mW);
        double onPowerConsumption  @unit(W) = default(1mW);
        double timerInterval @unit(s) = default(0.001s)
        
        @display("i=block/cogwheel");
        @class(physicallayer::A_SimpleEnergyConsumer);
        
        @signal[powerConsumptionChanged];
        
        @statistic[powerConsumptionChanged](title="Power Consumption Simple";
        source=powerConsumptionChanged;
        record=count,vector;
        interpolationmode=sample-hold);
        @statistic[powerConsumptionChanged](title="Power Consumption Simple2";
        source=powerConsumptionChanged;
        record=histogram);
        
}