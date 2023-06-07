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

package tawns.node.inet;

import tawns.node.inet.ManetRouter;


import inet.applications.contract.IApp;
import inet.routing.contract.IManetRouting;


module ManetRouter extends AdhocHost
{
    submodules:
        routingApp: <default("")> like IApp if typename != "" {      // Aodv or other manet routing over UDP/TCP protocol
            @display("p=913,75");
        }
        routing: <default("")> like IManetRouting  if typename != "" {
            @display("p=913,225");
        }
    connections:
        routing.ipOut --> tn.in++ if exists(routing);
        routing.ipIn <-- tn.out++ if exists(routing);
        routingApp.socketOut --> at.in++ if exists(routingApp);
        routingApp.socketIn <-- at.out++ if exists(routingApp);
}

