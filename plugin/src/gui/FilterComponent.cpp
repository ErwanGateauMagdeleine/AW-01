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

#include "FilterComponent.h"
#include "colourScheme.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& parameters, AutoWah<float>& wah) :
    freqSlider("Cutoff"),
    resSlider("Res"),
    freqAttachment(parameters, "Filter Center Frequency", freqSlider),
    resAttachment(parameters, "Filter Renonance", resSlider),
    screen(wah),
    filterSelector(*parameters.getParameter("Filter Type"))
{
    for (auto* s : { &freqSlider, &resSlider })
    {
        addAndMakeVisible(*s);
    }
    addAndMakeVisible(screen);
    addAndMakeVisible(filterSelector);
}

void FilterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    if (auto* lnf = dynamic_cast<customLookAndFeel*> (&getLookAndFeel()))
    {
        lnf->drawComponentBoundaries(g, "Filter", bounds, juce::Justification::topLeft);
    }
}

void FilterComponent::resized()
{
    auto bounds = getLocalBounds();
    auto knobsAreaBounds = bounds.removeFromTop(100).reduced(15, 20).translated(0, 15);
    auto buttonBounds = bounds.removeFromTop(100).reduced(10, 10);
    auto screenBounds = bounds.reduced(10, 10);

    knobWidth = knobsAreaBounds.getWidth() / 3;
    knobHeight = knobsAreaBounds.getHeight();

    freqSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    resSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    filterSelector.setBounds(knobsAreaBounds);

    screen.setBounds(screenBounds);
}

#if defined(JUCE_UNIT_TESTS)
void FilterComponent::getKnobSize(int* width, int* height)
{
    *width = knobWidth;
    *height = knobHeight;
}

void FilterComponent::getFilterButtonStates(bool* lpfState, bool* bpfState, bool* hpfState)
{
    filterSelector.getLpfButtonState(lpfState);
    filterSelector.getBpfButtonState(bpfState);
    filterSelector.getHpfButtonState(hpfState);
}

void FilterComponent::lpfButtonTriggerClick()
{
    filterSelector.lpfButtonTriggerClick();
}

void FilterComponent::hpfButtonTriggerClick()
{
    filterSelector.hpfButtonTriggerClick();
}

void FilterComponent::bpfButtonTriggerClick()
{
    filterSelector.bpfButtonTriggerClick();
}
#endif /* JUCE_UNIT_TESTS */
