#include "screenComponent.h"

ScreenComponent::ScreenComponent()
{

}

void ScreenComponent::drawScreenBoundaries(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    juce::Path ScreenBoundsPath;

    g.setColour(findColour(colourScheme::screenBoundaryOuterColour));
    g.drawRoundedRectangle(bounds.expanded(1), cornerSize, strokeWidth);

    g.setColour(findColour(colourScheme::screenBoundaryInnerColour));
    g.drawRoundedRectangle(bounds, cornerSize, strokeWidth);
}

void ScreenComponent::drawScreenBackground(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    float cx = getWidth() / 2.0f;
    float cy = getHeight() / 2.0f;
    float radius = std::sqrt(cx * cx + cy * cy);

    g.setColour(findColour(colourScheme::screenBackgroundColour));
    g.fillRoundedRectangle(bounds, cornerSize);

    juce::ColourGradient warmGlow(juce::Colour(0x991e280a), cx, cy,
                                  juce::Colour(0x00000000), cx, cy + radius,
                                  true);
    g.setGradientFill(warmGlow);
    g.fillRoundedRectangle(bounds, cornerSize);

    juce::ColourGradient vignette(juce::Colour(0x00000000), cx, cy,
                                  juce::Colour(0x8c000000), cx, cy + radius,
                                  true);
    g.setGradientFill(vignette);
    g.fillRoundedRectangle(bounds, cornerSize);


}

void ScreenComponent::paint(juce::Graphics& g)
{
    drawScreenBoundaries(g);
    drawScreenBackground(g);
}

void ScreenComponent::resized()
{

}