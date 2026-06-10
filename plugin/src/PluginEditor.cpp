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
    setSize (250, 455);

    filterComponent.onChange = [this](bool isPeak)
    {
        auto* param = processorRef.parameters.getParameter("filter Type");
        param->setValueNotifyingHost(isPeak ? 1.0f : 0.0f);
    };

    float val = processorRef.parameters.getRawParameterValue("filter Type")->load();
    filterComponent.setFilterType(val > 0.5f);
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
    filterComponent.setBounds(0, 105, 250, 350);
}

void AudioPluginAudioProcessorEditor::getKnobSizes(int* filterCompKnobWidth, int* filterCompKnobHeight, int* EnvelopeKnobWidth, int* EnvelopeKnobHeight)
{
    envelopeComponent.getKnobSize(EnvelopeKnobWidth, EnvelopeKnobHeight);
    filterComponent.getKnobSize(filterCompKnobWidth, filterCompKnobHeight);
}

void AudioPluginAudioProcessorEditor::getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect)
{
    filterComponent.getScreenRects(screenRect, gainRect, freqRect);
}

void AudioPluginAudioProcessorEditor::getPluginRect(juce::Rectangle<float>* plugin)
{
    *plugin = getBounds().toFloat();
}

void AudioPluginAudioProcessorEditor::getFilterCompKnobRect(juce::Rectangle<float>* filterKnob)
{
    filterComponent.getKnobRects(filterKnob);
}

void AudioPluginAudioProcessorEditor::getFilterCompButtonRect(juce::Rectangle<float>* button)
{
    filterComponent.getButtonRect(button);
}

void AudioPluginAudioProcessorEditor::getFilterButtonStates(bool* peakState, bool* bandState)
{
    filterComponent.getButtonsStates(peakState, bandState);
}

void AudioPluginAudioProcessorEditor::triggerPeakButtonClick()
{
    filterComponent.triggerPeakButtonClick();
}

void AudioPluginAudioProcessorEditor::triggerBandButtonClick()
{
    filterComponent.triggerBandButtonClick();
}
