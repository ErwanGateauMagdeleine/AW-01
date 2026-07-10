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

TEST_CASE("Knob Size of all components are the same", "[knob]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    int envKnobWidth;
    int envKnobHeigth;
    int filtKnobWidth;
    int filtKnobHeight;
    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);

    editor.getKnobSizes(&filtKnobWidth, &filtKnobHeight, &envKnobWidth, &envKnobHeigth);

    REQUIRE(filtKnobWidth == envKnobWidth);
    REQUIRE(filtKnobHeight == envKnobHeigth);
}
