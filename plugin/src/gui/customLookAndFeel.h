#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

class customLookAndFeel : public juce::LookAndFeel_V4
{
public:
    customLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int, int, int, int,
                          float sliderPos, float, float, juce::Slider&) override;

private:
    juce::Image knobImage;
    int knobFrameWidth = 0;
    int knobTotalFrames = 0;
};
