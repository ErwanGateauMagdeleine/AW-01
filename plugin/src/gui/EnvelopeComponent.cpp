/* AW-01 - AutoWah JUCE based audio plugin
   Copyright (C) 2026 Erwan Gateau-Magdeleine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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
