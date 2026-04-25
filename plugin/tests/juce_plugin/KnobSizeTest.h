#include <catch2/catch_all.hpp>
#include "PluginEditor.h"

TEST_CASE("Knob Size of all components are the same", "[knob]")
{
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
