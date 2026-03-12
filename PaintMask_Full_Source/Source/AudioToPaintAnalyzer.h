#pragma once
#include <JuceHeader.h>
#include "PaintCanvas.h"
class AudioToPaintAnalyzer
{
public:
    PaintDocument buildDocumentFromBuffer(const juce::AudioBuffer<float>& buffer) const;
};
