#pragma once

#include "FilterComponent.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& parameters) :
    freqSlider(*parameters.getParameter("Filter Center Frequency"), "Cutoff"),
    resSlider(*parameters.getParameter("Filter Renonance"), "Res"),
    morphSlider(*parameters.getParameter("Filter Morph"), "Morph"),
    freqAttachment(parameters, "Filter Center Frequency", freqSlider),
    resAttachment(parameters, "Filter Renonance", resSlider),
    morphAttachment(parameters, "Filter Morph", morphSlider)
{
    for (auto* s : { &freqSlider, &resSlider, &morphSlider })
    {
        addAndMakeVisible(*s);
    }
}

void FilterComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    float cornerRadius = 10.0f;
    float lineThickness = 2.0f;

    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    juce::FontOptions options("Futura", 15.0f, juce::Font::bold);
    juce::Font customFont(options);

    g.setFont(customFont);
    g.drawText("Filter", getLocalBounds().removeFromTop(20), juce::Justification::centred);
}

void FilterComponent::resized()
{
    int knobWidth = freqSlider.getWidth();
    int knobHeight = freqSlider.getHeight();
    const int spacing = 20;
    const int y = 25;

    freqSlider.setBounds(spacing, y, knobWidth, knobHeight);
    resSlider.setBounds(spacing * 2 + knobWidth, y, knobWidth, knobHeight);
    morphSlider.setBounds(spacing * 3 + knobWidth * 2, y, knobWidth, knobHeight);
}