#pragma once

#include "PluginProcessor.h"
#include "gui/customRotarySlider.h"
#include "gui/FilterComponent.h"
#include "gui/EnvelopeComponent.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    //==============================================================================

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    EnvelopeComponent envelopeComponent;

    FilterComponent filterComponent;

    const juce::String colourString = "ff1a1f13";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
