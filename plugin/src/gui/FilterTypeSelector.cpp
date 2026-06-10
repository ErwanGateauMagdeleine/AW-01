#include "FilterTypeSelector.h"

FilterTypeSelector::FilterTypeSelector()
{
    addAndMakeVisible(peakButton);
    addAndMakeVisible(bandButton);

    peakButton.setToggleState(true, juce::dontSendNotification);
    bandButton.setToggleState(false, juce::dontSendNotification);

    peakButton.onClick = [this] ()
    {
        peakButton.setToggleState(true, juce::dontSendNotification);
        bandButton.setToggleState(false, juce::dontSendNotification);
    };

    bandButton.onClick = [this] ()
    {
        peakButton.setToggleState(false, juce::dontSendNotification);
        bandButton.setToggleState(true, juce::dontSendNotification);
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
