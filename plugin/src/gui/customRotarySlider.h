#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "customLookAndFeel.h"

class CustomRotarySlider : public juce::Slider
{
public:
    CustomRotarySlider(juce::RangedAudioParameter& rap);
    ~CustomRotarySlider();

private:
    juce::RangedAudioParameter* param;
    customLookAndFeel lnf;
};
