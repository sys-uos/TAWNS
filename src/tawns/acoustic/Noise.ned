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

package tawns.acoustic;

moduleinterface INoiseAndInterference
{
    parameters:
    	string type;
    	
}

// Wrapper when no noise should be added to the audios
module NoneNoiseAndInterferenceModel like INoiseAndInterference
{
    parameters:
        string type = "NONE";
        @class(NoneNoiseAndInterferenceModel);	        
}

// Addative White Gaussian Noise (AWGN)
module AWGNNoiseAndInterferenceModel like INoiseAndInterference
{
    parameters:
        string type = "AWGN";
        double volume = default(-80); // Volume in DB relative to maximum possible amplitude	
        @class(AWGNNoiseAndInterferenceModel);	        
}