#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "CustomRotarySlider.h"
#include "ScreenComponent.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& state);

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    CustomRotarySlider freqSlider, resSlider, morphSlider;

    ScreenComponent screen;
    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    attachment freqAttachment;
    attachment resAttachment;
    attachment morphAttachment;
};
