#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      dummyKnob(*processorRef.parameters.getParameter("Dummy")),
      dummySliderAttachment(processorRef.parameters, "Dummy", dummyKnob)
{
    addAndMakeVisible(dummyKnob);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudioPluginAudioProcessorEditor::resized()
{
    /* Get the size of the custom knob */
    auto knobHeigth = dummyKnob.getHeight();
    auto knobWidth = dummyKnob.getWidth();

    /* Paint it*/
    dummyKnob.setBounds(getWidth() / 2, getHeight() / 2, knobWidth, knobHeigth);
}
