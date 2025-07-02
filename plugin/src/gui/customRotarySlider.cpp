#pragma once

#include <JuceHeader.h>
#include "customRotarySlider.h"

//==============================================================================
CustomRotarySlider::CustomRotarySlider(juce::RangedAudioParameter& rap, juce::String knobLabel) :
        param(&rap)
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
