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

package tawns.acoustic.base;

moduleinterface IAttenuationModel
{
    parameters:
        @display("bgb=,448");
}

module AttenuationModel_Log like IAttenuationModel
{
    // Model that fits the formula f(x) = a * log_10(max(1, b+x)) + c, x equals the distance in meters
    // used parameter set according to publications "TAWNS - A Terrestrial Acoustic and Wireless Network Simulation Framework"
    parameters:
    	double a = default(-10.8);
    	double b = default(2.3);
    	double c = default(-23.5);
    	@class(AttenuationModel_Log);
}
