#include <juce_gui_basics/juce_gui_basics.h>

class FilterTypeSelector : public juce::Component
{
public:
    FilterTypeSelector();

    void resized() override;

private:
    juce::TextButton peakButton { "PEQ" };
    juce::TextButton bandButton { "BP" };
};
