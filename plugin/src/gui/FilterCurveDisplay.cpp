#include "FilterCurveDisplay.h"
#include "colourScheme.h"
#include "customLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

FilterCurveDisplay::FilterCurveDisplay(AutoWah<float>& initLeftWah,
                                       AutoWah<float>& initRightWah,
                                       float nDbMin,
                                       float nDbMax,
                                       float nFreqMin,
                                       float nFreqMax,
                                       float cSize)
    : leftWah(initLeftWah),
      rightWah(initRightWah)
{
    dbMin = nDbMin;
    dbMax = nDbMax;
    freqMin = nFreqMin;
    freqMax = nFreqMax;
    cornerSize = cSize;

    startTimerHz(30);
}

void FilterCurveDisplay::timerCallback()
{
    repaint();
}

void FilterCurveDisplay::paint(juce::Graphics& g)
{
    auto width = getWidth();
    juce::Path curve;
    juce::Path clipPath;

    clipPath.addRoundedRectangle(getLocalBounds(), cornerSize, cornerSize);
    g.reduceClipRegion(clipPath);

    for (int x = 0; x < width; x++)
    {
        float normalized = static_cast<float>(x) / static_cast<float>(width);
        float freq = freqMin * std::pow(freqMax / freqMin, normalized);

        float magnitude = leftWah.getMagnitudeFromFrequency(freq);

        float dB = 20.0f * std::log10(std::max(magnitude, 1e-6f));

        float y = juce::jmap(dB, dbMin, dbMax, static_cast<float>(getY() + getHeight()), static_cast<float>(getY()));

        if (x == 0)
        {
            curve.startNewSubPath(static_cast<float>(x), y);
        }
        else
        {
            curve.lineTo(static_cast<float>(x), y);
        }

        g.setColour(findColour(colourScheme::FilterCurveColourId));
        g.strokePath(curve, juce::PathStrokeType(1.0f));
    }
}
