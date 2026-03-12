#pragma once
#include <JuceHeader.h>
#include "PaintCanvas.h"
class GestureBrushEngine
{
public:
    PaintStroke makeRhythmBrushStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour) const;
    PaintStroke makeChordBrushStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour) const;
    PaintStroke makeDrumLaneStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour, int laneIndex) const;
};
