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

import os
import shutil
from dataclasses import dataclass, field
from typing import List
import argparse

from IDef import BG_TMP_DIR, FG_TMP_DIR
from scaper.scaper.scaper import core as scaper
from sspAttenuation import Attenuation_Model_Log, IAttenuation_Model
from sspAudio import Ambient_Audio, Source_Audio
from sspCore import Ambient, Source, Listener, Microphone
from sspEnv import Sound_Environment
from sspMobility import BonnMotionMobility, StationaryMobility
from sspNoise import Noise
from sspNoise import AcousticNoise


@dataclass
class ScaperConfig:
    """
    Wrapper for Scaper's configuration.
    """
    simulationTime : int = 0
    outputDir : str = ""
    speedOfSound : float = 0.0
    seed : int = 0
    annotations : bool = False;
    foregroundDir : str = ""
    tmp_fg_dir : str = ""
    backgroundDir : str = ""
    tmp_bg_dir : str = ""
    refDb: float = 0.0
    normalization : scaper.NORMALIZATION = None
    preventClipping : bool = False
    ambient : Ambient = None
    noise : AcousticNoise = None
    sources : List[Source] = field(default_factory=list)
    listeners : List[Listener] = field(default_factory=list)

def parseScaperConf(confFile):
    global FG_TMP_DIR, BG_TMP_DIR
    conf = ScaperConfig()
    with open(confFile, 'r') as fd:
        lines = fd.readlines()
        i = 0
        while i < len(lines):
            if lines[i][0] == "#":
                i += 1
            flag, val = lines[i].split(':')
            if flag == "simulationTime":
                conf.simulationTime = int(val.strip())
            elif flag == "outputDir":
                conf.outputDir = val.strip()
            elif flag == "soundModel":
                pass # TODO refactor configuration file
            elif flag == "speedOfSound":
                conf.speedOfSound = float(val.strip())
            elif flag == "seed":
                conf.seed = int(val.strip())
            elif flag == "annotations":
                conf.annotations = bool(int(val.strip()))
            elif flag == "foregroundDir":
                conf.foregroundDir= val.strip()
                # create directory for temporary files
                conf.tmp_fg_dir = os.path.join(conf.foregroundDir, "tmp")
                if os.path.isdir(conf.tmp_fg_dir):
                    shutil.rmtree(conf.tmp_fg_dir)
                os.mkdir(conf.tmp_fg_dir)
            elif flag == "backgroundDir":
                conf.backgroundDir = val.strip()
                # create directory for temporary files
                conf.tmp_bg_dir = os.path.join(conf.backgroundDir, "tmp")
                if os.path.isdir(conf.tmp_bg_dir):
                    shutil.rmtree(conf.tmp_bg_dir)
                os.mkdir(conf.tmp_bg_dir)
            elif flag == "refDb":
                conf.refDb = float(val.strip())
            elif flag == "normalization":
                conf.normalization = scaper.NORMALIZATION(val.strip())
            elif flag == "preventClipping":
                conf.preventClipping = bool(int(val.strip()))
            elif flag == "global_noise":
                global_noise = val.strip()
                global_noise_params = lines[i+1].split(':')[1].strip().split(";")
                conf.noise = Noise(AcousticNoise[global_noise], global_noise_params)
            elif flag == "Ambient_Name":
                moduleName = val.strip()
                audioLabel = lines[i+1].split(':')[1].strip()
                absPath = lines[i+2].split(':')[1].strip()
                sourceTime = lines[i+3].split(':')[1].strip()
                i += 3

                # create a scaper ambient audio
                scaperAmbient = Ambient_Audio(audio_label=audioLabel,
                                              audio_path=absPath,
                                              audio_source_time=float(sourceTime))

                # make an ambient object that resembles the undistinct environment noise
                conf.ambient = Ambient(id=moduleName, audio=scaperAmbient, tmpDir=conf.tmp_bg_dir)
            elif flag == "Source_ModuleName":
                moduleName = val.strip();
                id = lines[i+1].split(':')[1].strip()
                event_times = lines[i+2].split(':')[1].strip().split(" ")
                print(lines[i+2])
                print(len(event_times))
                if len(event_times) == 1 and event_times[0] == '':
                    event_times = None
                else:
                    event_times = [float(i) for i in event_times]
                audioLabel = lines[i+3].split(':')[1].strip()
                absPath = lines[i+4].split(':')[1].strip().strip()
                sourceTime = lines[i+5].split(':')[1].strip()
                sound_random_state = int(lines[i+6].split(':')[1].strip())
                maxSamples = int(lines[i+7].split(':')[1].strip())
                # create a scaper source audio
                scaperAudio = Source_Audio(audio_label=id,
                                           audio_path=absPath,
                                           audio_source_time=float(sourceTime),
                                           simulationTime=conf.simulationTime,
                                           event_times=event_times,
                                           random_state=sound_random_state,
                                           max_samples=maxSamples)

                attenuation_model = lines[i+8].split(':')[1].strip()
                if attenuation_model == "AttenuationModel_Log":
                    attenuation_params = lines[i+9].split(':')[1].strip().split(";")[:-1]
                    attenuation_params = [float(x) for x in attenuation_params]
                attenuation = Attenuation_Model_Log(id=attenuation_model, ref_db=conf.refDb, params=attenuation_params)

                mobilityModel = lines[i+10].split(':')[1].strip()
                if mobilityModel == "A_StationaryMobility":
                    coord = lines[i+11].split(':')[1].strip().split(" m")[0]
                    x,y,z = coord.split(", ")
                    x = float(x.replace("(", ""))
                    y = float(y)
                    z = float(z.replace(")", ""))
                    i += 11
                    # create mobility model for source
                    mobilityModel = StationaryMobility(initialX=x,
                                                       initialY=y,
                                                       initialZ=z)
                elif mobilityModel == "A_BonnMotionMobility":
                    trace = lines[i+11].split(':')[1].strip()
                    nodeId = int(lines[i+12].split(':')[1].strip())
                    i += 12
                    # create mobility model for source
                    mobilityModel = BonnMotionMobility(trace, nodeId)
                conf.sources.append(Source(moduleName=moduleName, id=id, mobility=mobilityModel, audio=scaperAudio, attenuation_model=attenuation))
            elif flag == "Listener_Name":
                moduleName = val.strip()
                mobilityModel = lines[i+1].split(':')[1].strip()
                if mobilityModel == "A_StationaryMobility":
                    coord = lines[i+2].split(':')[1].strip().split(" m")[0]
                    x,y,z = coord.split(", ")
                    x = float(x.replace("(", ""))
                    y = float(y)
                    z = float(z.replace(")", ""))
                    mobilityModel = StationaryMobility(initialX=x,
                                                       initialY=y,
                                                       initialZ=z)
                    i += 2
                elif mobilityModel == "A_BonnMotionMobility":
                    trace = lines[i + 2].split(':')[1].strip()
                    nodeId = int(lines[i + 3].split(':')[1].strip())
                    i += 3
                    mobilityModel = BonnMotionMobility(trace, nodeId)
                # --- parse microphone positions --- #
                numberMics = len(lines[i+1].strip().split(":")[1].split(";")) - 1
                mics : [Microphone] = []
                for j in range(0, numberMics):
                    id = lines[i+1].strip().split(":")[1].split(";")[j]
                    offsetX = float(lines[i+2].strip().split(":")[1].split(";")[j])
                    offsetY = float(lines[i+3].strip().split(":")[1].split(";")[j])
                    offsetZ = float(lines[i+4].strip().split(":")[1].split(";")[j])
                    fileType = lines[i+5].strip().split(":")[1].split(";")[j]
                    samplingFrequency = int(lines[i+6].strip().split(":")[1].split(";")[j])
                    sampleSize = int(lines[i+7].strip().split(":")[1].split(";")[j])
                    numChannel = int(lines[i+8].strip().split(":")[1].split(";")[j])
                    encoding = lines[i+9].strip().split(":")[1].split(";")[j]
                    duration = float(lines[i+10].strip().split(":")[1].split(";")[j])
                    mics.append(Microphone(id=id, offsetX=offsetX, offsetY=offsetY, offsetZ=offsetZ,
                                           fileType=fileType, samplingFrequency=samplingFrequency, sampleSize=sampleSize,
                                           numChannels=numChannel, encoding=encoding, duration=duration))
                conf.listeners.append(Listener(id=moduleName, mobility=mobilityModel, mics=mics))
            i += 1

    return conf

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Parse config for Scaper and generate audios.')
    parser.add_argument('-p', "--path", type=str, required=True)
    args = parser.parse_args()
    conf = parseScaperConf(args.path)

    # create a sound environment
    sound_env = Sound_Environment(simulationTime=conf.simulationTime,
                                  foreground_folder=conf.foregroundDir,
                                  background_folder=conf.backgroundDir,
                                  random_state=conf.seed,
                                  speedOfSound=conf.speedOfSound,
                                  outputDir=conf.outputDir,
                                  normalization=conf.normalization)

    # simulate the sound environment and generate audio for the nodes locations
    for listener in conf.listeners:#[0:1]:
        sound_env.set_ambient(conf.ambient)
        for source in conf.sources:
            sound_env.add_source(source)
        sound_env.simulate_soundscape(listener=listener, generate_annotations=conf.annotations, ref_dbRMS=conf.refDb,
                                      global_noise=conf.noise)

        sound_env.soundscape_postprocessing(listener=listener)
        sound_env.reset_scaper()

    if os.path.isdir(BG_TMP_DIR):
        shutil.rmtree(BG_TMP_DIR)
    if os.path.isdir(FG_TMP_DIR):
        shutil.rmtree(FG_TMP_DIR)
