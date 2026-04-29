#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ScreenComponent : public juce::Component
{
public:
    ScreenComponent();

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    juce::Rectangle<float> gainLabelArea;
    juce::Rectangle<float> screenArea;

    const float dBMin = -30.0f;
    const float dBMax = 30.0f;
    static constexpr int dBMarks[] = {-24, -18, -12, -6, 0, 6, 12, 18, 24};
    const float dashLen = 4.0f;
    const float gapLen = 6.0f;

    float cornerSize = 12.0f;
    float strokeWidth = 2.0f;

    void drawScreenBoundaries(juce::Graphics& g);
    void drawScreenBackground(juce::Graphics& g);
    float dbToY(float gain, float yMin, float yMax);
    void drawScreenGainLines(juce::Graphics& g);
    void drawGainLabels(juce::Graphics& g);
};
