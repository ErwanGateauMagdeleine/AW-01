import numpy as np
import argparse
import matplotlib.pyplot as plt
import utils


class EnvelopeFollower:
    """
    A simple envelope follower that tracks the amplitude envelope of an audio signal with
    adjustable attack and decay times.

    Properties:
    ----------
        sample_rate: The sample rate of the audio signal.

        attack_time: The time constant for the attack phase (in seconds).

        decay_time: The time constant for the decay phase (in seconds).
    """
    def __init__(self, sample_rate, attack_time=0.00001, decay_time=0.1):
        self.sample_rate = sample_rate
        self.attack_time = attack_time
        self.decay_time = decay_time

    def get_envelope(self, signal):
        """
        Computes the envelope of the input signal using a simple attack-decay model.

        Parameters:
            signal: The input audio signal (numpy array).

        Returns:
            envelope: The computed envelope of the input signal (numpy array).
        """
        # Initialize the envelope array
        envelope = np.zeros_like(signal)

        # Compute the attack and decay coefficients
        attack_coeff = np.exp(-1.0 / (self.attack_time * self.sample_rate))
        decay_coeff = np.exp(-1.0 / (self.decay_time * self.sample_rate))

        for i in range(1, len(signal)):
            if signal[i] > envelope[i - 1]:
                # Attack phase
                envelope[i] = attack_coeff * envelope[i - 1] + (1 - attack_coeff) * abs(signal[i])
            else:
                # decay phase
                envelope[i] = decay_coeff * envelope[i - 1] + (1 - decay_coeff) * abs(signal[i])

        return envelope


def parse_args():
    """
    Parses command line arguments.

    Returns:
        argparse.Namespace: Parsed command line arguments.
    """
    parser = argparse.ArgumentParser(description="Envelope test script")

    parser.add_argument('--attack_time', type=float, default=0.00001, help='Attack time in seconds')
    parser.add_argument('--decay_time', type=float, default=0.09, help='decay time in seconds')
    parser.add_argument('--input_file', type=str, required=True, help='Path to the input audio file')

    return parser.parse_args()


if __name__ == "__main__":
    # Parse arguments
    parsed_args = parse_args()

    sample_rate, num_channels, channels_data = utils.get_audio(parsed_args.input_file)

    print(f"Sample Rate: {sample_rate}, Number of Channels: {num_channels}")
    # Create an envelope follower instance
    envelope_follower = EnvelopeFollower(sample_rate, attack_time=parsed_args.attack_time, decay_time=parsed_args.decay_time)

    # Plot the audio signal
    plt.figure(figsize=(10, 6))
    for i in range(num_channels):
        if num_channels > 1:
            plt.subplot(num_channels, 1, i + 1)
        else:
            plt.subplot(1, 1, 1)

        # Get the current channel data
        chan = channels_data[:, i] if num_channels > 1 else channels_data

        # Create temporal vector
        t = np.linspace(0, len(chan) / sample_rate, num=len(chan))

        # Compute the envelope
        envelope = envelope_follower.get_envelope(chan)

        # Plot the audio signal + computed envelope
        plt.title(f"Audio Signal Channel {i + 1}")
        plt.xlabel("Time (s)")
        plt.ylabel("Amplitude")
        plt.grid()
        plt.plot(t, chan)
        plt.plot(t, envelope)

    plt.show()
