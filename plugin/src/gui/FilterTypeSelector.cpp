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

enum filterIndexes
{
    LPF = 0,
    BPF = 1,
    HPF = 2,
};

FilterTypeSelector::FilterTypeSelector(juce::RangedAudioParameter& filterTypeParam)
    : attachment (filterTypeParam,
                  [this] (float newValue)
                  {
                    const int index = juce::roundToInt(newValue);
                    if (juce::isPositiveAndBelow (index, numButtons))
                    {
                        buttons[(size_t)index].setToggleState(true, juce::dontSendNotification);
                    }
                  })
{
    const char* names[numButtons] = { "LP", "BP", "HP" };

    for (int i = 0; i < numButtons; i++)
    {
        auto& b = buttons[(size_t)i];
        b.setButtonText(names[i]);
        b.setClickingTogglesState(true);
        b.setRadioGroupId(1001);

        b.onClick = [this, i]
        {
            if (buttons[(size_t)i].getToggleState())
            {
                attachment.setValueAsCompleteGesture((float) i);
            }
        };

        addAndMakeVisible(b);
    }

    attachment.sendInitialUpdate();
}

void FilterTypeSelector::resized()
{
    auto area = getLocalBounds();
    const int width = area.getHeight() / numButtons;

    for (auto& b : buttons)
    {
        b.setBounds (area.removeFromTop(width));
    }
}

void FilterTypeSelector::getLpfButtonState(bool* lpfState)
{
    *lpfState = buttons[LPF].getToggleState();
}

void FilterTypeSelector::getBpfButtonState(bool* bpfState)
{
    *bpfState = buttons[BPF].getToggleState();
}

void FilterTypeSelector::getHpfButtonState(bool* hpfState)
{
    *hpfState = buttons[HPF].getToggleState();
}
