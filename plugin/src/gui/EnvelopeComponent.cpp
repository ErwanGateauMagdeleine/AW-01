#pragma once

#include "EnvelopeComponent.h"

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
    g.setColour(juce::Colours::black);
    float cornerRadius = 10.0f;
    float lineThickness = 2.0f;

    auto bounds = getLocalBounds().toFloat().reduced(lineThickness / 2.0f);
    g.drawRoundedRectangle(bounds, cornerRadius, lineThickness);

    juce::FontOptions options("Futura", 15.0f, juce::Font::bold);
    juce::Font customFont(options);

    g.setFont(customFont);
    g.drawText("Envelope", getLocalBounds().removeFromTop(20), juce::Justification::centred);
}

void EnvelopeComponent::resized()
{
    int knobWidth = attackSlider.getWidth();
    int knobHeight = attackSlider.getHeight();
    const int spacing = 20;
    const int y = 25;

    attackSlider.setBounds(spacing, y, knobWidth, knobHeight);
    decaySlider.setBounds(spacing * 2 + knobWidth, y, knobWidth, knobHeight);
    amountSlider.setBounds(spacing * 3 + knobWidth * 2, y, knobWidth, knobHeight);
}