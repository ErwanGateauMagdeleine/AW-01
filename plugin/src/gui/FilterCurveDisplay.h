#include <juce_gui_basics/juce_gui_basics.h>
#include "../dsp/AutoWah.h"

class FilterCurveDisplay : public juce::Component,
                           public juce::Timer
{
public:
    FilterCurveDisplay(AutoWah<float>& leftWah,
                       AutoWah<float>& rightWah,
                       float dbMin,
                       float dbMax,
                       float freqMin,
                       float freqMax,
                       float cSize);

    void timerCallback() override;
    void paint(juce::Graphics& g) override;
private:
    AutoWah<float>& leftWah;
    AutoWah<float>& rightWah;

    float dbMin, dbMax, freqMin, freqMax;

    float cornerSize = 12.0f;
};
