#pragma once

#include <JuceHeader.h>
#include "CustomRotarySlider.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& state);

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    CustomRotarySlider freqSlider, resSlider, morphSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    attachment freqAttachment;
    attachment resAttachment;
    attachment morphAttachment;
};
