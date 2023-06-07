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

package tawns.applications.ftpsimpleapp;

import tawns.applications.contract.IAcousticApp;

//
// Accepts any number of incoming TCP connections, and discards whatever
// arrives on them. Compatible with both ~Ipv4 and ~Ipv6.
//
module FtpTcpSink like IAcousticApp, FtpTcp
{
    parameters:
        string localAddress = default(""); // may be left empty ("")
        int localPort = default(1000); // port number to listen on
        string serverThreadModuleType; // module type of "thread" module to launch on incoming connections
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
   		int ftpPacketSize = default(536);
        serverThreadModuleType = default("tawns.applications.ftpsimpleapp.FtpTcpSinkThread");
        @class(FtpTcpSink);
        @display("i=block/sink");
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none; checkSignals=false);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none; checkSignals=false);
		
		gates:
            input socketIn @labels(TcpCommand/up);
            output socketOut @labels(TcpCommand/down);
            
            input dataIn;
        	output dataOut;
    
}

simple FtpTcpSinkThread
{
    parameters:
        @display("i=block/sink");
        @signal[packetReceived](type=inet::Packet);
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none);
}

