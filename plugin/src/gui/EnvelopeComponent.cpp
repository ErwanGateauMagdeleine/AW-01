#pragma once

#include "EnvelopeComponent.h"

EnvelopeComponent::EnvelopeComponent(juce::AudioProcessorValueTreeState& parameters, juce::Font newFont, juce::Colour newFontColour) :
    attackSlider(*parameters.getParameter("Envelope Follower Attack"), "Attack", newFont, newFontColour),
    decaySlider(*parameters.getParameter("Envelope Follower Decay"), "Decay", newFont, newFontColour),
    amountSlider(*parameters.getParameter("Envelope Follower Amount"), "Amount", newFont, newFontColour),
    attackAttachment(parameters, "Envelope Follower Attack", attackSlider),
    decayAttachment(parameters, "Envelope Follower Decay", decaySlider),
    amountAttachment(parameters, "Envelope Follower Amount", amountSlider),
    font(std::move(newFont))
{
    fontColour = newFontColour;

    for (auto* s : { &attackSlider, &decaySlider, &amountSlider })
    {
        addAndMakeVisible(*s);
    }
}

void EnvelopeComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    float cornerRadius = 10.0f;
    float lineThickness = 2.0f;

    g.setColour(fontColour);
    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    g.setFont(font);
    g.drawText("Envelope", bounds.reduced(8, 6), juce::Justification::topLeft);
}

void EnvelopeComponent::resized()
{
    int knobWidth = attackSlider.getWidth();
    int knobHeight = attackSlider.getHeight();
    const int spacing = 20;
    const int y = 30;

    attackSlider.setBounds(spacing, y, knobWidth, knobHeight);
    decaySlider.setBounds(spacing * 2 + knobWidth, y, knobWidth, knobHeight);
    amountSlider.setBounds(spacing * 3 + knobWidth * 2, y, knobWidth, knobHeight);
}
