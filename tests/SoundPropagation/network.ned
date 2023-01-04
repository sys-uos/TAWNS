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

package tawns.tests.SoundPropagation;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

import tawns.node.inet.ManetRouter;
import tawns.acoustic.Ambient;
import tawns.acoustic.SoundSource;
import tawns.acoustic.SoundManager;

network Network
{
    parameters:
        int numHosts; 
        int numSources;

        @display("bgb=1500,800");
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

        soundManager: SoundManager {
            @display("p=306,15;is=s");
        }


}
