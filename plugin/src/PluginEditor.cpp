#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      dummyKnob(*processorRef.parameters.getParameter("Dummy"), "dummy knob"),
      dummySliderAttachment(processorRef.parameters, "Dummy", dummyKnob)
{
    /* Load Background image */
    textureImage = juce::ImageCache::getFromMemory(BinaryData::Metal009_1KPNG_Color_png,
                                                   BinaryData::Metal009_1KPNG_Color_pngSize);

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
    /* Tile the background  texture */
    if (textureImage.isValid())
    {
        for (int y = 0; y < getHeight(); y += textureImage.getHeight())
        {
            for (int x = 0; x < getWidth(); x += textureImage.getWidth())
            {
                g.drawImageAt(textureImage, x, y);
            }
        }
    }
}

void AudioPluginAudioProcessorEditor::resized()
{
    /* Get the size of the custom knob */
    auto knobHeigth = dummyKnob.getHeight();
    auto knobWidth = dummyKnob.getWidth();

    /* Paint it*/
    dummyKnob.setBounds(getWidth() / 2, getHeight() / 2, knobWidth, knobHeigth);
}
