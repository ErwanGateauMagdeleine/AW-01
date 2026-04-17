#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "customRotarySlider.h"

//==============================================================================
CustomRotarySlider::CustomRotarySlider(juce::RangedAudioParameter& rap, juce::String knobLabel) :
        param(&rap)
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setName(knobLabel);
}

CustomRotarySlider::~CustomRotarySlider()
{
}
