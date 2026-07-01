#include <catch2/catch_all.hpp>
#include <numbers>
#include <span>
#include "PluginProcessor.h"

static juce::AudioBuffer<float> monoToStereo(const std::vector<float>& mono)
{
    juce::AudioBuffer<float> buffer(2, static_cast<int>(mono.size()));

    for (int i = 0; i < 2; i++)
    {
        buffer.copyFrom(i, 0, mono.data(), static_cast<int>(mono.size()));
    }

    return buffer;
}

TEST_CASE("Matching outputs", "[process]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    /* This tests that processing audio using the plugin processor gives the same
       resut as using the AutoWah object directly. */
    constexpr int numSamples  = 512;
    constexpr int samplesPerBlock = 512;
    constexpr double sampleRate = 44100.0f;
    constexpr float frequency  = 440.0f;
    juce::MidiBuffer midi;

    REQUIRE(numSamples % samplesPerBlock == 0);

    AudioPluginAudioProcessor processor;
    AutoWah<float> autoWah(50.0f, 15000.0f);

    std::vector<float> input(numSamples);
    std::vector<float> autoWahOutput(numSamples);
    std::vector<float> PluginChannel0Output(numSamples);
    std::vector<float> PluginChannel1Output(numSamples);

    /* Initialize the Plugin Processor */
    processor.prepareToPlay(sampleRate, samplesPerBlock);

    auto * param = processor.parameters.getParameter("Envelope Follower Amount");
    param->setValueNotifyingHost(param->convertTo0to1(5.0f));

    /* Initialize the AutoWah */
    autoWahSettings<float> settings
    {
        100.0f, /* envAttack */
        500.0f, /* envDecay */
        5.0f,   /* envAmount */
        1000.0,  /* filtCutoff */
        0.7f,   /* filtRes */
        0.5f    /* filtMorph */
    };
    autoWah.prepare(sampleRate);
    autoWah.updateSettings(settings);

    /* Generate sinus input signal with increasing amp */
    for (int n = 0; n < numSamples; ++n)
    {
        float t = static_cast<float>(n) / sampleRate;
        float amp = static_cast<float>(n) / numSamples;
        input[n] = amp * std::sin(2.0f * std::numbers::pi_v<float> * frequency * t);
    }

    /* Process the samples using AutoWah Object */
    for (int n = 0; n < numSamples; ++n)
    {
        autoWahOutput[n] = autoWah.process(input[n]);
    }

    for (int n = 0; n < numSamples; n += samplesPerBlock)
    {
        std::vector<float> block(std::span<const float>(input).subspan(n, samplesPerBlock).begin(),
                                 std::span<const float>(input).subspan(n, samplesPerBlock).end());

        auto buffer = monoToStereo(block);
        processor.processBlock(buffer, midi);

        auto* channel0Data = buffer.getReadPointer(0);
        std::copy(channel0Data, channel0Data + samplesPerBlock, PluginChannel0Output.data() + n);
        auto* channel1Data = buffer.getReadPointer(1);
        std::copy(channel1Data, channel1Data + samplesPerBlock, PluginChannel1Output.data() + n);
    }

    for (int n = 0; n < numSamples; n++)
    {
        REQUIRE(autoWahOutput[n] == Catch::Approx(PluginChannel0Output[n]).margin(0.0001));
        REQUIRE(autoWahOutput[n] == Catch::Approx(PluginChannel1Output[n]).margin(0.0001));
    }
}
