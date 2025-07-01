#pragma once

#include <JuceHeader.h>
#include "customRotarySlider.h"

//==============================================================================
CustomRotarySlider::CustomRotarySlider(juce::RangedAudioParameter& rap) :
        param(&rap)
{
    setLookAndFeel(&lnf);
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow,
                    false,
                    lnf.getKnobFrameHeightAndWidth() - 10,
                    textBoxHeight);
}

CustomRotarySlider::~CustomRotarySlider()
{
    /* Reset the LookAndFeel to avoid dangling pointer */
    setLookAndFeel(nullptr);
}

//==============================================================================
int CustomRotarySlider::getHeight()
{
    return lnf.getKnobFrameHeightAndWidth() + textBoxHeight;
}

int CustomRotarySlider::getWidth()
{
    return lnf.getKnobFrameHeightAndWidth();
}
