/* AW-01 - AutoWah JUCE based audio plugin
   Copyright (C) 2026 Erwan Gateau-Magdeleine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "FilterCurveDisplay.h"
#include "colourScheme.h"
#include "customLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

FilterCurveDisplay::FilterCurveDisplay(AutoWah<float>& initWah,
                                       float nDbMin,
                                       float nDbMax,
                                       float nFreqMin,
                                       float nFreqMax,
                                       float cSize)
    : wah(initWah)
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

        float magnitude = wah.getMagnitudeFromFrequency(freq);

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
