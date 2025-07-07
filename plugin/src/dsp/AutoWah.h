#pragma once

#include "EnvelopeFollower.h"
#include "WahFilter.h"
#include "cmath"

template <typename SampleType>
struct autoWahSettings
{
    SampleType envAttack{ 0 }, envDecay{ 0 }, envAmnt{ 0 };
    SampleType filtFreq{ 0 }, filtRes{ 0 }, filtMorph{ 0 };
};

template <typename SampleType>
class AutoWah
{
public:
    AutoWah(){};

    void prepare(double newSampleRate)
    {
        envFollower.prepare(newSampleRate);
        wahFilt.prepare(newSampleRate);
    }

    void reset()
    {
        envFollower.reset();
        wahFilt.reset();
    }

    void process(SampleType* sample)
    {
        SampleType envelope = (*sample);

        /* Get the enveloppe */
        envFollower.process(&envelope);

        /* Use the envelope follower to update the filter center frequency.
           Cap the filter frequency to the audible range. */
        SampleType filterFrequency = std::max(settings.filtFreq + settings.envAmnt * envelope, 20000);
        wahFilt.setCenterFrequency(filterFrequency);

        /* Process the sample with the filter */
        wahFilt.process(sample);
    }

    void updateSettings(autoWahSettings<SampleType> newSettings)
    {
        settings = newSettings;

        envFollower.setAttackTime(settings.envAttack);
        envFollower.setDecayTime(settings.envDecay);

        wahFilt.setFilterParameters(settings.filtFreq, settings.filtRes, settings.filtMorph);
    }

private:
    EnvelopeFollower<SampleType> envFollower;
    WahFilter<SampleType> wahFilt;

    autoWahSettings<SampleType> settings;
};
