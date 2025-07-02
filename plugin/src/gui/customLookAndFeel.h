#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class customLookAndFeel : public juce::LookAndFeel_V4
{
public:
    customLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float, float, juce::Slider& slider) override;

    int getKnobHeight();
    int getKnobWidth();

private:
    juce::Image knobImage;
    int knobFrameWidth = 0;
    int knobTotalFrames = 0;

    int labelHeight = 12;
    float labelFontSize = 12.0f;
};
