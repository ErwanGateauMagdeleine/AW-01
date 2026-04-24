#include "screenComponent.h"

ScreenComponent::ScreenComponent()
{

}

void ScreenComponent::drawScreenBoundaries(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    float cornerSize = 12.0f;
    float strokeWidth = 2.0f;
    juce::Path ScreenBoundsPath;

    ScreenBoundsPath.addRoundedRectangle(bounds, cornerSize);

    juce::PathStrokeType strokeType(strokeWidth,
                                    juce::PathStrokeType::curved,
                                    juce::PathStrokeType::rounded);
    strokeType.createStrokedPath(ScreenBoundsPath, ScreenBoundsPath);

    juce::ColourGradient gradient(
        juce::Colours::cyan,   0.0f,              bounds.getCentreY(),
        juce::Colours::purple, bounds.getWidth(), bounds.getCentreY(),
        false
    );

    g.setGradientFill(gradient);
    g.fillPath(ScreenBoundsPath);
}

void ScreenComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    drawScreenBoundaries(g, bounds);
}

void ScreenComponent::resized()
{

}