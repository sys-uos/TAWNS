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


package tawns.examples.RealAudioTransfer;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

import tawns.node.inet.ManetRouter;
import tawns.acoustic.Ambient;
import tawns.acoustic.SoundSource;
import tawns.acoustic.SoundManager;
import tawns.acoustic.INoiseAndInterference;


// Steps to generate acoustic data (no transmission!). All configuration of the network is done in the files network.ned and the omnetpp.ini.
// 1. Build your network with their infrastructure
// 1.2. Decide on the node module, e.g. ManetRouter.
// 1.3. Modules Ipv4NetworkConfigurator and Ieee80211ScalarRadioMedium are mandatory and must be configured.
// 1.4. If using Omnetpp with GUI, then the modul IntegratedVisualizer or similar must be used
// 2. Place your acoustic modules 
// 2.1. Module Ambient provides background acoustic e.g. some wind noises.
// 2.2. Module Soundsource refers to an entitiy such as a bird and its foreground audio.
// 2.3. Module SoundManager handles the audio generation (it basically wraps up all previous information and starts the audio generation). it MUST be the last submodule! 
//... check omnetpp.ini configuration

network Network
{
    parameters:
        int numHosts;
        int numSources;

        @display("bgb=1000,1000");
    submodules:
        node[numHosts]: ManetRouter {
            parameters:
                @display("r=,,#707070;p=150,200");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=212,15;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            //radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            parameters:
                @display("p=122,15;is=s");
        }

// ------------ Audio Modules ----------------------------------------- //

        ambient: Ambient {
            @display("p=386,15;is=s");
        }

        source[numSources]: SoundSource {
            //ToDo: set icon
            @display("p=56,102");
        }


		// --- IMPORTANT --- //
		// --- must be placed at the end of the submodule, so other entities such as SoundSources and Sensors are already initialized --- //
        soundManager: SoundManager {
            @display("p=306,15;is=s");
        }


}
