/* AW-01 - AutoWah JUCE based audio plugin
   Copyright (C) 2026 Erwan Gateau-Magdeleine

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

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

    void getFilterCompKnobRect(juce::Rectangle<float>* plugin);

    juce::Rectangle<float> getFilterCompButtonRect(void);

    void getFilterButtonStates(bool* peakState, bool* bandState);

    void triggerPeakButtonClick();

    void triggerBandButtonClick();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    //==============================================================================
    customLookAndFeel lnf;

    EnvelopeComponent envelopeComponent;

    FilterComponent filterComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
