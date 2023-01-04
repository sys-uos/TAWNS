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

package tawns.node.base;

import tawns.power.contract.ISimpleEnergyConsumer;
import tawns.physicallayer.energyconsumer.SimpleEnergyConsumer;
import tawns.mobility.static.AStationaryMobility;

// Simple module for handling microphone logic

module Microphone
{
    parameters:
        @display("i=misc/microphone1");
       	string id = default(fullName()); // identifier of the microphone
        string filename = default(fullName()); // name of the audio generated
        
        string energySourceModule = default("");
        *.energySourceModule = default(absPath(this.energySourceModule)); //FIXME: does this actually do what i think it does

		// recording params
        // TODO: same types as pysox, refer to documentation of pysox https://pysox.readthedocs.io/en/latest/api.html#sox.transform.Transformer.build
        string audioFileType = default("flac");
        double audioSampleFrequency @unit(Hz) = default(44100Hz);
        int audioSampleSize @enum(8,16,32,64) @unit(b) = default(16b);
        int numChannels = default(1); // more than one channel currently not supported, use multiple microphones to simulate >1 channels
        string audioEncoding @enum("signed-integer", "unsigned-integer", "floating-point", "a-law", "u-law", "oki-adpcm", "ima-adpcm", "ms-adpcm", "gsm-full-rate") = default("signed-integer");  // todo other audio encodings
        double audioDuration @unit(s) = default(30.0s);
        @class(Microphone);
        @signal[recordings](type="long");
        @statistic[numberRecordings](title="Number of Recordings made"; source="recordings"; record=max);
    gates:
        input dataIn;
        output dataOut;
        
    submodules:    
        mobility: AStationaryMobility {
            parameters:
                @display("p=125,160;is=s");
                // initialX,Y,Z values are set to avoid error in StationaryMobilityModel
                // they're ignored, microphonePosition= nodePosition+relativePosition
                initialX = 0.0m;
                initialY = 0.0m;
                initialZ = 0.0m;
                
                relativPosition = true;  // display string is ignored for microphones
                offsetX = default(0m);
                offsetY = default(0m);
                offsetZ = default(0m);
        }
        
        energyConsumer: <default("")> like ISimpleEnergyConsumer if typename != "" {
            parameters:
                @display("p=100,400");
        }
        

}