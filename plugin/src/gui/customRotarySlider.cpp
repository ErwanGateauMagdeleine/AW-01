#pragma once

#include <JuceHeader.h>
#include "customRotarySlider.h"

//==============================================================================
CustomRotarySlider::CustomRotarySlider(juce::RangedAudioParameter& rap) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&rap)
{
    setLookAndFeel(&lnf);
}

CustomRotarySlider::~CustomRotarySlider()
{
    /* Reset the LookAndFeel to avoid dangling pointer */
    setLookAndFeel(nullptr);
}
