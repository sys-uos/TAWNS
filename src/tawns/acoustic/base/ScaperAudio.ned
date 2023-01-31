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

// Wrappers for the information Scaper requires to generate audio. Note that the label parameter of an audio is always
// the name of the parent directory and automatically extracted.

module ScaperAudio
{
    parameters:
    	string absPath = default("");
    	double sourceTime = default(0.0); 
		@class(ScaperAudio);
}

module ScaperSourceAudio extends ScaperAudio
{
	parameters:
	    // if parameter event_times in SoundSource is set, this parameter is ignored, else  audios are added automatically maxSamples times
	    int maxSamples = default(1);
		@class(ScaperSourceAudio);
}

