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

package tawns.examples.Showcase;

import inet.common.geometry.common.OsgGeographicCoordinateSystem;
import inet.visualizer.common.IntegratedVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.internetcloud.InternetCloud;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth10M;

import tawns.node.inet.ManetRouter;
import tawns.acoustic.Ambient;
import tawns.acoustic.SoundSource;
import tawns.acoustic.SoundManager;
import tawns.acoustic.INoiseAndInterference;

network Showcase_2D
{
    parameters:
        int numDevs;
        int numBirds0;
        int numBirds1;

    submodules:
        node[numDevs]: ManetRouter {
            parameters:
                @display("r=,,#707070;p=150,200");
        }
        clusterhead: ManetRouter {
            parameters:
                @display("r=,,#707070;p=150,200");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=212,15;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=122,15;is=s");
        }
        internetCloud: InternetCloud {
            @display("p=650,425;is=vl");
        }
        backend: StandardHost {
            parameters:
                @display("p=850,425");
        }
        // ------------ Audio Modules ----------------------------------------- //
        ambient: Ambient {
            @display("p=386,15;is=s");
        }
        bird0[numBirds0]: SoundSource {
            @display("i=soundsource/bird1");
        }
        bird1[numBirds1]: SoundSource {
            @display("i=soundsource/bird2");
        }
        soundManager: SoundManager {
            @display("p=306,15;is=s");
        }
    connections:
        clusterhead.pppg++ <--> Eth10M <--> internetCloud.pppg++;
        internetCloud.pppg++ <--> Eth10M <--> backend.pppg++;
}

network Showcase_3D
{
    parameters:
        int numDevs;
        int numBirds0;
        int numBirds1;
        int numFrogs;
    submodules:
        // ------------ 3D Mapping ----------------------------------------- //
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=100,50");
        }
        coordinateSystem: OsgGeographicCoordinateSystem {
            parameters:
                @display("p=100,150");
        }
		// ------------ Wireless Network Modules ----------------------------------------- //
        node[numDevs]: ManetRouter {
            parameters:
                @display("r=,,#707070;p=150,200");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=212,15;is=s");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=122,15;is=s");
        }
		// ------------ Audio Modules ----------------------------------------- //
        ambient: Ambient {
            @display("p=386,15;is=s");
        }
        birds0[numBirds0]: SoundSource {
            @display("p=56,102");
        }
        birds1[numBirds1]: SoundSource {
            @display("p=56,102");
        }
        frogs[numFrogs]: SoundSource {
            @display("p=56,102");
        }
        soundManager: SoundManager {
            @display("p=306,15;is=s");
        }
}

