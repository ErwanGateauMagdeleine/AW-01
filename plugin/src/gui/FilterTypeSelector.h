#include <juce_gui_basics/juce_gui_basics.h>

class FilterTypeSelector : public juce::Component
{
public:
    FilterTypeSelector();

    void resized() override;

    void getButtonRect(juce::Rectangle<float>* button);

    std::function<void(bool)> onChange;

    void setFilterType(bool isPeak);

    void getButtonsStates(bool* peakState, bool* bandState);

    void triggerPeakButtonClick();

    void triggerBandButtonClick();

    juce::TextButton& getPeakButton();

private:
    juce::TextButton peakButton { "PEQ" };
    juce::TextButton bandButton { "BP" };
};
