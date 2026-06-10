#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "customRotarySlider.h"
#include "ScreenComponent.h"
#include "FilterTypeSelector.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& state, AutoWah<float>& wah);

    void paint(juce::Graphics& g) override;

    void resized() override;

    void getKnobSize(int* width, int* height);

    void getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect);

    void getKnobRects(juce::Rectangle<float>* filterKnob);

private:
    CustomRotarySlider freqSlider, resSlider, morphSlider;

    FilterTypeSelector filterSelector;

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    attachment freqAttachment;
    attachment resAttachment;
    attachment morphAttachment;

    ScreenComponent screen;

    int knobWidth;
    int knobHeight;
};
