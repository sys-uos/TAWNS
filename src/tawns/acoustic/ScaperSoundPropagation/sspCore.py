#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU  General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU  General Public License for more details.
# 
# You should have received a copy of the GNU  General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.
# 

import shutil
from dataclasses import dataclass
from os.path import join
from typing import List, Dict, Callable

import numpy as np
from sortedcontainers import SortedList

from IDef import BG_TMP_DIR, FG_TMP_DIR
from sspAttenuation import IAttenuation_Model
from sspAudio import Source_Audio, Ambient_Audio
from sspNoise import Noise, AcousticNoise
from sspUtils import Coordinate


@dataclass
class SoundscapeAnnotation:
        simulationTime : int
        audio : str
        jams : str = None
        txt : str = None

#----------------------------- Microphone -----------------------------------------#

class Microphone:

    def __init__(self, id : str, offsetX : float, offsetY : float, offsetZ : float, fileType:str, samplingFrequency : int,
                 sampleSize : str, numChannels : int, encoding: str, duration:float) -> None:
        """

        Parameters
        ----------
        id: str

        offsetX: float
        offsetY: float
        offsetZ: float
        fileType: str
        samplingFrequency
        sampleSize
        numChannels
        encoding
        duration
        """
        self.id = id
        self.offsetX : float = offsetX
        self.offsetY : float = offsetY
        self.offsetZ : float = offsetZ
        self.fileType : str = fileType
        self.samplingFrequency : int = samplingFrequency
        self.sampleSize : int = sampleSize
        self.numChannels : int = numChannels
        self.encoding : str = encoding
        self.duration : float = duration
        self.soundscapePath : os.path = "" # used in postprocessing, automatically filled when soundscape is generated
        self.annotations : SoundscapeAnnotation = None

    def getOffset(self) -> Coordinate:
        return Coordinate(self.offsetX, self.offsetY, self.offsetZ)


#----------------------------- Source -----------------------------------------#

class Source:

    def __init__(self, moduleName:str, id: str, mobility, audio: Source_Audio, attenuation_model : IAttenuation_Model, noise : Noise = Noise(AcousticNoise.NONE )):
        """
        Refers to a sound source

        Parameters
        ----------
        id: str
            identifier of the sound source
        mobility: I_Mobility
            the mobility model
        audio: Source_Audio
            The source audio
        attenuation_model: IAttenuation_Model
            The attenuation model of the audio.
        """

        self.id = id
        self.audio : Source_Audio = audio
        self.mobility = mobility
        self.event_list : SortedList = SortedList(self.mobility.mobilityEvents) # init with start coordinates
        self.attenuation_model : IAttenuation_Model = attenuation_model
        self.noise : Noise = noise

        #self.audio = self.__add_noise_and_interference() # todo: implemented in the future to reduce correlation

        # prevent overlap of sound_events and mobility_events, meaning when source emits sound, it does not move and vice versa
        # no sound event is added when mobility event occurs
        # todo: theoretically, the bird moves some times to an other location, which takes some time

        for sound_event in self.audio.audioEvents:
            for mobility_event in self.mobility.mobilityEvents:
                if sound_event.time + self.audio.audio_duration_seconds >= mobility_event.time:
                    self.event_list.add(sound_event)

    def __add_noise_and_interference(self)->Source_Audio:
        if self.noise.type == AcousticNoise.NONE: # audio is not modified
            return self.audio
        elif self.noise.type == AcousticNoise.AWGN: # white gaussian noise is added to audio
            dst_tmp = join(FG_TMP_DIR, self.id + "_" + self.audio.audio_path.split('/')[-1])
            shutil.copyfile(self.audio.audio_path, dst_tmp)
            Noise.add_whitenoise(dst_tmp, volume=self.noise.params[0])
            deprecatedAudio = self.audio
            a = Source_Audio(audio_label=self.audio.audio_label,
                             audio_path=dst_tmp,
                             audio_source_time=float(self.audio.audio_source_time),
                             simulationTime=self.audio.simulationTime,
                             random_state=self.audio.random_state,
                             max_samples=self.audio.max_samples)
            del deprecatedAudio
            return a
        else:
            raise Exception("Unknown noise-and-interference model!")

#----------------------------- Listener ---------------------------------------#

class Listener:

    def __init__(self, id:str, mobility, mics: List[Microphone]):
        """


        Parameters
        ----------
        id: str
            The name of the node which is a unique identifier.
        mobility: I_Mobility
            Must be a mobility model that is derived from I_Mobility.
        mics: [Microphone]
            List of microphones
        """
        self.id = id
        self.mobility = mobility
        self.mics : [Microphone] = mics


#-------------------------------- Ambient -------------------------------------#

class Ambient:

    def __init__(self, id: str, audio: Ambient_Audio, noise : Noise = Noise(AcousticNoise.NONE , []), tmpDir:str=""):
        global FG_TMP_DIR, BG_TMP_DIR
        """
        Refers to the ambient (background) noise.

        Parameters
        ----------
        audio: Ambient_Audio
            The ambient (background) audio.
        """
        self.noise = noise

        if self.noise.type == AcousticNoise.NONE: # audio is not modified
            self.audio: Ambient_Audio = audio
        elif self.noise.type == AcousticNoise.AWGN: # white gaussian noise is added to audio
            dst_tmp = join(tmpDir, audio.audio_path.split('/')[-1])
            shutil.copyfile(audio.audio_path, dst_tmp)
            Noise.add_whitenoise(dst_tmp, volume=self.noise.params[0])
            self.audio = Ambient_Audio(audio_label="tmp",
                                          audio_path=dst_tmp,
                                          audio_source_time=float(audio.audio_source_time))
            del audio
        else:
            raise Exception("Unknown noise-and-interference model!")

