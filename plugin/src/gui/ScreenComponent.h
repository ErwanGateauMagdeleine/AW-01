#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ScreenComponent : public juce::Component
{
public:
    ScreenComponent();

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    float cornerSize = 12.0f;
    float strokeWidth = 2.0f;

    void drawScreenBoundaries(juce::Graphics& g);
    void drawScreenBackground(juce::Graphics& g);
