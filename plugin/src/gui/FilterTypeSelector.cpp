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

#include "FilterTypeSelector.h"

FilterTypeSelector::FilterTypeSelector()
{
    addAndMakeVisible(peakButton);
    addAndMakeVisible(bandButton);

    peakButton.onClick = [this] ()
    {
        peakButton.setToggleState(true, juce::dontSendNotification);
        bandButton.setToggleState(false, juce::dontSendNotification);

        if (onChange)
        {
            onChange(true);
        }
    };

    bandButton.onClick = [this] ()
    {
        peakButton.setToggleState(false, juce::dontSendNotification);
        bandButton.setToggleState(true, juce::dontSendNotification);

        if (onChange)
        {
            onChange(false);
        }
    };
}

void FilterTypeSelector::resized()
{
    auto bounds = getLocalBounds();

    peakButton.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
    bandButton.setBounds(bounds);
}

void FilterTypeSelector::getButtonRect(juce::Rectangle<float>* button)
{
    *button = localAreaToGlobal(getLocalBounds().toFloat());
}

void FilterTypeSelector::setFilterType(bool isPeak)
{
    peakButton.setToggleState(isPeak, juce::dontSendNotification);
    bandButton.setToggleState(!isPeak, juce::dontSendNotification);
}

void FilterTypeSelector::getButtonsStates(bool* peakState, bool* bandState)
{
    *peakState = peakButton.getToggleState();
    *bandState = bandButton.getToggleState();
}

void FilterTypeSelector::triggerPeakButtonClick()
{
    peakButton.onClick();
}

void FilterTypeSelector::triggerBandButtonClick()
{
    bandButton.onClick();
}

juce::TextButton& FilterTypeSelector::getPeakButton()
{
    return peakButton;
}
