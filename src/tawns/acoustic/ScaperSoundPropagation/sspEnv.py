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

import os
from os.path import join
from typing import List
import numpy as np
import sox
from sspCore import Ambient, SoundscapeAnnotation, Source, Listener, Microphone
from sspNoise import AcousticNoise, Noise
from sspUtils import Mobility_Event, Sound_Event, Coordinate
from scaper.scaper.scaper import core as scaper


class Sound_Environment:

    def __init__(self, simulationTime : int,
                 foreground_folder : str,
                 background_folder: str,
                 random_state : int,
                 speedOfSound : float,
                 outputDir : str,
                 normalization : scaper.NORMALIZATION) -> None:
        '''
        Generate a 3D spatial sound environment.

        Parameters
        ----------
        simulationTime : int
            The duration of the simulation. Or in other words: the length of the audios generated.
        foreground_folder : str
            Path to the foreground directory.
        background_folder : str
            Path to the background directory
        random_state : int
            Seed of the random number generator.
        speedOfSound : float
            The speed of sound, in meters per second
        outputDir
            The path of the output directory.
        '''
        self.simulationTime = simulationTime
        self.outputDir : str = outputDir
        self.ambient : Ambient = None
        self.sources : List[Source] = []

        # Scaper parameters
        self.foreground_folder = foreground_folder
        self.background_folder = background_folder
        self.random_state = random_state
        self.scaper = scaper.Scaper(duration=self.simulationTime,
                                    fg_path=self.foreground_folder,
                                    bg_path=self.background_folder,
                                    random_state=random_state)
        self.normalization : scaper.NORMALIZATION = normalization

        # sound propagation parameters
        self.speed_of_sound : float = speedOfSound

        # orther params
        self.noise = AcousticNoise.NONE # add noise according to

    def simulate_soundscape(self, listener:Listener,
                            generate_annotations:bool=False,
                            ref_dbRMS:float =-80.0,
                            global_noise: Noise = AcousticNoise.NONE) -> None:
        """
        Generate the spatial soundscapes for one listener. All mobility and sound events are processed sequentially.
        Note that a mobility and sound event cannot occur at the same time.
        Mobility events are provided by a BonnMotion-trace and the sound events are generated in the class Source_Audio.

        Parameters
        ----------
        listener : Listener
            The listener which might be a node or some other entity that is equipped with a microphone.
        generate_annotations : bool
            Configure whether Scaper should generate annotations or not.
        ref_dbRMS : float
            The reference amplitude used in Scaper.
        Returns : None
        -------
        """
        dst_dir = join(self.outputDir, listener.id)
        if not os.path.isdir(dst_dir):
            os.mkdir(dst_dir)
        self.scaper.ref_db = ref_dbRMS

        # log location of bird during the audio_events for extending the annotation files
        audio_event_location : [Coordinate]= []

        for mic in listener.mics:
            for source in self.sources:
                distance_micro2source = self.__distance_device_2_source(listener, mic.getOffset(), source)
                for event in source.event_list:

                    # update the location of the source
                    if isinstance(event, Mobility_Event):
                        source.mobility.updateCurrentLocation(event.time)

                    # the source should generate sounds only when a sound event occurs
                    if isinstance(event, Sound_Event):
                        audio_event_location.append(source.mobility.currentLocation)
                        # at the event time the sound is generated and delayed by the distance between microphone and source
                        event_time_delayed = event.time + distance_micro2source / self.speed_of_sound
                        print(source.audio.audio_label)
                        self.scaper.add_event(label=('const', source.audio.audio_label),
                                              source_file=('choose', [source.audio.audio_path]),
                                              source_time=('const', source.audio.audio_source_time),
                                              event_time=('const', event_time_delayed),  # delay
                                              event_duration=('const', source.audio.audio_duration_seconds),
                                              snr=('const', source.attenuation_model.function(distance_micro2source)),
                                              pitch_shift=None,
                                              time_stretch=None)


            mic.soundscapePath = join(dst_dir, str(mic.id)) + ".wav"
            if generate_annotations:
                jams_path = None
                txt_path = join(dst_dir, str(listener.id) + ".txt")
            else:
                jams_path = None
                txt_path = None

            self.scaper.generate(mic.soundscapePath, jams_path,
                                 allow_repeated_label=True,
                                 allow_repeated_source=True,
                                 reverb=0.0,
                                 disable_sox_warnings=True,
                                 no_audio=False,
                                 txt_path=txt_path,
                                 normalization=self.normalization,
                                 disable_instantiation_warnings=True)
            mic.annotations = SoundscapeAnnotation(simulationTime=self.simulationTime, audio=mic.soundscapePath,
                                                    jams=jams_path, txt=txt_path)
            self.add_location2annotation(mic.annotations, audio_event_location)

            # add noise to the *generated* audios (reduce correlation between audios)
            if global_noise.type == AcousticNoise.NONE:
                pass
            elif global_noise.type == AcousticNoise.AWGN:
                Noise.add_whitenoise(soundscape_path=mic.soundscapePath, volume=float(global_noise.params[0]))
            elif global_noise.type == AcousticNoise.REAL:
                # todo: not implemented yet
                pass

            # reset foreground events in case of multiple microphones
            self.scaper.reset_fg_event_spec()

    def add_location2annotation(self, annotation: SoundscapeAnnotation, audio_event_location: [Coordinate]):
        txt = annotation.txt
        with open(txt, 'r') as fin:
            lines = [line.strip('\n') + "\t(" + str(audio_event_location[i].x) + "," + str(audio_event_location[i].y) +
                     "," + str(audio_event_location[i].z) + ')\n' for i, line in enumerate(fin)]
        with open(txt, 'w') as fout:
            fout.writelines(lines)


    def soundscape_postprocessing(self, listener: Listener) -> None:
        """
        Apply some postprocessing to the generated acoustic data according to the specification for the microphone.

        Parameters
        ----------
        listener : Listener
            The listener which might be a node or some other entity that is equipped with a microphone.

        Returns : None
        -------

        """
        for mic in listener.mics:
            try:
                localprefix, old_audio_, format = mic.soundscapePath.split('.')
            except ValueError:
                raise Exception("Some unexpected error in the soundscape simulation occured")
                exit(-1)
            localprefix = "."
            old_audio = localprefix + old_audio_ +"_." + format
            os.rename(mic.soundscapePath, old_audio)
            tfm = sox.Transformer()
            tfm.set_output_format(file_type=mic.fileType,
                                  rate=mic.samplingFrequency,
                                  bits=mic.sampleSize,
                                  channels=mic.numChannels,
                                  encoding=mic.encoding)
            outputFile = localprefix + old_audio_ + "." + mic.fileType
            tfm.build(old_audio, outputFile)

            os.remove(old_audio)

    def set_ambient(self, new_ambient : Ambient)->None:
        """
        Set the ambient (background) audio.

        Parameters
        ----------
        new_ambient: Ambient
            The ambient sound, or in other word, the background sound.

        -------

        """
        self.ambient = new_ambient
        self.scaper.add_background(label=('const', self.ambient.audio.audio_label),
                                   source_file=('choose', [self.ambient.audio.audio_path]),
                                   source_time=('const', self.ambient.audio.audio_source_time))

    def add_source(self, other_source: Source) -> None:
        """
        Add sound source.

        Parameters
        ----------
        other_source: Source

        -------

        """
        self.sources.append(other_source)

    def reset_scaper(self) -> None:
        """
        Delete the ambient and all sources from the scene.

        Returns: None
        -------

        """
        self.scaper.reset_bg_event_spec()
        self.scaper.reset_fg_event_spec()

    @staticmethod
    def __distance_device_2_source(listener: Listener, microphone_offset: Coordinate, source: Source)->float:
        """
        Calculate the distance between the source and the microphone in meters

        Parameters
        ----------
        listener: Listener
            The listener object, contains the listener coordinates.
        microphone_offset: CoordinateOffset
            The microphones offset in relation to the listener.
        source: Source
            The sound source

        Returns: float
            The distance between the microphone and the source in meters.
        -------

        """
        source_coord = source.mobility.currentLocation
        mic_coord = listener.mobility.currentLocation + microphone_offset
        return np.sqrt((mic_coord.x - source_coord.x) ** 2 + (mic_coord.y - source_coord.y) ** 2 +
                       (mic_coord.z - source_coord.z) ** 2)
