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
    auto bounds = getLocalBounds().toFloat();

    if (auto* lnf = dynamic_cast<customLookAndFeel*> (&getLookAndFeel()))
    {
        lnf->drawComponentBoundaries(g, "Filter", bounds, juce::Justification::topLeft);
    }
}

void FilterComponent::resized()
{
    auto knobsAreaBounds = getLocalBounds().reduced(20, 20).translated(0, 5);
    auto knobWidth = knobsAreaBounds.getWidth() / 3;

    freqSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    resSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    morphSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
}
