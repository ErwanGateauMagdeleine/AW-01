#include "customLookAndFeel.h"
#include "customRotarySlider.h"
#include "colourScheme.h"
#include "BinaryData.h"

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
    setColour(colourScheme::buttonInactiveFillCoulourId, juce::Colour::fromString("0xff001a0e"));
    setColour(colourScheme::buttonActiveFillColourId, juce::Colour::fromString("ff1a1f13"));
    setColour(colourScheme::ledInactiveFillColourId, juce::Colour::fromString("ffa0711e"));
    setColour(colourScheme::ledActiveFillColourId, juce::Colour::fromString("ffe8a020"));
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

void customLookAndFeel::drawLabelValue(juce::Graphics& g, int x, int y, int width,  int height, juce::Slider& slider)
{
    juce::Rectangle localArea(x, y, width, height);
    auto textArea = localArea.removeFromBottom(11);

    g.setFont (getLabelsFont());
    g.setColour(findColour(colourScheme::fontColourId));
    drawGlowText(g,
                 slider.getName(),
                 textArea.toFloat(),
                 juce::Justification::centred,
                 getLabelsFont(),
                 findColour(colourScheme::fontColourId));
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
    drawLabelValue(g, x, y, width, height, slider);
}

juce::Font customLookAndFeel::getTitleFont()
{
    return titleFont;
}

juce::Font customLookAndFeel::getLabelsFont()
{
    return labelFont;
}

juce::Font customLookAndFeel::getScreenLabelsFont()
{
    return screenLabelsFont;
}

void customLookAndFeel::drawGlowText(juce::Graphics& g,
                                     const juce::String& text,
                                     juce::Rectangle<float> bounds,
                                     juce::Justification justification,
                                     juce::Font font,
                                     juce::Colour colour)
{
    for (int i = 3; i >= 1; --i)
    {
        g.setColour(colour.withAlpha(0.05f));
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

    g.setColour(colour);
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

    drawGlowText(g,
                 text,
                 bounds.reduced(8, 6),
                 justification,
                 getTitleFont(),
                 findColour(colourScheme::fontColourId));
}

void customLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour&, bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat();
    bool active = button.getToggleState();
    juce::Colour backgroundColor;

    if (active)
    {
        backgroundColor = findColour(colourScheme::buttonActiveFillColourId);
    }
    else
    {
        backgroundColor = findColour(colourScheme::buttonInactiveFillCoulourId);
    }

    g.setColour(backgroundColor);
    g.fillRoundedRectangle(bounds.reduced(1), compBorderCornerRadius);

    g.setColour(findColour(colourScheme::screenBoundaryInnerColourId));
    g.drawRoundedRectangle(bounds.reduced(1), compBorderCornerRadius, 1.0);

    g.setColour(findColour(colourScheme::screenBoundaryOuterColourId));
    g.drawRoundedRectangle(bounds, compBorderCornerRadius, 1.0);

    float ledRadius = 2.5f;
    float ledX = bounds.getX() + 10.0f;
    float ledY = bounds.getCentreY();

    if (active)
    {
        for (int i = 0; i < 3; i++)
        {
            g.setColour(findColour(colourScheme::buttonActiveFillColourId).withAlpha(0.05f));
            g.fillEllipse(ledX - ledRadius * i, ledY - ledRadius * i, 2.0f * ledRadius * i, 2.0f * ledRadius * i);
        }
        g.setColour(findColour(colourScheme::ledActiveFillColourId));
    }
    else
    {
        g.setColour(findColour(colourScheme::ledInactiveFillColourId));
    }
    g.fillEllipse(ledX - ledRadius, ledY - ledRadius, 2.0f * ledRadius, 2.0f * ledRadius);
}

void customLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool, bool)
{
    bool active = button.getToggleState();
    auto bounds = button.getLocalBounds();
    bounds.removeFromLeft(20);
    juce::Colour colour;

    if (active)
    {
        drawGlowText(g,
                     button.getName(),
                     bounds.toFloat(),
                     juce::Justification::centredLeft,
                     getLabelFont(),
                     findColour(colourScheme::fontColourId));
    }
    else
    {
        g.setFont(getLabelFont());
        g.setColour(findColour(colourScheme::ledInactiveFillColourId));
        g.drawText(button.getName(), bounds, juce::Justification::centredLeft);
    }
}
