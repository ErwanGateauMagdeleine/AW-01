#pragma once

#include <JuceHeader.h>
#include "CustomRotarySlider.h"

class EnvelopeComponent : public juce::Component
{
public:
    EnvelopeComponent(juce::AudioProcessorValueTreeState& state);

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    CustomRotarySlider attackSlider, decaySlider, amountSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    attachment attackAttachment;
    attachment decayAttachment;
    attachment amountAttachment;
};
