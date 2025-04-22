from WahFilter import WahFilter
from EnvelopeFollower import EnvelopeFollower
import numpy as np
import utils
import sounddevice as sd
import argparse

class AutoWah:
    """ AutoWah effect that combines an envelope follower and a wah filter.

    Properties:
    ----------
        sample_rate: The sample rate of the audio signal.

        envelope_attack: The time constant for the attack phase (in seconds).

        envelope_decay: The time constant for the decay phase (in seconds).

        filter_fc: The center frequency of the wah filter.

        filter_res: The Q factor of the wah filter.

        filter_morph: The morph factor for the wah filter.

        amount: The quantity of the wah effect (-1 to 1).
    """
    def __init__(self,
                 sample_rate,
                 amount,
                 attack_time,
                 decay_time,
                 center_freq,
                 res,
                 morph_factor,
                 ):
        self.sample_rate = sample_rate
        self.attack_time = attack_time
        self.decay_time = decay_time
        self.center_freq = center_freq
        self.res = res
        self.morph_factor = morph_factor
        self.amount = amount

        self.envelope_follower = EnvelopeFollower(sample_rate, attack_time, decay_time)
        self.wah_filter = WahFilter(sample_rate, morph_factor, center_freq, res)

    def process_signal(self, signal):
        """ Process the input signal through the wah effect.

        Parameters:
            signal: The input audio signal (numpy array).
        """
        envelope = self.envelope_follower.get_envelope(signal)
        filter_freq = self.center_freq + (self.center_freq * envelope * self.amount)
        filter_freq = np.clip(filter_freq, 20, self.sample_rate / 2 - 1)

        processed_signal = self.wah_filter.process_signal(signal, filter_freq)

        return processed_signal


def parse_args():
    """
    Parses command line arguments.

    Returns:
        argparse.Namespace: Parsed command line arguments.
    """
    parser = argparse.ArgumentParser(description="Envelope test script")

    parser.add_argument('--input_file', type=str, required=True, help='Path to the input audio file')
    parser.add_argument('--attack_time', type=float, default=0.00001, help='Attack time in seconds')
    parser.add_argument('--decay_time', type=float, default=0.09, help='decay time in seconds')
    parser.add_argument('--effect_amount', type=float, default=5 , help='Amount of wah effect to apply')
    parser.add_argument('--fc', type=float, default=250, help='Center frequency of the wah filter.')
    parser.add_argument('--res', type=float, default=10.0, help='Resonance of the wah filter.')
    parser.add_argument('--morph', type=float, default=0.8, help='Morph factor of the wah filter.')

    return parser.parse_args()


if __name__ == "__main__":
    parsed_args = parse_args()

    sample_rate, num_channels, channels_data = utils.get_audio(parsed_args.input_file)

    # If stereo, take only one channel for processing
    if num_channels > 1:
        channels_data = channels_data[:, 0]

    auto_wah = AutoWah(sample_rate,
                       parsed_args.effect_amount,
                       parsed_args.attack_time,
                       parsed_args.decay_time,
                       parsed_args.fc,
                       parsed_args.res,
                       parsed_args.morph)

    processed_signal = auto_wah.process_signal(channels_data)

    # Play the audio
    sd.play(channels_data, samplerate=sample_rate)
    sd.wait()
    sd.play(processed_signal, samplerate=sample_rate)
    sd.wait()
