/* AW-01 - AutoWah JUCE based audio plugin
   Copyright (C) 2026 Erwan Gateau-Magdeleine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "cmath"
#include <complex>
#include <numbers>
#include <cmath>

/** Enumeration of the coefficient indexes. */
enum filterCoefficients
{
    A1,
    A2,
    B0,
    B1,
    B2,
    NUM_COEFFS
};

/** Enumeration of the filters present in the wah filter. */
typedef enum filters
{
    LPF,
    BPF,
    HPF,
    NUM_FILTERS
} filters_t;

/** Enumeration of the filter state */
enum filterState
{
    X_Z1,
    X_Z2,
    Y_Z1,
    Y_Z2,
    NUM_STATES
};

template <typename SampleType>
class WahFilter
{
public:
    //==============================================================================
    /** Constructor */
    WahFilter()
    {
        centerFrequency = static_cast<float>(1000.0);
        resonance = static_cast<float>(0.707);
    }

    //==============================================================================
    /**
     * Sets the new center frequency of the wah filter. It recalculates the filter
     * coefficients.
     */
    void setCenterFrequency(SampleType newCenterFrequency)
    {
        centerFrequency = newCenterFrequency;
    }

    void setResonance(SampleType newResonance)
    {
        resonance = newResonance;
    }

    void setFilterType(filters_t newFilterType)
    {
        filterType = newFilterType;
    }

    void setFilterParameters(SampleType newCenterFrequency,
                             SampleType newResonance,
                             filters_t newFilterType)
    {
        centerFrequency = newCenterFrequency;
        resonance = newResonance;
        filterType = newFilterType;
    }

    //==============================================================================
    /** Initialize the wah filter */
    void prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
        omegaConst = static_cast<SampleType>(2.0) * std::numbers::pi_v<SampleType> / static_cast<SampleType>(sampleRate);

        reset();
    }

    /** Reset the internal state of the wah filter */
    void reset()
    {
        for (int i = 0; i < NUM_STATES; i++)
        {
            stateArray[i] = 0.0;
        }
    }

    /** Process a single sample of data */
    SampleType process (SampleType sample)
    {
        /* Compute the filter coefficients */
        computeCoefficients();

        /* Compute the output */
        SampleType yn = coeffs[B0] * sample +
                        coeffs[B1] * stateArray[X_Z1] +
                        coeffs[B2] * stateArray[X_Z2] -
                        coeffs[A1] * stateArray[Y_Z1] -
                        coeffs[A2] * stateArray[Y_Z2];

        /* Update states */
        stateArray[X_Z2] = stateArray[X_Z1];
        stateArray[X_Z1] = sample;

        stateArray[Y_Z2] = stateArray[Y_Z1];
        stateArray[Y_Z1] = yn;

        return yn;
    }

    SampleType getMagnitudeFromFrequency(SampleType frequency)
    {
        constexpr std::complex<SampleType> j (0, 1);
        std::complex<SampleType> numerator = 0.0;
        std::complex<SampleType> denominator = 1.0;
        std::complex <SampleType> jw = std::exp(-2 * std::numbers::pi_v<SampleType> * frequency * j / static_cast<SampleType>(sampleRate));

        std::complex<SampleType> factor = jw;

        for (int i = 0; i <= A2; i++)
        {
            denominator += static_cast<SampleType>(coeffs[i]) * factor;
            factor *= jw;
        }

        factor = 1.0f;
        for (int i = B0; i <= B2; i++)
        {
            numerator += static_cast<SampleType>(coeffs[i]) * factor;
            factor *= jw;
        }

        return std::abs(numerator / denominator);
    }

    double getSampleRate()
    {
        return sampleRate;
    }

private:

    void computeCoefficients()
    {
        SampleType omega = omegaConst * centerFrequency;
        SampleType cosOmega = std::cos(omega);
        SampleType sinOmega = std::sin(omega);

        switch (filterType)
        {
            case LPF:
            {
                SampleType d = static_cast<SampleType>(1.0 / resonance);
                SampleType beta = static_cast<SampleType>(0.5 * (1.0 - d * sinOmega / 2.0) / (1.0 + d * sinOmega / 2.0));
                SampleType gamma = static_cast<SampleType>((0.5 + beta) * cosOmega);

                coeffs[A1] = static_cast<SampleType>(-2.0 * gamma);
                coeffs[A2] = static_cast<SampleType>(2.0 * beta);
                coeffs[B0] = static_cast<SampleType>((0.5 + beta - gamma) / 2.0);
                coeffs[B1] = static_cast<SampleType>(0.5 + beta - gamma);
                coeffs[B2] = coeffs[B0];

                break;
            }
            case HPF:
            {
                SampleType d = static_cast<SampleType>(1.0 / resonance);
                SampleType beta = static_cast<SampleType>(0.5 * (1.0 - d * sinOmega / 2.0) / (1.0 + d * sinOmega / 2.0));
                SampleType gamma = static_cast<SampleType>((0.5 + beta) * cosOmega);

                coeffs[A1] = static_cast<SampleType>(-2.0 * gamma);
                coeffs[A2] = static_cast<SampleType>(2.0 * beta);
                coeffs[B0] = static_cast<SampleType>((0.5 + beta + gamma) / 2.0);
                coeffs[B1] = static_cast<SampleType>(-(0.5 + beta + gamma));
                coeffs[B2] = coeffs[B0];

                break;
            }
            case BPF:
            {
                SampleType k = static_cast<SampleType>(std::tan(std::numbers::pi_v<SampleType> * centerFrequency / sampleRate));
                SampleType delta = k * k * resonance + k + resonance;

                coeffs[A1] = static_cast<SampleType>((2.0 * resonance * (k * k - 1.0)) / delta);
                coeffs[A2] = (k * k * resonance - k + resonance) / delta;
                coeffs[B0] = k / delta;
                coeffs[B1] = static_cast<SampleType>(0.0);
                coeffs[B2] = -coeffs[B0];

                break;
            }
        }
    }

    double sampleRate;
    SampleType centerFrequency;
    SampleType resonance;
    filters_t filterType;

    SampleType omegaConst;
    SampleType coeffs[NUM_COEFFS];
    SampleType stateArray[NUM_STATES];
};
