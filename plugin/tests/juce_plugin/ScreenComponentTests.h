#include <catch2/catch_all.hpp>
#include "PluginEditor.h"

TEST_CASE("Screen Component Boundaries are as expected", "[screen]")
{
    juce::ScopedJuceInitialiser_GUI juceInit;

    AudioPluginAudioProcessor processor;
    AudioPluginAudioProcessorEditor editor(processor);
    juce::Rectangle<float> screenRect, gainRect,  freqRect, pluginRect;

    editor.getPluginRect(&pluginRect);

    editor.getScreenRects(&screenRect, &gainRect, &freqRect);

    float rigthOffset =  gainRect.getX() - pluginRect.getX();
    float LeftOffset = pluginRect.getRight() - screenRect.getRight();

    REQUIRE(rigthOffset == Catch::Approx(LeftOffset).margin(0.0001));
}
