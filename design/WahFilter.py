from enum import Enum
import math
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt
import argparse


class FilterType(Enum):
    """
    Enum class for filter types.
    """
    LPF = 0
    BPF = 1
    HPF = 2


class WahFilter:
    """
    A Wah filter implementation using morphing. The morphing is between LPF,
    BPF and HPF.

    Properties:
    ----------
        sample_rate: The sample rate of the audio signal.

        morph: The morph value (0 to 1) that determines the filter type.
        0 = LPF, 0.5 = BPF, 1 = HPF.

        fc: The cutoff frequency of the filter.

        res: The resonance factor of the filter.
    """
    def __init__(self, sample_rate, morph, fc, res):
        self.sample_rate = sample_rate
        self.morph = morph
        self.fc = fc
        self.res = res

        self.x_mem_tap = np.zeros(2)  # Memory for input samples
        self.y_mem_tap = np.zeros(2)  # Memory for output samples

    def calculate_filter_coeff(self, filter_type):
        """
        Calculate the biquad filter coefficients for HPF, BPF, and LPF.

        Parameters:
            filter_type (FilterType): The type of filter (LPF, BPF, HPF).

        Returns:
            tuple: A tuple containing the filter coefficients
            (a0, a1, a2, b0, b1, b2).
        """
        # Normalize cutoff frequency
        omega = 2 * math.pi * self.fc / self.sample_rate
        sin_omega = math.sin(omega)
        cos_omega = math.cos(omega)

        # Q factor (resonance)
        d = 1 / self.res
        beta = 0.5 * (1 - d * sin_omega / 2) / (1 + d * sin_omega / 2)
        gamma = (0.5 + beta) * cos_omega

        # Initialize coefficient arrays
        b = np.zeros(3)  # [b0, b1, b2]
        a = np.zeros(3)  # [a0, a1, a2]

        if filter_type == FilterType.LPF:
            # Low-Pass Filter
            b[0] = (0.5 + beta - gamma) / 2
            b[1] = 0.5 + beta - gamma
            b[2] = b[0]
            a[0] = 1
            a[1] = -2 * gamma
            a[2] = 2 * beta

        elif filter_type == FilterType.HPF:
            # High-Pass Filter
            b[0] = (0.5 + beta + gamma) / 2
            b[1] = -(0.5 + beta + gamma)
            b[2] = b[0]
            a[0] = 1
            a[1] = -2 * gamma
            a[2] = 2 * beta

        elif filter_type == FilterType.BPF:
            # Band-Pass Filter
            k = math.tan(math.pi * self.fc / self.sample_rate)
            delta = k * k * self.res + k + self.res

            b[0] = k / delta
            b[1] = 0.0
            b[2] = -b[0]
            a[0] = 1
            a[1] = (2 * self.res * (k * k - 1)) / delta
            a[2] = (k * k * self.res - k + self.res) / delta

        return b, a

    def blend_coefficients(self):
        """
        Blends the coefficients of the filters based on the morph value.
        """
        b_lpf, a_lpf = self.calculate_filter_coeff(FilterType.LPF)
        b_bpf, a_bpf = self.calculate_filter_coeff(FilterType.BPF)
        b_hpf, a_hpf = self.calculate_filter_coeff(FilterType.HPF)

        w_lpf = max(0.0, 1 - 2 * self.morph)
        w_bpf = 1 - abs(2 * self.morph - 1)
        w_hpf = max(0.0, 2 * self.morph - 1)

        b = w_lpf * b_lpf + w_bpf * b_bpf + w_hpf * b_hpf
        a = w_lpf * a_lpf + w_bpf * a_bpf + w_hpf * a_hpf

        return b, a

    def process_signal(self, input_signal, filter_frequency):
        """
        Process the input signal through the Wah filter.

        Parameters:
            input_signal (numpy.ndarray): The input audio signal.

            filter_frequency (numpy.ndarray): The filter frequency
            for each sample.
        Returns:
            numpy.ndarray: The filtered output signal.
        """
        output_signal = np.zeros_like(input_signal)

        for idx, sample in enumerate(input_signal):
            self.fc = filter_frequency[idx]
            b, a = self.blend_coefficients()
            output_signal[idx] = (b[0] * sample +
                                  b[1] * self.x_mem_tap[0] +
                                  b[2] * self.x_mem_tap[1] -
                                  a[1] * self.y_mem_tap[0] -
                                  a[2] * self.y_mem_tap[1])

            # Update memory taps
            self.x_mem_tap[1] = self.x_mem_tap[0]
            self.x_mem_tap[0] = sample
            self.y_mem_tap[1] = self.y_mem_tap[0]
            self.y_mem_tap[0] = output_signal[idx]

        return output_signal


def parse_args():
    """ Parses the arguments of the script. """
    parser = argparse.ArgumentParser(
        description="Generates plots of the frequecy response of the"
                    " wah filter.")

    parser.add_argument("--sample-rate",
                        default=44100,
                        type=float,
                        help="Sample rate of the system")

    parser.add_argument("--morph",
                        default=0.5,
                        type=float,
                        help="Morphing value of the Wah filter")

    parser.add_argument("--center-freq",
                        default=1000,
                        type=float,
                        help="Center frequency value of the Wah filter")

    parser.add_argument("--res",
                        default=[0.707, 1, 5],
                        type=float,
                        nargs="*",
                        help="Resonance value of the Wah filter")

    return parser.parse_args()


if __name__ == "__main__":
    legend = []

    args = parse_args()

    plt.figure(figsize=(10, 6))

    for res in args.res:
        wah_filter = WahFilter(args.sample_rate,
                               args.morph,
                               args.center_freq,
                               res)
        b, a = wah_filter.blend_coefficients()
        w, h = signal.freqz(b, a, fs=args.sample_rate, worN=1024)
        plt.semilogx(w, 20 * np.log(np.abs(h)))
        legend.append(f"res = {res:.2f}")

    plt.legend(legend)
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.grid()
    plt.ylim(-40, max(20 * np.log(np.abs(h))) + 5)

    plt.show()
