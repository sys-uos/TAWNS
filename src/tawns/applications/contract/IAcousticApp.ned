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

package tawns.applications.contract;

import inet.applications.contract.IApp;


// Interface class that defines the interaction with the Disc in ApplicationLayerNodeBase
// Idea: Disc-module sends message to all apps, apps handle message accordingly. Access Disc-module by using this interface class.

moduleinterface IAcousticApp extends IApp
{
    parameters:
        @display("i=block/app");
        @class(IAcousticApp);
    gates:
        input dataIn;
        output dataOut;
    
}
