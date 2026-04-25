#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "CustomRotarySlider.h"

class EnvelopeComponent : public juce::Component
{
public:
    EnvelopeComponent(juce::AudioProcessorValueTreeState& state);

    void paint(juce::Graphics& g) override;

    void resized() override;

    void getKnobSize(int* width, int* height);

private:
    CustomRotarySlider attackSlider, decaySlider, amountSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    attachment attackAttachment;
    attachment decayAttachment;
    attachment amountAttachment;

    int knobWidth;
    int knobHeight;
};
