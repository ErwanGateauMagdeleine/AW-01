#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      envelopeComponent(processorRef.parameters),
      filterComponent(processorRef.parameters)
{
    /* Load Background image */
    textureImage = juce::ImageCache::getFromMemory(BinaryData::Metal009_1KPNG_Color_png,
                                                   BinaryData::Metal009_1KPNG_Color_pngSize);

    addAndMakeVisible(envelopeComponent);

    addAndMakeVisible(filterComponent);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 100);
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
    /* Draw envelope component */
    envelopeComponent.setBounds(0, 0, 250, 100);

    /* Draw filter component */
    filterComponent.setBounds(250, 0, 250, 100);
}
