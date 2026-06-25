#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class customLookAndFeel : public juce::LookAndFeel_V4
{
public:
    customLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width,  int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour&, bool, bool) override;
    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool, bool) override;

    juce::Font getTitleFont();
    juce::Font getLabelsFont();
    juce::Font getScreenLabelsFont();

    void drawGlowText(juce::Graphics& g,
                      const juce::String& text,
                      juce::Rectangle<float> bounds,
                      juce::Justification justification,
                      juce::Font font,
                      juce::Colour colour);

    void drawComponentBoundaries(juce::Graphics& g, const juce::String& text, juce::Rectangle<float> bounds, juce::Justification justification);

private:

    void drawTrackArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle);
    void drawValueArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos);
    void drawKnobBody(juce::Graphics& g, juce::Point<float> center, float radius);
    void drawLabelValue(juce::Graphics& g, int x, int y, int width,  int height, juce::Slider& slider);
    void drawPointerLine(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos);

    float compBorderCornerRadius = 10.0f;

    juce::Typeface::Ptr typeface;
    juce::Font titleFont;
    juce::Font labelFont;
    juce::Font screenLabelsFont;
};
