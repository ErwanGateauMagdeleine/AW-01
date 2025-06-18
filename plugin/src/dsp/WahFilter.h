#pragma once

#include "cmath"

/** Enumeration of the coefficient indexes. */
enum filterCoefficients
{
    A0,
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
    BPF,
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
    WahFilter(){}

    //==============================================================================
    /**
     * Sets the new center frequency of the wah filter. It recalculates the filter
     * coefficients.
     */
    void setCenterFrequency(SampleType newCenterFrequency)
    {
        centerFrequency = newCenterFrequency;
        computeCoefficients();
    }

    void setResonance(SampleType newResonance)
    {
        resonance = newResonance;
        computeCoefficients();
    }

    void setMorphing(SampleType newMorphing)
    {
        morphing = newMorphing;

        /* Recalculate filter weights */
        filterWeights[LPF] = std::max(0.0, 1.0 - 2.0 * morphing);
        filterWeights[BPF] = 1 - std::abs(2.0 * morphing - 1.0);
        filterWeights[HPF] = std::max(0.0, 2.0 * morphing - 1.0);

        computeCoefficients();
    }

    //==============================================================================
    /** Initialize the wah filter */
    void prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
        omegaConst = 2.0 * PI / sampleRate;
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
    void process (SampleType* sample)
    {
        /* Compute the output */
        SampleType yn = coeffs[B0] * (*sample) +
                        coeffs[B1] * stateArray[X_Z1] +
                        coeffs[B2] * stateArray[X_Z2] -
                        coeffs[A1] * stateArray[Y_Z1] -
                        coeffs[A2] * stateArray[Y_Z2];

        /* Update states */
        stateArray[X_Z2] = stateArray[X_Z1];
        stateArray[X_Z1] = (*sample);

        stateArray[Y_Z2] = stateArray[Y_Z1];
        stateArray[Y_Z1] = yn;

        /* Update the in-place processing of the sample. */
        (*sample) = yn;
    }

private:

    void computeCoefficients()
    {
        SampleType filtersCoefficients[NUM_FILTERS][NUM_COEFFS];
        SampleType omega = omegaConst * centerFrequency;
        SampleType cosOmega = std::cos(omega);
        SampleType sinOmega = std::sin(omega);
        SampleType alpha = sinOmega / (2.0 * resonance);

        /* Calculating LPF */
        filtersCoefficients[LPF][A0] = 1.0 + alpha;
        filtersCoefficients[LPF][A1] = -2.0 * cosOmega;
        filtersCoefficients[LPF][A2] = 1.0 - alpha;
        filtersCoefficients[LPF][B0] = (1.0 - cosOmega) / 2.0;
        filtersCoefficients[LPF][B1] = 1.0 - cosOmega;
        filtersCoefficients[LPF][B2] = (1.0 - cosOmega) / 2.0;

        /* Calculating BPF */
        filtersCoefficients[BPF][A0] = filtersCoefficients[LPF][A0];
        filtersCoefficients[BPF][A1] = filtersCoefficients[LPF][A1];
        filtersCoefficients[BPF][A2] = filtersCoefficients[LPF][A2];
        filtersCoefficients[BPF][B0] = alpha;
        filtersCoefficients[BPF][B1] = 0.0;
        filtersCoefficients[BPF][B2] = -alpha;

        /* Calculating HPF */
        filtersCoefficients[HPF][A0] = filtersCoefficients[LPF][A0];
        filtersCoefficients[HPF][A1] = filtersCoefficients[LPF][A1];
        filtersCoefficients[HPF][A2] = filtersCoefficients[LPF][A2];
        filtersCoefficients[HPF][B0] = (1.0 + cosOmega) / 2.0;
        filtersCoefficients[HPF][B1] = -(1.0 + cosOmega) / 2.0;
        filtersCoefficients[HPF][B2] = (1.0 + cosOmega) /2.0;

        /* Normalize Coefficients */
        for (int i = 0; i < NUM_FILTERS; i++)
        {
            for (int j = 0; j < NUM_COEFFS; j++)
            {
                filtersCoefficients[i][j] /= filtersCoefficients[i][A0];
            }
        }

        /* Update filter coefficients */
        for (int i = 0; i < NUM_COEFFS; i++)
        {
            coeffs[i] = filterWeights[LPF] * filtersCoefficients[LPF][i] +
                        filterWeights[BPF] * filtersCoefficients[BPF][i] +
                        filterWeights[HPF] * filtersCoefficients[HPF][i];
        }
    }

    double sampleRate;
    SampleType centerFrequency;
    SampleType resonance;
    SampleType morphing;

    SampleType omegaConst;
    const SampleType PI = static_cast<SampleType>(3.14159265359);
    SampleType coeffs[NUM_COEFFS];
    SampleType filterWeights[NUM_FILTERS];
    SampleType stateArray[NUM_STATES];
};
