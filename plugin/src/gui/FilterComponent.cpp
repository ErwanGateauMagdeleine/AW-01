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
    auto knobsAreaBounds = getLocalBounds().reduced(20, 20).translated(0, 5);
    auto knobWidth = knobsAreaBounds.getWidth() / 3;

    freqSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    resSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    morphSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
}
