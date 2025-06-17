#pragma once

#include "cmath"

template <typename SampleType>
class WahFilter
{
public:
    //==============================================================================
    /** Constructor */
    WahFilter(){}

    //==============================================================================
    /** Initialize the wah filter */
    void prepare(double newSampleRate)
    {
        sampleRate = newSampleRate;
    }

    /** Reset the internal state of the wah filter */
    void reset() {}

    /** Process a single sample of data */
    void process (SampleType* sample) {}

private:
    double sampleRate;
};
