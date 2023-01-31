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
from typing import List

from pydub import AudioSegment

from sspNoise import Noise
from sspUtils import Sound_Event, random_Sequence_With_Range_And_Minimum_Distance_Between_Samples


class I_Audio:

    def __init__(self, audio_path:str):
        """
        Load some often used audio related information once for easier access.

        Parameters
        ----------
        audio_path: str
            Path to the audio
        """
        self.audio_path = audio_path
        assert audio_path is not None

        # do not load file in ram to save memory
        pyas = AudioSegment.from_file(audio_path)
        self.audio_duration_seconds = pyas.duration_seconds
        self.audio_rmsRAW = pyas.rms
        self.audio_rmsFS = pyas.dBFS
        self.channels = pyas.channels
        self.sample_width = pyas.sample_width
        self.frame_rate = pyas.frame_rate
        self.frame_width = pyas.frame_width
        self.max_dBFS = pyas.max_dBFS
        self.max_possible_amplitude = pyas.max_possible_amplitude

    def __repr__(self):
        return f"Audio_Path: {self.audio_path}"

class Ambient_Audio(I_Audio):

    def __init__(self, audio_label: str,
                 audio_path:str,
                 audio_source_time:float):
        """
        Wrapper for audio-related information which is required by Scaper.

        Parameters
        ----------
        audio_label: str
            name of the audio
        audio_path: str
            path to the audio
        audio_source_time: float
            time within the audio at which the audio is used by Scaper
        """
        super().__init__(audio_path)
        self.audio_label = audio_label
        self.audio_path = audio_path
        self.audio_source_time = audio_source_time

        assert self.audio_label is not None
        assert self.audio_path is not None
        assert self.audio_source_time is not None
        assert self.audio_duration_seconds is not None


    def __repr__(self) -> str:
        return f"audio label: {self.audio_label} | audio_path: {self.audio_path} | source_time: {self.audio_source_time}"


class Source_Audio(Ambient_Audio):

    def __init__(self,
                 audio_label,
                 audio_path,
                 audio_source_time,
                 simulationTime,
                 event_times = None,
                 max_samples=None,
                 random_state=None) -> None:
        """
        Wrapper for audio configuration of a sound source.

        Parameters
        ----------
        audio_label: str
            name of the audio
        audio_path: str
            path to the audio
        audio_source_time
            The time the simulated sound event starts
        simulationTime: float
            the time in which audio events might occur
        max_samples: iunt
            maximal number of audio events (= simulation time at which sound start to occur, default: None
        random_state: int
            the seed of the random number generator
        """

        super().__init__(audio_label, audio_path, audio_source_time)
        self.simulationTime = simulationTime
        self.max_samples = max_samples
        self.random_state = random_state
        if event_times:
            self.audioEvents : List[Sound_Event] = [Sound_Event(etime) for etime in event_times]
        else:
            self.audioEvents : List[Sound_Event] = self.__generateAudioEvents(simulationTime,
                                                                              random_state=random_state,
                                                                              max_samples=max_samples)

    def __generateAudioEvents(self, simulationTime:float, random_state:int=None, max_samples:int=None)->List[Sound_Event]:
        """
        Generate the sound events in a "quasi" uniform distribution.

        Parameters
        ----------
        simulationTime: float
            The duration in which the sound events should occur. Note that the last sound event will be
            at simulationTime - Audio duration - 4s, to guarantee the spatial sound generation using Scaper.
        random_state: int
            The seed for the random number generator.
        max_samples: int
            The maximal number of sound events generated. Might be less than specified which depends on the simulation
            time, and the length of the related audio.

        Returns: List[Sound_Event]
            List with the sound events
        -------

        """
        if max_samples is None:
            max_samples = int(simulationTime / self.audio_duration_seconds)

        events = random_Sequence_With_Range_And_Minimum_Distance_Between_Samples(
            max=simulationTime - self.audio_duration_seconds - 4, max_samples=max_samples,
            mingap=self.audio_duration_seconds, random_state=random_state)
        soundEvents = [Sound_Event(etime) for etime in events]
        return soundEvents



