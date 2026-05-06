#pragma once

#include "customLookAndFeel.h"
#include "customRotarySlider.h"
#include "colourScheme.h"
#include "binaryData.h"

//==============================================================================
customLookAndFeel::customLookAndFeel() :
        typeface(juce::Typeface::createSystemTypefaceFor(BinaryData::OrbitronRegular_ttf,
                                                         BinaryData::OrbitronRegular_ttfSize)),
        titleFont(juce::FontOptions().withHeight(15.0f).withName("").withStyle("").withTypeface(typeface)),
        labelFont(juce::FontOptions().withHeight(10.0f).withName("").withStyle("").withTypeface(typeface)),
        screenLabelsFont(juce::FontOptions().withHeight(8.0f).withName("").withStyle("").withTypeface(typeface))
{
    setColour(colourScheme::backgroundColourId, juce::Colour::fromString("ff1a1f13"));
    setColour(colourScheme::fontColourId, juce::Colour::fromString("ffe8a020"));
    setColour(colourScheme::componentOutlineColourId, juce::Colour::fromString("ffe8a020"));
    setColour(colourScheme::sliderTrackColourId, juce::Colour::fromString("ff1e2615"));
    setColour(colourScheme::knobOutlineColourId, juce::Colour::fromString("ffe8a020"));
    setColour(colourScheme::knobBodyInsidecolourId, juce::Colour::fromString("ff2e3820"));
    setColour(colourScheme::knowBodyOutsideColourId, juce::Colour::fromString("ff141a0c"));
    setColour(colourScheme::screenBoundaryInnerColourId, juce::Colour::fromString("ff4a5c38"));
    setColour(colourScheme::screenBoundaryOuterColourId, juce::Colour::fromString("ff0a0e07"));
    setColour(colourScheme::screenBackgroundColourId, juce::Colour::fromString("ff060a04"));
    setColour(colourScheme::screenLinesColourId, juce::Colour::fromString("18e8a020"));
    setColour(colourScheme::screenLabelColourId, juce::Colour::fromString("66e8a020"));
    setColour(colourScheme::FilterCurveColourId, juce::Colour::fromString("ffe8a020"));
}

void customLookAndFeel::drawTrackArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle)
{
    juce::Path trackArk;

    trackArk.addArc(center.getX() - radius, center.getY() - radius,
                    radius * 2, radius * 2,
                    rotaryStartAngle, rotaryEndAngle, true);

    g.setColour(findColour(colourScheme::sliderTrackColourId));
    g.strokePath(trackArk, juce::PathStrokeType(4.0f));
}

void customLookAndFeel::drawValueArk(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos)
{
    float valueAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Path valueArk;
    valueArk.addArc(center.getX() - radius, center.getY() - radius,
                    radius * 2, radius * 2,
                    rotaryStartAngle, valueAngle, true);

    g.setColour(findColour(colourScheme::knobOutlineColourId).withAlpha(0.35f));
    g.strokePath(valueArk, juce::PathStrokeType(6.0f));

    g.setColour(findColour(colourScheme::knobOutlineColourId));
    g.strokePath(valueArk, juce::PathStrokeType(2.5f));
}

void customLookAndFeel::drawKnobBody(juce::Graphics& g, juce::Point<float> center, float radius)
{
    juce::ColourGradient bodyGrad(findColour(colourScheme::knobBodyInsidecolourId),
                                  center.getX(),
                                  center.getY(),
                                  findColour(colourScheme::knowBodyOutsideColourId),
                                  center.getX() - radius,
                                  center.getY() - radius,
                                  true);
    g.setGradientFill(bodyGrad);

    g.fillEllipse(center.getX() - radius * (0.75f),
                  center.getY() - radius * (0.75f),
                  radius * 1.5f,
                  radius * 1.5f);
}

void customLookAndFeel::drawPointerLine(juce::Graphics& g, juce::Point<float> center, float radius, float rotaryStartAngle, float rotaryEndAngle, float sliderPos)
{
    float valueAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Line<float> pointer(center.getX(),
                              center.getY(),
                              center.getX() + std::sin(valueAngle) * radius * 0.5f,
                              center.getY() - std::cos(valueAngle) * radius * 0.5f);
    juce::Path pointerLine;

    pointerLine.addLineSegment(pointer, 1.5f);

    g.setColour(findColour(colourScheme::knobOutlineColourId).withAlpha(0.35f));
    g.strokePath(pointerLine,juce::PathStrokeType(3.0f));

    g.setColour(findColour(colourScheme::knobOutlineColourId));
    g.strokePath(pointerLine, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void customLookAndFeel::drawLabel(juce::Graphics& g, int x, int y, int width,  int height, juce::Slider& slider)
{
    juce::Rectangle localArea(x, y, width, height);
    auto textArea = localArea.removeFromBottom(11);

    g.setFont (getLabelFont());
    g.setColour(findColour(colourScheme::fontColourId));
    drawGlowText(g, slider.getName(), textArea.toFloat(), juce::Justification::centred, getLabelFont());
}

//==============================================================================
void customLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width,  int height,
                                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                         juce::Slider& slider)
{
    juce::Point center(x + width  / 2.0f, y + height / 2.0f);
    float radius = juce::jmin(width, height) / 4.0f;

    drawTrackArk(g, center, radius, rotaryStartAngle, rotaryEndAngle);
    drawValueArk(g, center, radius, rotaryStartAngle, rotaryEndAngle, sliderPos);
    drawKnobBody(g, center, radius);
    drawPointerLine(g, center, radius, rotaryStartAngle, rotaryEndAngle, sliderPos);
    drawLabel(g, x, y, width, height, slider);
}

juce::Font customLookAndFeel::getTitleFont()
{
    return titleFont;
}

juce::Font customLookAndFeel::getLabelFont()
{
    return labelFont;
}

juce::Font customLookAndFeel::getScreenLabelsFont()
{
    return screenLabelsFont;
}

void customLookAndFeel::drawGlowText(juce::Graphics& g, const juce::String& text, juce::Rectangle<float> bounds, juce::Justification justification, juce::Font font)
{
    for (int i = 3; i >= 1; --i)
    {
        g.setColour(findColour(colourScheme::fontColourId).withAlpha(0.05f));
        g.setFont(font);

        g.drawText(text, bounds.translated((float)i,  0), justification);
        g.drawText(text, bounds.translated((float)-i,  0), justification);
        g.drawText(text, bounds.translated(0, (float)i), justification);
        g.drawText(text, bounds.translated(0, (float)-i), justification);
        g.drawText(text, bounds.translated((float)i, (float)i), justification);
        g.drawText(text, bounds.translated((float)-i, (float)-i), justification);
        g.drawText(text, bounds.translated((float)i, (float)-i), justification);
        g.drawText(text, bounds.translated((float)-i, (float)i), justification);
    }

    g.setColour(findColour(colourScheme::fontColourId));
    g.drawText(text, bounds, justification);
}

void customLookAndFeel::drawComponentBoundaries(juce::Graphics& g, const juce::String& text, juce::Rectangle<float> bounds, juce::Justification justification)
{
    juce::Path borderPath;

    borderPath.addRoundedRectangle(bounds, compBorderCornerRadius, compBorderCornerRadius);
    g.setColour(findColour(colourScheme::componentOutlineColourId).withAlpha(0.35f));
    g.strokePath(borderPath, juce::PathStrokeType(6.0f));

    g.setColour(findColour(colourScheme::componentOutlineColourId));
    g.strokePath(borderPath, juce::PathStrokeType(1.0f));

    drawGlowText(g, text, bounds.reduced(8, 6), justification, getTitleFont());
}
