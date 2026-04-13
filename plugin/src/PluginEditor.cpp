#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      envelopeComponent(processorRef.parameters),
      filterComponent(processorRef.parameters)
{
    addAndMakeVisible(envelopeComponent);

    addAndMakeVisible(filterComponent);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (505, 100);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromString(colourString));
}

void AudioPluginAudioProcessorEditor::resized()
{
    /* Draw envelope component */
    envelopeComponent.setBounds(0, 0, 250, 100);

    /* Draw filter component */
    filterComponent.setBounds(255, 0, 250, 100);
}
