#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ScreenComponent : public juce::Component
{
public:
    ScreenComponent();

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    void drawScreenBoundaries(juce::Graphics& g, juce::Rectangle<float> bounds);
};