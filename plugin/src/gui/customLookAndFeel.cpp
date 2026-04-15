#pragma once

#include "BinaryData.h"
#include "customLookAndFeel.h"
#include "customRotarySlider.h"

//==============================================================================
customLookAndFeel::customLookAndFeel(juce::Font newFont, juce::Colour newFontColour) :
        font(std::move(newFont))
{
    fontColour = newFontColour;

    knobImage = juce::ImageCache::getFromMemory(BinaryData::knob_png, BinaryData::knob_pngSize);

    /* Knob frames are square */
    knobFrameWidth = knobImage.getWidth();
    knobTotalFrames = knobImage.getHeight() / knobFrameWidth;
}

//==============================================================================
void customLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int,
                                         float sliderPos, float, float, juce::Slider& slider)
{
    if (!knobImage.isValid() || knobTotalFrames <= 0)
    {
        /* This means that there is an error */
        return;
    }

    /* Draw the knob image */
    const int frame = juce::jlimit(0, knobTotalFrames - 1, static_cast<int>(std::round(sliderPos * (knobTotalFrames - 1))));
    g.drawImage(knobImage, x, y, knobFrameWidth, knobFrameWidth, 0, frame * knobFrameWidth, knobFrameWidth, knobFrameWidth);

    g.setColour(fontColour);
    g.setFont(font.withHeight(10.0));
    g.drawFittedText(slider.getName(),
                     x,
                     y + knobFrameWidth - 5,
                     width - 5,
                     labelHeight,
                     juce::Justification::centred,
                     1);
}

//==============================================================================
int customLookAndFeel::getKnobHeight()
{
    return knobFrameWidth + labelHeight;
}

int customLookAndFeel::getKnobWidth()
{
    return knobFrameWidth;
}
