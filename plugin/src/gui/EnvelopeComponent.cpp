#include "EnvelopeComponent.h"
#include "colourScheme.h"

EnvelopeComponent::EnvelopeComponent(juce::AudioProcessorValueTreeState& parameters) :
    attackSlider("Attack"),
    decaySlider("Decay"),
    amountSlider("Amount"),
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
    auto bounds = getLocalBounds().toFloat();

    if (auto* lnf = dynamic_cast<customLookAndFeel*> (&getLookAndFeel()))
    {
        lnf->drawComponentBoundaries(g, "Envelope", bounds, juce::Justification::topLeft);
    }
}

void EnvelopeComponent::resized()
{
    auto knobsAreaBounds = getLocalBounds().reduced(15, 15).translated(0, 10);
    knobWidth = knobsAreaBounds.getWidth() / 3;
    knobHeight = knobsAreaBounds.getHeight();

    attackSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    decaySlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    amountSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
}

void EnvelopeComponent::getKnobSize(int* width, int* height)
{
    *width = knobWidth;
    *height = knobHeight;
}
