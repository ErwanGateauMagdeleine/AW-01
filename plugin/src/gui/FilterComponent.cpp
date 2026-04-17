#pragma once

#include "FilterComponent.h"
#include "colourScheme.h"

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
    float cornerRadius = 10.0f;
    float lineThickness = 2.0f;

    g.setColour(findColour(colourScheme::componentOutlineColourId));
    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    g.setColour(findColour(colourScheme::fontColourId));
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
