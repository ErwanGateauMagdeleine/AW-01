#pragma once

#include "BinaryData.h"
#include "customLookAndFeel.h"

//==============================================================================
customLookAndFeel::customLookAndFeel()
{
    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);

    /* Knob frames are square */
    knobFrameWidth = knobImage.getWidth();
    knobTotalFrames = knobImage.getHeight() / knobFrameWidth;
}

//==============================================================================
void customLookAndFeel::drawRotarySlider(juce::Graphics& g, int, int, int, int,
                                         float sliderPos, float, float, juce::Slider&)
{
    if (!knobImage.isValid() || knobTotalFrames <= 0)
    {
        /* This means that there is an error */
        return;
    }

    const int frame = juce::jlimit(0, knobTotalFrames - 1, static_cast<int>(std::round(sliderPos * (knobTotalFrames - 1))));

    g.drawImage(knobImage, 0, 0, knobFrameWidth, knobFrameWidth, 0, frame * knobFrameWidth, knobFrameWidth, knobFrameWidth);
}

//==============================================================================
int customLookAndFeel::getKnobFrameHeightAndWidth()
{
    /* Return one single value as the knob image is square. */
    return knobFrameWidth;
}
