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
    editor.getFilterCompButtonRect(&buttonRect);

    float rigthOffset =  gainRect.getX() - pluginRect.getX();
    float LeftOffset = pluginRect.getRight() - screenRect.getRight();

    REQUIRE(rigthOffset == Catch::Approx(LeftOffset).margin(0.0001));

    float bottomOffset = pluginRect.getBottom() - screenRect.getBottom();
    float topOffset = screenRect.getTopLeft().getY() - buttonRect.getBottom() ;

    REQUIRE(bottomOffset == Catch::Approx(topOffset).margin(0.0001));
}
