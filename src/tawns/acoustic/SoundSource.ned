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

package tawns.acoustic;

import inet.mobility.contract.IMobility;
import inet.node.contract.INetworkNode;

import tawns.mobility.contract.IAcousticMobility;
import tawns.acoustic.base.ScaperSourceAudio;
import tawns.acoustic.base.IAttenuationModel;
import tawns.acoustic.base.AttenuationModel_Log;
import tawns.acoustic.INoiseAndInterference;

// Simple Node for a SoundSource. Wrapper for a sound entity. Movement is visualized in Omnet, timestamps of emitted signals are calculated in SSP.

// It is possible to set the event_times (the times a sound is emitted) by using the parameter event_times, but note that they will later be delayed and attenuated.
// If this parameter stays empty, the song occurences are calculated automatically. The parameter audio->maxSamples will limit the number of emitted sounds.

module SoundSource
{
    parameters:
        @display("i=soundsource/animal1");
        string id = default("");  // used in the ground-truth file in results/soundscapes/
        string event_times = default(""); // e.g. ["3s 7s"], unit in seconds and space as delimeter
        mobility.subjectModule = default("^");
        @class(SoundSource);
        @networkNode();  // technically no network node, but this annotation allows the 3D-visualization
        
    submodules:
        mobility: <default("AStationaryMobility")> like IMobility {
            parameters:
        }
        audio: ScaperSourceAudio {
            @display("p=100,244");
        }
        attenuation: <default("AttenuationModel_Log")> like IAttenuationModel {
            parameters:
                @display("p=100,320;is=s");
        }
}
