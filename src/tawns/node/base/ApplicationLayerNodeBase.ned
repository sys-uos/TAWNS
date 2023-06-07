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

package tawns.node.base;

import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;
import inet.node.base.TransportLayerNodeBase;

import tawns.power.contract.ISimpleEnergyConsumer;
import tawns.physicallayer.energyconsumer.SimpleEnergyConsumer;
import tawns.applications.contract.IAcousticApp;
import tawns.node.base.Disc;

//
// This module extends NodeBase, a class which contains the most basic infrastructure for network nodes that is
// not strictly communication protocol related, by basic acoustic hardware and functions.
//
// See derived modules
//
module ApplicationLayerNodeBase extends TransportLayerNodeBase
{
    parameters:
        @display("i=node/acoustic_node_1m_1");
        
        int numApps = default(0);
        int numMicrophones = default(1);
        mobility.typename = default("AStationaryMobility");
        @networkNode();
        @class(ApplicationLayerNodeBase);
        @figure[applicationLayer](type=rectangle; pos=250,6; size=1000,130; lineColor=#808080; cornerRadius=5; fillColor=#ffff00; fillOpacity=0.1);
        @figure[applicationLayer.title](type=text; pos=1245,11; anchor=ne; text="application layer");

    submodules:
        app[numApps]: <> like IAcousticApp {
            parameters:
                @display("p=375,76,row,150");
        }
        at: MessageDispatcher {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }
        
        microphones[numMicrophones]: Microphone {
            @display("p=1100.9249,75.6");
        }

        disc: Disc {
            @display("p=811.125,78.75");
        }
        
        energyConsumer: <default("")> like ISimpleEnergyConsumer if typename != "" {
            parameters:
                @display("p=100,400");
        }

    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].socketOut --> at.in++;
            app[i].socketIn <-- at.out++;
        }
        for i=0..numMicrophones-1 {
            microphones[i].dataOut --> disc.dataIn++;
            microphones[i].dataIn <-- disc.dataOut++;
        }
        for i=0..numApps-1 {
            app[i].dataOut --> disc.dataIn++;
            app[i].dataIn <-- disc.dataOut++;
        }
        
        at.out++ --> udp.appIn if hasUdp;
        at.in++ <-- udp.appOut if hasUdp;

        at.out++ --> tcp.appIn if hasTcp;
        at.in++ <-- tcp.appOut if hasTcp;

        at.out++ --> sctp.appIn if hasSctp;
        at.in++ <-- sctp.appOut if hasSctp;

        at.out++ --> tn.in++;
        at.in++ <-- tn.out++;
}

