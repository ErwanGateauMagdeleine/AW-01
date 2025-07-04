#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
      envFollowerAttackSlider(*processorRef.parameters.getParameter("Envelope Follower Attack"), "Attack"),
      envFollowerDecaySlider(*processorRef.parameters.getParameter("Envelope Follower Decay"), "Decay"),
      envAmountSlider(*processorRef.parameters.getParameter("Envelope Follower Amount"), "Amount"),
      FilterCenterFreqSlider(*processorRef.parameters.getParameter("Filter Center Frequency"), "Cutoff"),
      FilterResonanceSlider(*processorRef.parameters.getParameter("Filter Renonance"), "Res"),
      FilterMorphSlider(*processorRef.parameters.getParameter("Filter Morph"), "Morph"),
      envFollowerAttackSliderAttachment(processorRef.parameters, "Envelope Follower Attack", envFollowerAttackSlider),
      envFollowerDecaySliderAttachment(processorRef.parameters, "Envelope Follower Decay", envFollowerDecaySlider),
      envAmountSliderAttachment(processorRef.parameters, "Envelope Follower Amount", envAmountSlider),
      FilterCenterFreqSliderAttachment(processorRef.parameters, "Filter Center Frequency", FilterCenterFreqSlider),
      FilterResonanceSliderAttachment(processorRef.parameters, "Filter Renonance", FilterResonanceSlider),
      FilterMorphSliderAttachment(processorRef.parameters, "Filter Morph", FilterMorphSlider)
{
    /* Load Background image */
    textureImage = juce::ImageCache::getFromMemory(BinaryData::Metal009_1KPNG_Color_png,
                                                   BinaryData::Metal009_1KPNG_Color_pngSize);

    addAndMakeVisible(envFollowerAttackSlider);
    addAndMakeVisible(envFollowerDecaySlider);
    addAndMakeVisible(envAmountSlider);
    addAndMakeVisible(FilterCenterFreqSlider);
    addAndMakeVisible(FilterResonanceSlider);
    addAndMakeVisible(FilterMorphSlider);

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
    /* Get the size of the custom knob */
    auto knobHeigth = envFollowerAttackSlider.getHeight();
    auto knobWidth = envFollowerAttackSlider.getWidth();

    // === Layout constants ===
    const int knobSpacing = 20;
    const int startX = knobSpacing;
    const int startY = 20;

    // All knobs in a list for cleaner layout
    juce::Slider* knobs[] = {
        &envFollowerAttackSlider,
        &envFollowerDecaySlider,
        &envAmountSlider,
        &FilterCenterFreqSlider,
        &FilterResonanceSlider,
        &FilterMorphSlider
    };

for (int i = 0; i < std::size(knobs); ++i)
{
    int x = startX + i * (knobWidth + knobSpacing);
    knobs[i]->setBounds(x, startY, knobWidth, knobHeigth);
}

}
