# AW-01 - AutoWah JUCE based audio plugin
# Copyright (C) 2026 Erwan Gateau-Magdeleine
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program. If not, see <https://www.gnu.org/licenses/>.

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