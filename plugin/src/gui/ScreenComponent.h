#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "FilterCurveDisplay.h"

class ScreenComponent : public juce::Component
{
public:
    ScreenComponent(AutoWah<float>& leftWah, AutoWah<float>& rightWah);

    void paint(juce::Graphics& g) override;

    void resized() override;

private:
    juce::Rectangle<float> gainLabelArea;
    juce::Rectangle<float> screenArea;
    juce::Rectangle<float> frequencyLabelArea;

    const float dBMin = -16.0f;
    const float dBMax = 16.0f;
    static constexpr int dBMarks[] = {-12, -6, 0, 6, 12};

    const float dashLen = 4.0f;
    const float gapLen = 6.0f;

    const float freqMin = 15.0f;
    const float freqMax = 21000.0f;
    static constexpr int freqMarks[] = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 15000, 20000 };
    static constexpr int freqLabels[] = { 20, 100, 1000, 10000 };

    float cornerSize = 12.0f;
    float strokeWidth = 2.0f;

    void drawScreenBoundaries(juce::Graphics& g);
    void drawScreenBackground(juce::Graphics& g);
    float dbToY(float gain, float yMin, float yMax);
    float freqToX(float freq, float xMin, float xMax);
    void drawScreenGainLines(juce::Graphics& g);
    void drawGainLabels(juce::Graphics& g);
    void drawFreqLabels(juce::Graphics& g);
    void drawFreqLines(juce::Graphics& g);

    FilterCurveDisplay curveDisplay;
};
