#include "FilterComponent.h"
#include "colourScheme.h"

FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& parameters, AutoWah<float>& wah) :
    freqSlider("Cutoff"),
    resSlider("Res"),
    morphSlider("Morph"),
    gainSlider("Gain"),
    freqAttachment(parameters, "Filter Center Frequency", freqSlider),
    resAttachment(parameters, "Filter Renonance", resSlider),
    morphAttachment(parameters, "Filter Morph", morphSlider),
    gainAttachement(parameters, "Filter Gain", gainSlider),
    typeAttachment(parameters, "Filter Type", filterSelector.getPeakButton()),
    screen(wah)
{
    for (auto* s : { &freqSlider, &resSlider, &morphSlider, &gainSlider })
    {
        addAndMakeVisible(*s);
    }
    addAndMakeVisible(screen);
    addAndMakeVisible(filterSelector);

    filterSelector.onChange = [this] (bool isPeak)
    {
        if (onChange) onChange(isPeak);
    };
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
    auto knobsAreaBounds = bounds.removeFromTop(100).reduced(15, 15).translated(0, 10);
    auto buttonBounds = bounds.removeFromTop(100).reduced(10, 10);
    auto screenBounds = bounds.reduced(10, 10);

    knobWidth = knobsAreaBounds.getWidth() / 3;
    knobHeight = knobsAreaBounds.getHeight();

    freqSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    resSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));
    morphSlider.setBounds(knobsAreaBounds.removeFromLeft(knobWidth));

     auto freqResArea = freqSlider.getBounds().withRight(resSlider.getBounds().getRight())
                                  .withY(buttonBounds.getY())
                                  .withHeight(buttonBounds.getHeight());

    filterSelector.setBounds(freqResArea.withSizeKeepingCentre(100, buttonBounds.getHeight() / 2));
    gainSlider.setBounds(morphSlider.getBounds().withY(buttonBounds.getY())
                                    .withHeight(buttonBounds.getHeight()));
    screen.setBounds(screenBounds);
}

void FilterComponent::getKnobSize(int* width, int* height)
{
    *width = knobWidth;
    *height = knobHeight;
}

void FilterComponent::getScreenRects(juce::Rectangle<float>* screenRect, juce::Rectangle<float>* gainRect, juce::Rectangle<float>* freqRect)
{
    screen.getScreenRects(screenRect, gainRect, freqRect);
}

void FilterComponent::getKnobRects(juce::Rectangle<float>* filterKnob)
{
    auto bounds = getLocalBounds();
    *filterKnob = localAreaToGlobal(bounds.removeFromTop((int)(bounds.getHeight() / 3.0f)).reduced(15, 15).translated(0, 10)).toFloat();
}

juce::Rectangle<float> FilterComponent::getButtonRect(void)
{
    return localAreaToGlobal(gainSlider.getBounds().toFloat());
}

void FilterComponent::setFilterType(bool isPeak)
{
    filterSelector.setFilterType(isPeak);
}

void FilterComponent::getButtonsStates(bool* peakState, bool* bandState)
{
    filterSelector.getButtonsStates(peakState, bandState);
}

void FilterComponent::triggerPeakButtonClick()
{
    filterSelector.triggerPeakButtonClick();
}

void FilterComponent::triggerBandButtonClick()
{
    filterSelector.triggerBandButtonClick();
}
