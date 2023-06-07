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

import tawns.acoustic.base.ScaperAudio;
import tawns.acoustic.SoundSource;
import tawns.acoustic.Ambient;

// SoundManager is instantiated after the other audio modules and handles the simulation of the acoustic environment.
// The acoustic simulation takes place *before* the wireless simulation takes place. 
// The simulated data is saved into the directory results.

module SoundManager
{
    parameters:
        @display("i=misc/music_note");

        string soundscapeDir = default("soundscapes"); // by default this directory is generated in the folder results
        string confDir = "configs"; // by default this directory is generated in the folder results
        double speedOfSound @unit(mps) = 343.0mps;  
        
        // Scaper parameters
        int scaperSeed = default(intuniform(0, 16777215)); // seed in interval [0, 2^24-1]
        bool scaperAnnotations = default(true); // annotation file contain ground truth, format STIME ETIME COORD_STRING SPECIES
        string scaperForegroundDir = default("../../audioLibrary/foreground/");
        string scaperBackgroundDir = default("../../audioLibrary/background");
        double scaperRefDb = default(-85.0); // must be same unit as the applied normalization which is RMS or LUFS
        string scaperNormalization @enum("LUFS", "RMS") = default("RMS");
        bool scaperPreventClipping = default(false);
        @class(SoundManager);

    submodules:
		globalNoise: <default("NoneNoiseAndInterferenceModel")> like INoiseAndInterference {
            @display("p=506.016,14.056;is=s");
        }
}
