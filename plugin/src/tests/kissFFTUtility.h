#include <vector>

float getMagnitudeAtFrequency(const std::vector<float>& magnitude, float targetFreq, float sampleRate, int fftSize);

void getFFTResponseFromSignal(const std::vector<float>& signal, std::vector<float>& magnitude);
