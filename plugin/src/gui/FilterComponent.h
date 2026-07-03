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

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "customRotarySlider.h"
#include "ScreenComponent.h"
#include "FilterTypeSelector.h"

class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& state, AutoWah<float>& wah);

    void paint(juce::Graphics& g) override;

    void resized() override;

    void getKnobSize(int* width, int* height);

    void getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect);

    void getKnobRects(juce::Rectangle<float>* filterKnob);

    juce::Rectangle<float> getButtonRect();

    std::function<void(bool)> onChange;

    void setFilterType(bool isPeak);

    void getButtonsStates(bool* peakState, bool* bandState);

    void triggerPeakButtonClick();

    void triggerBandButtonClick();

private:
    CustomRotarySlider freqSlider, resSlider, morphSlider, gainSlider;

    FilterTypeSelector filterSelector;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;

    SliderAttachment freqAttachment, resAttachment, morphAttachment, gainAttachement;
    ButtonAttachment typeAttachment;

    ScreenComponent screen;

    int knobWidth;
    int knobHeight;
};
