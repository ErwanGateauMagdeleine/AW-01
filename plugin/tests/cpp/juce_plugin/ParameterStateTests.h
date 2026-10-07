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
    REQUIRE(processor.parameters.getParameter("Envelope Follower Attack")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Envelope Follower Decay")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Envelope Follower Amount")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Filter Center Frequency")->getValue() == Catch::Approx(0.8).margin(0.05f));
    REQUIRE(processor.parameters.getParameter("Filter Renonance")->getValue() == Catch::Approx(0.8).margin(0.05f));
}

TEST_CASE("Filter Selection Buttons are mutually exclusive at startup", "[params]")
{
    bool lpfState, bpfState, hpfState;

    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);

    editor.getFilterButtonStates(&lpfState, &bpfState, &hpfState);

    if (lpfState)
    {
        REQUIRE(bpfState != lpfState);
        REQUIRE(hpfState != lpfState);
    }
    if (bpfState)
    {
        REQUIRE(lpfState != bpfState);
        REQUIRE(hpfState != bpfState);
    }
    if (hpfState)
    {
        REQUIRE(bpfState != hpfState);
        REQUIRE(lpfState != hpfState);
    }

    /* At least one needs to be set */
    unsigned state = (unsigned)lpfState + (unsigned)bpfState + (unsigned)hpfState;
    REQUIRE(state == 1);
}

TEST_CASE("Button state is mutually exclusive at runtime", "[params]")
{
    bool lpfState, bpfState, hpfState;

    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);

    editor.lpfButtonTriggerClick();
    editor.getFilterButtonStates(&lpfState, &bpfState, &hpfState);

    REQUIRE(lpfState == true);
    REQUIRE(bpfState == false);
    REQUIRE(hpfState == false);

    editor.hpfButtonTriggerClick();
    editor.getFilterButtonStates(&lpfState, &bpfState, &hpfState);

    REQUIRE(hpfState == true);
    REQUIRE(lpfState == false);
    REQUIRE(bpfState == false);

    editor.bpfButtonTriggerClick();
    editor.getFilterButtonStates(&lpfState, &bpfState, &hpfState);

    REQUIRE(bpfState == true);
    REQUIRE(lpfState == false);
    REQUIRE(hpfState == false);
}
