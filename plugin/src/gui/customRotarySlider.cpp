#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "customRotarySlider.h"

//==============================================================================
CustomRotarySlider::CustomRotarySlider(juce::RangedAudioParameter& rap, juce::String knobLabel, juce::Font newFont, juce::Colour newFontColour) :
        param(&rap),
        lnf(newFont, newFontColour)
{
    setLookAndFeel(&lnf);
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setName(knobLabel);
}

CustomRotarySlider::~CustomRotarySlider()
{
    /* Reset the LookAndFeel to avoid dangling pointer */
    setLookAndFeel(nullptr);
}

//==============================================================================
int CustomRotarySlider::getHeight()
{
    return lnf.getKnobHeight() + textBoxHeight;
}

int CustomRotarySlider::getWidth()
{
    return lnf.getKnobWidth();
}
