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

    void getButtonRect(juce::Rectangle<float>* button);

    std::function<void(bool)> onChange;

    void setFilterType(bool isPeak);

    void getButtonsStates(bool* peakState, bool* bandState);

    void triggerPeakButtonClick();

    void triggerBandButtonClick();

private:
    CustomRotarySlider freqSlider, resSlider, morphSlider;

    FilterTypeSelector filterSelector;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;

    SliderAttachment freqAttachment;
    SliderAttachment resAttachment;
    SliderAttachment morphAttachment;

    ButtonAttachment typeAttachment;

    ScreenComponent screen;

    int knobWidth;
    int knobHeight;
};
