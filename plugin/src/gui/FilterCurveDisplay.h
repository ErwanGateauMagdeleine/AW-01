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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../dsp/AutoWah.h"

class FilterCurveDisplay : public juce::Component,
                           public juce::Timer
{
public:
    FilterCurveDisplay(AutoWah<float>& initWah,
                       float dbMin,
                       float dbMax,
                       float freqMin,
                       float freqMax,
                       float cSize);

    void timerCallback() override;
    void paint(juce::Graphics& g) override;
private:
    AutoWah<float>& wah;

    float dbMin, dbMax, freqMin, freqMax;

    float cornerSize = 12.0f;
};
