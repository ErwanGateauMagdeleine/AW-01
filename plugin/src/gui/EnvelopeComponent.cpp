#pragma once

#include "EnvelopeComponent.h"
#include "colourScheme.h"

EnvelopeComponent::EnvelopeComponent(juce::AudioProcessorValueTreeState& parameters) :
    attackSlider(*parameters.getParameter("Envelope Follower Attack"), "Attack"),
    decaySlider(*parameters.getParameter("Envelope Follower Decay"), "Decay"),
    amountSlider(*parameters.getParameter("Envelope Follower Amount"), "Amount"),
    attackAttachment(parameters, "Envelope Follower Attack", attackSlider),
    decayAttachment(parameters, "Envelope Follower Decay", decaySlider),
    amountAttachment(parameters, "Envelope Follower Amount", amountSlider)
{
    for (auto* s : { &attackSlider, &decaySlider, &amountSlider })
    {
        addAndMakeVisible(*s);
    }
}

void EnvelopeComponent::paint(juce::Graphics& g)
{
    float cornerRadius = 10.0f;
    float lineThickness = 2.0f;

    g.setColour(findColour(colourScheme::componentOutlineColourId));
    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    if (auto* lnf = dynamic_cast<customLookAndFeel*> (&getLookAndFeel()))
    {
        g.setFont (lnf->getTitleFont());
    }

    g.setColour(findColour(colourScheme::fontColourId));
    g.drawText("Envelope", bounds.reduced(8, 6), juce::Justification::topLeft);
}

void EnvelopeComponent::resized()
{
    auto knobsAreaBounds = getLocalBounds().reduced(20, 20).translated(0, 5);
    auto knobWidth = knobsAreaBounds.getWidth() / 3;

    attackSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    decaySlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    amountSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
}
