#include "kissfftUtility.h"
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "cmath"

float getMagnitudeAtFrequency(const std::vector<float>& magnitude, float targetFreq, float sampleRate, int fftSize)
{
    int bin = static_cast<int>((targetFreq / sampleRate) * fftSize);
    if (bin >= magnitude.size())
    {
        bin = magnitude.size() - 1;
    }
    return magnitude[bin];
}

void getFFTResponseFromSignal(const std::vector<float>& signal, std::vector<float>& magnitude)
{
    const int N = signal.size();

    /* Ensure output vector is sized properly. */
    magnitude.resize(N / 2 + 1);

    /* Allocate FFT config and output spectrum */
    kiss_fftr_cfg cfg = kiss_fftr_alloc(N, 0, nullptr, nullptr);
    std::vector<kiss_fft_cpx> spectrum(N / 2 + 1);

    /* Perform real-to-complex FFT */
    kiss_fftr(cfg, signal.data(), spectrum.data());

    /* Compute Magnitude spectrum */
    for (int i = 0; i <= N / 2; i++)
    {
        float re = spectrum[i].r;
        float im = spectrum[i].i;

        magnitude[i] = std::sqrt(re * re + im * im);
    }

    free(cfg);
}
