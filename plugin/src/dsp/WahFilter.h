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
enum filters
{
    LPF,
    MIF,
    HPF,
    NUM_FILTERS
};

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
        morphing = static_cast<float>(0.0);
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

    void setMorphing(SampleType newMorphing)
    {
        morphing = newMorphing;

        /* Recalculate filter weights */
        filterWeights[LPF] = static_cast<SampleType>(std::max(0.0, 1.0 - 2.0 * morphing));
        filterWeights[MIF] = static_cast<SampleType>(1 - std::abs(2.0 * morphing - 1.0));
        filterWeights[HPF] = static_cast<SampleType>(std::max(0.0, 2.0 * morphing - 1.0));
    }

    void setIsPeak(bool newIsPeak)
    {
        isPeak = newIsPeak;
    }

    void setGain(SampleType newGain)
    {
        gain = newGain;
    }

    void setFilterParameters(SampleType newCenterFrequency,
                             SampleType newResonance,
                             SampleType newMorphing,
                             bool newIsPeak,
                             SampleType newGain)
    {
        centerFrequency = newCenterFrequency;
        resonance = newResonance;
        morphing = newMorphing;
        gain = newGain;

        /* Recalculate filter weights */
        filterWeights[LPF] = static_cast<SampleType>(std::max(0.0, 1.0 - 2.0 * morphing));
        filterWeights[MIF] = static_cast<SampleType>(1 - std::abs(2.0 * morphing - 1.0));
        filterWeights[HPF] = static_cast<SampleType>(std::max(0.0, 2.0 * morphing - 1.0));

        isPeak = newIsPeak;
    }

    //==============================================================================
    /** Initialize the wah filter */
    void prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
        omegaConst = static_cast<SampleType>(2.0) * std::numbers::pi_v<SampleType> / static_cast<SampleType>(sampleRate);
        computeCoefficients();
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
        SampleType filtersCoefficients[NUM_FILTERS][NUM_COEFFS];

        SampleType omega = omegaConst * centerFrequency;
        SampleType cosOmega = std::cos(omega);
        SampleType sinOmega = std::sin(omega);
        SampleType d = static_cast<SampleType>(1.0 / resonance);
        SampleType beta = static_cast<SampleType>(0.5 * (1.0 - d * sinOmega / 2.0) / (1.0 + d * sinOmega / 2.0));
        SampleType gamma = static_cast<SampleType>((0.5 + beta) * cosOmega);
        SampleType k = static_cast<SampleType>(std::tan(std::numbers::pi_v<SampleType> * centerFrequency / sampleRate));
        SampleType delta = k * k * resonance + k + resonance;

        /* Calculating LPF */
        filtersCoefficients[LPF][A1] = static_cast<SampleType>(-2.0 * gamma);
        filtersCoefficients[LPF][A2] = static_cast<SampleType>(2.0 * beta);
        filtersCoefficients[LPF][B0] = static_cast<SampleType>((0.5 + beta - gamma) / 2.0);
        filtersCoefficients[LPF][B1] = static_cast<SampleType>(0.5 + beta - gamma);
        filtersCoefficients[LPF][B2] = filtersCoefficients[LPF][B0];

        /* Calculating HPF */
        filtersCoefficients[HPF][A1] = filtersCoefficients[LPF][A1];
        filtersCoefficients[HPF][A2] = filtersCoefficients[LPF][A2];
        filtersCoefficients[HPF][B0] = static_cast<SampleType>((0.5 + beta + gamma) / 2.0);
        filtersCoefficients[HPF][B1] = static_cast<SampleType>(-(0.5 + beta + gamma));
        filtersCoefficients[HPF][B2] = filtersCoefficients[HPF][B0];

        /* Calculating Mid FIlter */
        if (isPeak)
        {
            SampleType upsilon = static_cast<SampleType>(std::pow(10.0, gain / 20.0));
            SampleType zeta = static_cast<SampleType>(4.0 / (1.0 + upsilon));
            SampleType zetan = static_cast<SampleType>(zeta * std::tan(omega / (2.0 * resonance)));
            beta = static_cast<SampleType>(0.5 * ((1.0 - zetan) / (1.0 + zetan)));
            gamma = static_cast<SampleType>((0.5 + beta) * cosOmega);
            SampleType c0 = upsilon - 1;
            SampleType d0 = 1;

            filtersCoefficients[MIF][A1] = static_cast<SampleType>((-2.0 * gamma));
            filtersCoefficients[MIF][A2] = static_cast<SampleType>((2.0 * beta));
            filtersCoefficients[MIF][B0] = static_cast<SampleType>(c0 * (0.5 - beta) + d0);
            filtersCoefficients[MIF][B1] = static_cast<SampleType>(d0 * filtersCoefficients[MIF][A1]);
            filtersCoefficients[MIF][B2] = static_cast<SampleType>(-c0 * ((0.5 - beta)) + d0 * filtersCoefficients[MIF][A2]);
        }
        else
        {
            filtersCoefficients[MIF][A1] = static_cast<SampleType>((2.0 * resonance * (k * k - 1.0)) / delta);
            filtersCoefficients[MIF][A2] = (k * k * resonance - k + resonance) / delta;
            filtersCoefficients[MIF][B0] = k / delta;
            filtersCoefficients[MIF][B1] = static_cast<SampleType>(0.0);
            filtersCoefficients[MIF][B2] = -filtersCoefficients[MIF][B0];
        }

        /* Update filter coefficients */
        for (int i = 0; i < NUM_COEFFS; i++)
        {
            coeffs[i] = filterWeights[LPF] * filtersCoefficients[LPF][i] +
                        filterWeights[MIF] * filtersCoefficients[MIF][i] +
                        filterWeights[HPF] * filtersCoefficients[HPF][i];
        }
    }

    double sampleRate;
    SampleType centerFrequency;
    SampleType resonance;
    SampleType morphing;
    SampleType gain;

    SampleType omegaConst;
    SampleType coeffs[NUM_COEFFS];
    SampleType filterWeights[NUM_FILTERS];
    SampleType stateArray[NUM_STATES];

    bool isPeak;
};
