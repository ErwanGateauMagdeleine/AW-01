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

#include <catch2/catch_all.hpp>
#include "PluginEditor.h"

TEST_CASE("Screen Component Boundaries are as expected", "[screen]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);
    juce::Rectangle<float> screenRect, gainRect,  freqRect, pluginRect, filterKnobRect, buttonRect;

    editor.getPluginRect(&pluginRect);
    editor.getScreenRects(&screenRect, &gainRect, &freqRect);
    editor.getFilterCompKnobRect(&filterKnobRect);
    buttonRect = editor.getFilterCompButtonRect();

    float rigthOffset =  gainRect.getX() - pluginRect.getX();
    float LeftOffset = pluginRect.getRight() - screenRect.getRight();

    REQUIRE(rigthOffset == Catch::Approx(LeftOffset).margin(0.0001));

    float bottomOffset = pluginRect.getBottom() - screenRect.getBottom();
    float topOffset = screenRect.getTopLeft().getY() - buttonRect.getBottom() ;

    REQUIRE(bottomOffset == Catch::Approx(topOffset).margin(0.0001));
}
