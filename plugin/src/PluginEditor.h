#pragma once

#include "PluginProcessor.h"
#include "gui/customRotarySlider.h"
#include "gui/FilterComponent.h"
#include "gui/EnvelopeComponent.h"
#include "gui/customLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void getKnobSizes(int* filterCompKnobWidth, int* filterCompKnobHeight, int* EnvelopeKnobWidth, int* EnvelopeKnobHeight);

    void getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect);

    void getPluginRect(juce::Rectangle<float>* plugin);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    //==============================================================================
    customLookAndFeel lnf;

    using APVTS = juce::AudioProcessorValueTreeState;
    using attachment = APVTS::SliderAttachment;

    EnvelopeComponent envelopeComponent;

    FilterComponent filterComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
