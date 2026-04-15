#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      fontOptions("Orbitron", 15.0f, juce::Font::bold),
      font(fontOptions),
      envelopeComponent(processorRef.parameters, font, juce::Colour::fromString(fontColourString)),
      filterComponent(processorRef.parameters, font, juce::Colour::fromString(fontColourString))
{
    addAndMakeVisible(envelopeComponent);

    addAndMakeVisible(filterComponent);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (505, 105);
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
    envelopeComponent.setBounds(0, 0, 250, 105);

    /* Draw filter component */
    filterComponent.setBounds(255, 0, 250, 105);
}
