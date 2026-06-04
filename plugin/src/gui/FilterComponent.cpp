#pragma once

#include "FilterComponent.h"
#include "colourScheme.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& parameters, AutoWah<float>& leftWah, AutoWah<float>& rightWah) :
    freqSlider(*parameters.getParameter("Filter Center Frequency"), "Cutoff"),
    resSlider(*parameters.getParameter("Filter Renonance"), "Res"),
    morphSlider(*parameters.getParameter("Filter Morph"), "Morph"),
    freqAttachment(parameters, "Filter Center Frequency", freqSlider),
    resAttachment(parameters, "Filter Renonance", resSlider),
    morphAttachment(parameters, "Filter Morph", morphSlider),
    screen(leftWah, rightWah)
{
    for (auto* s : { &freqSlider, &resSlider, &morphSlider })
    {
        addAndMakeVisible(*s);
    }

    addAndMakeVisible(screen);
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
    auto bounds = getLocalBounds();
    auto knobsAreaBounds = bounds.removeFromTop((int)(bounds.getHeight() / 3.0f)).reduced(20, 20).translated(0, 10);

    bounds = bounds.reduced(20, 20);

    knobWidth = knobsAreaBounds.getWidth() / 3;
    knobHeight = knobsAreaBounds.getHeight();

    freqSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    resSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    morphSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));

    screen.setBounds(bounds);
}

void FilterComponent::getKnobSize(int* width, int* height)
{
    *width = knobWidth;
    *height = knobHeight;
}

void FilterComponent::getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect)
{
    screen.getScreenRects(screenRect, gainRect, freqRect);
}

void FilterComponent::getKnobRects(juce::Rectangle<float>* filterKnob)
{
    auto bounds = getLocalBounds();
    *filterKnob = localAreaToGlobal(bounds.removeFromTop((int)(bounds.getHeight() / 3.0f)).reduced(20, 20).translated(0, 10)).toFloat();
}
