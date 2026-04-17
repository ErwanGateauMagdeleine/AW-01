#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class customLookAndFeel : public juce::LookAndFeel_V4
{
public:
    customLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width,  int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider) override;

    juce::Font getTitleFont();
    juce::Font getLabelFont();

    void drawGlowText (juce::Graphics& g, const juce::String& text, juce::Rectangle<float> bounds, juce::Justification justification, juce::Font font);

    void drawComponentBoundaries(juce::Graphics& g, const juce::String& text, juce::Rectangle<float> bounds, juce::Justification justification);

private:

    void drawTrackArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle);
    void drawValueArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos);
    void drawKnobBody(juce::Graphics& g, juce::Point<float> center, float radius);
    void drawLabel(juce::Graphics& g, int x, int y, int width,  int height, juce::Slider& slider);
    void drawPointerLine(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos);

    float compBorderCornerRadius = 10.0f;

    juce::Typeface::Ptr typeface;
    juce::Font titleFont;
    juce::Font labelFont;
};
