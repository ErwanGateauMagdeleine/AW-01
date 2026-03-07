#pragma once

#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <vector>
#include "AutoWah.h"

TEST_CASE("AutoWah tracks envelope of an increasing amplitude sine wave", "[AutoWah]")
{
    using SampleType = float;

    constexpr int numSamples  = 512;
    constexpr float sampleRate = 44100.0f;
    constexpr float frequency  = 440.0f; // Hz

    const SampleType PI = static_cast<SampleType>(3.14159265359);

    // Setup AutoWah
    AutoWah<SampleType> autoWah;

    constexpr SampleType baseFreq  = 500.0f;
    constexpr SampleType envAmount = 2.0f;

    autoWahSettings<SampleType> settings {
        0.01f,  /* envAttack */
        2.0f,   /* envDecay */
        envAmount,
        baseFreq,
        0.8f,   /* filtRes */
        0.5f    /* filtMorph */
    };

    autoWah.prepare(sampleRate);
    autoWah.updateSettings(settings);

    /* Generate input: sine wave with increasing amplitude */
    std::vector<SampleType> inputSignal(numSamples);
    for (int n = 0; n < numSamples; ++n)
    {
        float t = static_cast<float>(n) / sampleRate;
        float amp = static_cast<float>(n) / numSamples; // linearly increases from 0 to 1
        inputSignal[n] = amp * std::sin(2.0f * PI * frequency * t);
    }

    /* Track filter frequencies per sample */
    std::vector<SampleType> trackedFrequencies(numSamples);
    for (int n = 0; n < numSamples; ++n)
    {
        autoWah.process(inputSignal[n]);
        trackedFrequencies[n] = autoWah.getFilterFrequency();
    }

    SECTION("Filter frequency increases with input amplitude over time")
    {
        /* Check that frequency generally increases */
        int window = sampleRate / frequency; // samples per cycle
        int increasingCount = 0;
        for (int n = window; n < numSamples; ++n)
        {
            if (trackedFrequencies[n] > trackedFrequencies[n - window])
            {
                ++increasingCount;
            }
        }
        REQUIRE(increasingCount > (numSamples - window) * 0.6f);
    }

    SECTION("Filter frequency stays within expected modulation bounds")
    {
        for (auto freq : trackedFrequencies)
        {
            REQUIRE(freq >= baseFreq);
            REQUIRE(freq <= baseFreq + baseFreq * envAmount);
        }
    }
}
