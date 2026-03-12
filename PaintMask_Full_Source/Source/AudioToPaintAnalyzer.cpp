#include "AudioToPaintAnalyzer.h"
PaintDocument AudioToPaintAnalyzer::buildDocumentFromBuffer(const juce::AudioBuffer<float>& buffer) const
{
    PaintDocument document;
    if (buffer.getNumSamples() <= 0 || buffer.getNumChannels() <= 0) return document;
    PaintStroke stroke;
    stroke.tool = PaintStroke::Tool::line;
    stroke.colour = juce::Colours::cyan;
    stroke.thickness = 0.015f;
    const int samples = buffer.getNumSamples();
    const auto* ch = buffer.getReadPointer(0);
    for (int i = 0; i < samples; i += juce::jmax(1, samples / 128))
    {
        PaintPoint p;
        p.position.x = (float) i / (float) juce::jmax(1, samples - 1);
        p.position.y = juce::jlimit(0.0f, 1.0f, 0.5f - ch[i] * 0.45f);
        p.opacity = 1.0f;
        p.pressure = 1.0f;
        stroke.points.add(p);
    }
    document.addStroke(stroke);
    return document;
}
