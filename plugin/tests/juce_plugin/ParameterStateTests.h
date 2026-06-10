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

    auto filterType = processor.parameters.getParameter("filter Type")->getValue();
    checkButtonStates(&editor, filterType);

    /* Trigger a click on the */
    editor.triggerPeakButtonClick();

    auto newFilterType = processor.parameters.getParameter("filter Type")->getValue();
    REQUIRE(newFilterType != filterType);
    checkButtonStates(&editor, newFilterType);
}
