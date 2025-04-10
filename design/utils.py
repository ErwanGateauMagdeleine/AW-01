import numpy as np
from scipy.io import wavfile

def get_file_num_channels(signal):
    """
    Determines the number of channels in the audio signal.

    Parameters:
        signal (numpy.ndarray): The input audio signal.

    Returns:
            int: The number of channels in the audio signal.
    """
    # Determine the number of channels
    if signal.ndim == 1:
        # Mono
        num_channels = 1
    else:
        # Stereo or more
        num_channels = signal.shape[1]

    return num_channels


def get_audio(file_name):
    """
    Reads an audio file and returns the sample rate and signal.

    Parameters:
        file_name (str): Path to the audio file.

    Returns:
        tuple: Sample rate, nummber of channels and audio signal.
    """
    sample_rate, signal = wavfile.read(file_name)
    num_channel = get_file_num_channels(signal)

    # Determine the maximum value based on the signal's data type
    max_value = np.iinfo(signal.dtype).max

    # Normalize the signal between -1 and 1 using the byte depth max value
    signal = signal / max_value

    return sample_rate, num_channel, signal