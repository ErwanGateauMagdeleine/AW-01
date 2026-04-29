#include "screenComponent.h"
#include "colourScheme.h"
#include "customLookAndFeel.h"

ScreenComponent::ScreenComponent()
{
}

float ScreenComponent::dbToY(float gain, float yMin, float yMax)
{
    return juce::jmap(gain, dBMin, dBMax, yMax, yMin);
}

void ScreenComponent::drawScreenBoundaries(juce::Graphics& g)
{
    juce::Path ScreenBoundsPath;

    g.setColour(findColour(colourScheme::screenBoundaryOuterColourId));
    g.drawRoundedRectangle(screenArea.expanded(1), cornerSize, strokeWidth);

    g.setColour(findColour(colourScheme::screenBoundaryInnerColourId));
    g.drawRoundedRectangle(screenArea, cornerSize, strokeWidth);
}

void ScreenComponent::drawScreenBackground(juce::Graphics& g)
{
    float cx = getWidth() / 2.0f;
    float cy = getHeight() / 2.0f;
    float radius = std::sqrt(cx * cx + cy * cy);

    g.setColour(findColour(colourScheme::screenBackgroundColourId));
    g.fillRoundedRectangle(screenArea, cornerSize);

    juce::ColourGradient warmGlow(juce::Colour(0x991e280a), cx, cy,
                                  juce::Colour(0x00000000), cx, cy + radius,
                                  true);
    g.setGradientFill(warmGlow);
    g.fillRoundedRectangle(screenArea, cornerSize);

    juce::ColourGradient vignette(juce::Colour(0x00000000), cx, cy,
                                  juce::Colour(0x8c000000), cx, cy + radius,
                                  true);
    g.setGradientFill(vignette);
    g.fillRoundedRectangle(screenArea, cornerSize);
}

void ScreenComponent::drawScreenGainLines(juce::Graphics& g)
{
    g.setColour(findColour(colourScheme::screenLinesColourId));

    for (int mark : dBMarks)
    {
        juce::Path dashedLine;
        float xPos = screenArea.getX();
        float w = xPos + screenArea.getWidth();

        float y = dbToY((float)mark, screenArea.getY(), screenArea.getY() + screenArea.getHeight());

        while(xPos < w)
        {
            dashedLine.startNewSubPath(xPos, y);
            dashedLine.lineTo(juce::jmin(xPos + dashLen, w), y);
            xPos += dashLen + gapLen;
        }

        g.strokePath(dashedLine, juce::PathStrokeType(1.0f));
    }
}

void ScreenComponent::drawGainLabels(juce::Graphics& g)
{
    for (int mark : dBMarks)
    {
        float y = dbToY((float)mark, screenArea.getY(), screenArea.getY() + screenArea.getHeight());
        juce::String label = (mark > 0 ? "+" : "") + juce::String(mark);

        if (auto* lnf = dynamic_cast<customLookAndFeel*> (&getLookAndFeel()))
        {
            lnf->drawGlowText(g, label,
                              juce::Rectangle<float>(gainLabelArea.getX(), y - 6.0f, gainLabelArea.getWidth(), 12.0f),
                              juce::Justification::centred,
                              lnf->getScreenLabelsFont());
        }
    }
}

void ScreenComponent::paint(juce::Graphics& g)
{
    drawScreenBackground(g);
    drawScreenGainLines(g);
    drawScreenBoundaries(g);
    drawGainLabels(g);
}

void ScreenComponent::resized()
{
    auto bounds = getLocalBounds().toFloat();

    gainLabelArea = bounds.removeFromLeft(24.0f);
    screenArea = bounds;
}
