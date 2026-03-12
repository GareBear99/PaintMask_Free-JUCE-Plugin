#pragma once
#include <JuceHeader.h>
class ScanVisualizer
{
public:
    enum class Mode { line, radial, spiral, pulse };
    void setMode(Mode newMode) noexcept;
    void setBeatPosition(double newBeat) noexcept;
    void paint(juce::Graphics& g, juce::Rectangle<float> area) const;
private:
    Mode mode = Mode::line;
    double beatPosition = 0.0;
};
