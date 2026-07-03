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
#include <catch2/catch_approx.hpp>
#include "PluginEditor.h"

TEST_CASE("Processor state saves and restores correctly", "[state]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;

    /* Set params */
    processor.parameters.getParameter("Filter Morph")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Filter Morph")->getValue() == Catch::Approx(0.8).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Attack")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Attack")->getValue() == Catch::Approx(0.8).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Decay")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Decay")->getValue() == Catch::Approx(0.8).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Amount")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Amount")->getValue() == Catch::Approx(0.8).margin(0.05f));

    processor.parameters.getParameter("Filter Center Frequency")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Filter Center Frequency")->getValue() == Catch::Approx(0.8).margin(0.05f));
    
    processor.parameters.getParameter("Filter Renonance")->setValueNotifyingHost(0.8f);
    REQUIRE(processor.parameters.getParameter("Filter Renonance")->getValue() == Catch::Approx(0.8).margin(0.05f));

    /* Save the state */
    juce::MemoryBlock state;
    processor.getStateInformation(state);
    REQUIRE(state.getSize() > 0);

    /* Change parameters to a different value */
    processor.parameters.getParameter("Filter Morph")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Filter Morph")->getValue() == Catch::Approx(0.1f).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Attack")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Attack")->getValue() == Catch::Approx(0.1).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Decay")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Decay")->getValue() == Catch::Approx(0.1).margin(0.05f));

    processor.parameters.getParameter("Envelope Follower Amount")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Envelope Follower Amount")->getValue() == Catch::Approx(0.1).margin(0.05f));

    processor.parameters.getParameter("Filter Center Frequency")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Filter Center Frequency")->getValue() == Catch::Approx(0.1).margin(0.05f));
    
    processor.parameters.getParameter("Filter Renonance")->setValueNotifyingHost(0.1f);
    REQUIRE(processor.parameters.getParameter("Filter Renonance")->getValue() == Catch::Approx(0.1).margin(0.05f));

    /* Restore state */
    processor.setStateInformation(state.getData(), static_cast<int>(state.getSize()));

    /* Check that the value is properly restored */
    REQUIRE(processor.parameters.getParameter("Filter Morph")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Envelope Follower Attack")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Envelope Follower Decay")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Envelope Follower Amount")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Filter Center Frequency")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Filter Renonance")->getValue() == Catch::Approx(0.8).margin(0.05f));
}

static void checkButtonStates(AudioPluginAudioProcessorEditor* editor, float filterType)
{
    bool peakState, bandState;

    editor->getFilterButtonStates(&peakState, &bandState);

    if (filterType > 0.5f)
    {
        REQUIRE(peakState);
        REQUIRE(!bandState);
    }
    else
    {
        REQUIRE(!peakState);
        REQUIRE(bandState);
    }
}

TEST_CASE("Editor's Button state is in line with Processor's button state", "[state]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);

    auto filterType = processor.parameters.getParameter("Filter Type")->getValue();
    checkButtonStates(&editor, filterType);

    editor.triggerPeakButtonClick();

    auto newFilterType = processor.parameters.getParameter("Filter Type")->getValue();
    REQUIRE(newFilterType != filterType);
    checkButtonStates(&editor, newFilterType);
}

TEST_CASE("Button state is saved properly", "[state]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);

    /* Trigger a click on the */
    editor.triggerPeakButtonClick();
    auto savedFilterType = processor.parameters.getParameter("Filter Type")->getValue();

    juce::MemoryBlock state;
    processor.getStateInformation(state);
    REQUIRE(state.getSize() > 0);

    editor.triggerBandButtonClick();
    REQUIRE(processor.parameters.getParameter("Filter Type")->getValue() != savedFilterType);

    processor.setStateInformation(state.getData(), static_cast<int>(state.getSize()));
    auto RestoredState = processor.parameters.getParameter("Filter Type")->getValue();
    REQUIRE(RestoredState == savedFilterType);
    checkButtonStates(&editor, RestoredState);
}
