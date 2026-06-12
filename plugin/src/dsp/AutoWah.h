#pragma once

#include "EnvelopeFollower.h"
#include "WahFilter.h"
#include "cmath"

template <typename SampleType>
struct autoWahSettings
{
    SampleType envAttack{ 0 }, envDecay{ 0 }, envAmnt{ 0 };
    SampleType filtFreq{ 0 }, filtRes{ 0 }, filtMorph{ 0 };
    SampleType filtGain{ 0 };
    bool isPeak;
};

template <typename SampleType>
class AutoWah
{
public:
    AutoWah(SampleType minFiltFreq, SampleType maxFiltFreq)
    {
        minFreq = minFiltFreq;
        maxFreq = maxFiltFreq;
    }

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
        filterFrequency = static_cast<SampleType>(std::clamp(settings.filtFreq * (1 + settings.envAmnt * envelope), minFreq, maxFreq));
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

        wahFilt.setFilterParameters(settings.filtFreq,
                                    settings.filtRes,
                                    settings.filtMorph,
                                    settings.isPeak,
                                    settings.filtGain);
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

    void updateFilterGain(SampleType newGain)
    {
        settings.filtGain = newGain;
        wahFilt.setGain(settings.filtRes);
    }

    SampleType getFilterFrequency()
    {
        return filterFrequency;
    }

    SampleType getMagnitudeFromFrequency(SampleType frequency)
    {
        return wahFilt.getMagnitudeFromFrequency(frequency);
    }

    double getSampleRate()
    {
        return wahFilt.getSampleRate();
    }

    void updateFilterType(bool isPeak)
    {
        settings.isPeak = isPeak;
        wahFilt.setIsPeak(isPeak);
    }

private:
    EnvelopeFollower<SampleType> envFollower;
    WahFilter<SampleType> wahFilt;

    autoWahSettings<SampleType> settings;

    SampleType envelope;
    SampleType filterFrequency;

    SampleType minFreq;
    SampleType maxFreq;
};
