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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/colourScheme.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      envelopeComponent(processorRef.parameters),
      filterComponent(processorRef.parameters, processorRef.getWahFilter())
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(envelopeComponent);

    addAndMakeVisible(filterComponent);

    /* Set size is the last thing to do. */
    setSize (250, 505);

    filterComponent.onChange = [this](bool isPeak)
    {
        auto* param = processorRef.parameters.getParameter("Filter Type");
        param->setValueNotifyingHost(isPeak ? 1.0f : 0.0f);
    };
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
    /* Reset the LookAndFeel to avoid dangling pointer */
    setLookAndFeel(nullptr);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(findColour(colourScheme::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    /* Draw envelope component */
    envelopeComponent.setBounds(0, 0, 250, 100);

    /* Draw filter component */
    filterComponent.setBounds(0, 105, 250, 400);
}

void AudioPluginAudioProcessorEditor::getKnobSizes(int* filterCompKnobWidth, int* filterCompKnobHeight, int* EnvelopeKnobWidth, int* EnvelopeKnobHeight)
{
    envelopeComponent.getKnobSize(EnvelopeKnobWidth, EnvelopeKnobHeight);
    filterComponent.getKnobSize(filterCompKnobWidth, filterCompKnobHeight);
}

void AudioPluginAudioProcessorEditor::getFilterButtonStates(bool* lpfState, bool* bpfState, bool* hpfState)
{
    filterComponent.getFilterButtonStates(lpfState, bpfState, hpfState);
}
