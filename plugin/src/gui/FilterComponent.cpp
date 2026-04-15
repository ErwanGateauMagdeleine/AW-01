#pragma once

#include "FilterComponent.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& parameters, juce::Font newFont, juce::Colour newFontColour) :
    freqSlider(*parameters.getParameter("Filter Center Frequency"), "Cutoff", newFont, newFontColour),
    resSlider(*parameters.getParameter("Filter Renonance"), "Res", newFont, newFontColour),
    morphSlider(*parameters.getParameter("Filter Morph"), "Morph", newFont, newFontColour),
    freqAttachment(parameters, "Filter Center Frequency", freqSlider),
    resAttachment(parameters, "Filter Renonance", resSlider),
    morphAttachment(parameters, "Filter Morph", morphSlider),
    font(std::move(newFont))
{
    fontColour = newFontColour;

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

    g.setColour(fontColour);
    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    g.setFont(font);
    g.drawText("Filter", bounds.reduced(8,6), juce::Justification::topLeft);
}

void FilterComponent::resized()
{
    int knobWidth = freqSlider.getWidth();
    int knobHeight = freqSlider.getHeight();
    const int spacing = 20;
    const int y = 30;

    freqSlider.setBounds(spacing, y, knobWidth, knobHeight);
    resSlider.setBounds(spacing * 2 + knobWidth, y, knobWidth, knobHeight);
    morphSlider.setBounds(spacing * 3 + knobWidth * 2, y, knobWidth, knobHeight);
}