#include "ScanVisualizer.h"
void ScanVisualizer::setMode(Mode newMode) noexcept { mode = newMode; }
void ScanVisualizer::setBeatPosition(double newBeat) noexcept { beatPosition = newBeat; }
void ScanVisualizer::paint(juce::Graphics& g, juce::Rectangle<float> area) const
{
    g.setColour(juce::Colours::white.withAlpha(0.15f));
    switch (mode)
    {
        case Mode::line:
        {
            const float x = area.getX() + area.getWidth() * (float) std::fmod(beatPosition, 1.0);
            g.drawLine(x, area.getY(), x, area.getBottom(), 2.0f);
            break;
        }
        case Mode::radial: g.drawEllipse(area.reduced(10.0f), 2.0f); break;
        case Mode::spiral: g.drawEllipse(area.reduced(20.0f), 2.0f); break;
        case Mode::pulse: g.drawRoundedRectangle(area.reduced(8.0f), 10.0f, 2.0f); break;
    }
}
