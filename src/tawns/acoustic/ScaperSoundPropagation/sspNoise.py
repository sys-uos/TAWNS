#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
# 

from enum import Enum
import os
from pydub.generators import WhiteNoise
from pydub import AudioSegment

class AcousticNoise(Enum):
    NONE = 0
    AWGN = 1  # additive white gaussian noise
    REAL = 2  # real world noises todo: according to https://medium.com/analytics-vidhya/adding-noise-to-audio-clips-5d8cee24ccb8


class Noise:

    def __init__(self, type: AcousticNoise, params: [] = []):
        """

        Parameters
        ----------
        type
        params if type==None then empty, if type==AWGN then it is only one parameter
        """
        self.type : AcousticNoise = type
        self.params : list = params

    @classmethod
    def add_whitenoise(self, soundscape_path, volume=-80):
        format = soundscape_path.split(".")[-1]
        sound = AudioSegment.from_file(soundscape_path)
        noise = WhiteNoise().to_audio_segment(duration=len(sound), volume=volume)
        combined = sound.overlay(noise)
        if os.path.exists(soundscape_path):
            os.remove(soundscape_path)
        combined.export(soundscape_path, format=format)


class CoherentWindNoise:

    def __init__(self):
        # siehe code: https://github.com/ehabets/Wind-Generator
        # ToDo matlab-code in python übertragen
        pass

