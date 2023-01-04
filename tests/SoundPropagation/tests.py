import numpy as np
import argparse
from pydub import AudioSegment, silence
import matplotlib.pyplot as plt
import numpy.fft as fft
import soundfile as sf                                                      
import array

def tdoa(sig, refsig, interp=1, fs=1, phat=True):
    """
    This implementation is based on the implementation of pyroomacoustic.
    This function computes the time difference of arrival (TDOA) of the signal at the two microphones. 
    We recover the time delay tau using the Generalized Cross Correlation - Phase Transform (GCC-PHAT) method. The reference is 
    Knapp, C., & Carter, G. C. (1976). The generalized correlation method for estimation of time delay.
    Parameters
    ----------
    sig : nd-array
        The signal of the reference microphone
    refsig : nd-array
        The signal of the second microphone
    interp : int, optional (default 1)
        The interpolation value for the cross-correlation, it can
        improve the time resolution (and hence DOA resolution)
    fs : int, optional (default 44100 Hz)
        The sampling frequency of the input signal
    Return
    ------
    tau : float
        the delay between the two microphones (in seconds)
    cc : list
        the cross correlation coefficients
    """

    # zero padded length for the FFT
    n = sig.shape[0] + refsig.shape[0] - 1
    if n % 2 != 0:
        n += 1

    # Generalized Cross Correlation Phase Transform
    sig1_fft = fft.rfft(np.array(sig, dtype=np.float32), n=n)
    refsig_fft = fft.rfft(np.array(refsig, dtype=np.float32), n=n)

    if phat:
        sig1_fft /= np.abs(sig1_fft)
        refsig_fft /= np.abs(refsig_fft)

    cc = fft.irfft(sig1_fft * np.conj(refsig_fft), n=interp * n)

    # maximum possible delay given distance between microphones
    t_max = n // 2 + 1

    # reorder the cross-correlation coefficients
    cc = np.concatenate((cc[-t_max:], cc[:t_max]))
    
    # pick max cross correlation index as delay
    tau = np.argmax(np.abs(cc))
    tau -= t_max  # because zero time is at the center of the array

    return tau / (fs * interp), cc

def attenuation_model_default_params(distance: float):
    a = -10.8
    b = 2.3
    c = -23.5
    if distance >=1 and (b + distance) >=1:
        ret = a * np.log(b + distance) + c
    else:
        ret =  a * np.log(1) + c
    return ret
    
    
if __name__ == "__main__":
    # Assuming following configuration in omnetpp.ini:
    # Setup: source<-->node[0]<-->node[1]<-->...<-->node[40] (numHosts = 40, numSources = 1)
    # distance between entities: 5m
    # speed of sound: 343m/s
    # for details check configuration files
  
    # generate the audio path
    paths = []
    for i in np.arange(0,40,1):
        paths.append("results/soundscapes/node[" + str(i) + "]/microphones[0].flac")
    speedOfSound=343.0 # default value
    delta = 5 # meters, distance between two adjacent entities (nodes and sources), see config file
    
    print("Test1: Time delay between microphones of one node using GCC-PHAT")
    a1 = AudioSegment.from_file("results/soundscapes/node[0]/microphones[0].flac", "flac")
    a2 = AudioSegment.from_file("results/soundscapes/node[0]/microphones[1].flac", "flac")
    a3 = AudioSegment.from_file("results/soundscapes/node[0]/microphones[2].flac", "flac")
    a4 = AudioSegment.from_file("results/soundscapes/node[0]/microphones[3].flac", "flac")
    a1Samples = np.array(a1.get_array_of_samples())
    a2Samples = np.array(a2.get_array_of_samples())
    a3Samples = np.array(a3.get_array_of_samples())
    a4Samples = np.array(a4.get_array_of_samples())
    dist_a1a2 = 0.1 # meters
    expected = abs(round(dist_a1a2 / speedOfSound, 4))
    calculated = abs(round(tdoa(a1Samples, a1Samples, fs=a1.frame_rate, interp=1)[0], 6))
    assert(calculated == 0)
    calculated = abs(round(tdoa(a1Samples, a2Samples, fs=a1.frame_rate, interp=1)[0], 6))
    assert(1*expected - calculated < 0.001)
    calculated = abs(round(tdoa(a1Samples, a3Samples, fs=a1.frame_rate, interp=1)[0], 6))
    assert(2*expected - calculated < 0.001)
    calculated = abs(round(tdoa(a1Samples, a4Samples, fs=a1.frame_rate, interp=1)[0], 6))
    assert(3*expected - calculated < 0.001)
    print("Test1: Successfull")
    
    print("---------------")
    
    print("Test2: Time delay between nodes using GCC-PHAT")
    for i in np.arange(0, 12, 1):
        for j in np.arange(i, 12, 1):
            a1 = AudioSegment.from_file(paths[i], "flac")
            a2 = AudioSegment.from_file(paths[j], "flac")
            a1Samples = np.array(a1.get_array_of_samples())
            a2Samples = np.array(a2.get_array_of_samples())
            dist_a1a2 = delta * abs(j-i)
            expected = abs(round(dist_a1a2 / speedOfSound, 4))
            calculated = abs(round(tdoa(a1Samples, a2Samples, fs=a1.frame_rate, interp=1)[0], 4))
            assert (expected - calculated) <= 0.001
    print("Test2:Successfull")
    
    print("---------------")
    
    print("Test3: Attenuation")
    for i in np.arange(0, 12, 1):
        a = AudioSegment.from_file(paths[i], "flac")
        a_sils = silence.detect_silence(a, min_silence_len=1000, silence_thresh=-95, seek_step=1)
        a_song = a[a_sils[0][1]:a_sils[1][0]]
        distance = delta * abs(i+1) # +1 because first node is placed at a distance
        assert(abs(a_song.dBFS -attenuation_model_default_params(distance)) < 0.5)
    print("Test3: Successfull")

    


    


