#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "gui/colourScheme.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      envelopeComponent(processorRef.parameters),
      filterComponent(processorRef.parameters)
{
    setLookAndFeel(&lnf);

    addAndMakeVisible(envelopeComponent);

    addAndMakeVisible(filterComponent);

    /* Set size is the last thing to do. */
    setSize (505, 100);
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
    filterComponent.setBounds(255, 0, 250, 100);
}
