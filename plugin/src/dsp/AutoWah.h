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

    SampleType process(SampleType sample)
    {
        /* Get the enveloppe */
        envelope = envFollower.process(sample);

        /* Use the envelope follower to update the filter center frequency.
           Cap the filter frequency to the audible range. */
        filterFrequency = static_cast<SampleType>(std::clamp(settings.filtFreq * (1 + settings.envAmnt * envelope), 500.0f, 15000.0f));
        wahFilt.setCenterFrequency(filterFrequency);

        /* Process the sample with the filter */
        SampleType yn = wahFilt.process(sample);

        return yn;
    }

    void updateSettings(autoWahSettings<SampleType> newSettings)
    {
        settings = newSettings;

        envFollower.setAttackTime(settings.envAttack);
        envFollower.setDecayTime(settings.envDecay);

        wahFilt.setFilterParameters(settings.filtFreq, settings.filtRes, settings.filtMorph);
    }

    void updateEnvAttack(SampleType newAttack)
    {
        settings.envAttack = newAttack;
        envFollower.setAttackTime(settings.envAttack);
    }

    void updateEnvDecay(SampleType newDecay)
    {
        settings.envDecay = newDecay;
        envFollower.setDecayTime(settings.envDecay);
    }

    void updateEnvAmount(SampleType newAmount)
    {
        settings.envAmnt = newAmount;
    }

    void updateFiltFreq(SampleType newFreq)
    {
        settings.filtFreq = newFreq;
        wahFilt.setCenterFrequency(settings.filtFreq);
    }

    void updateFiltMorph(SampleType newMorph)
    {
        settings.filtMorph = newMorph;
        wahFilt.setMorphing(settings.filtMorph);
    }

    void updateFiltRes(SampleType newRes)
    {
        settings.filtRes = newRes;
        wahFilt.setResonance(settings.filtRes);
    }

    SampleType getFilterFrequency()
    {
        return filterFrequency;
    }

private:
    EnvelopeFollower<SampleType> envFollower;
    WahFilter<SampleType> wahFilt;

    autoWahSettings<SampleType> settings;

    /* Keep values internal to the object. */
    SampleType envelope;
    SampleType filterFrequency;
};
