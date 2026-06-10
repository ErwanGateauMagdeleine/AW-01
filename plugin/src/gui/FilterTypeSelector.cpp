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
