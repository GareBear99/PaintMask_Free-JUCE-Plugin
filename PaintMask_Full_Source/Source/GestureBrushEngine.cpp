#include "GestureBrushEngine.h"
namespace {
static PaintPoint makePoint(float x, float y)
{
    PaintPoint p;
    p.position = { x, y };
    p.pressure = 1.0f;
    p.opacity = 1.0f;
    return p;
}}
PaintStroke GestureBrushEngine::makeRhythmBrushStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour) const
{
    PaintStroke s;
    s.tool = PaintStroke::Tool::line;
    s.colour = colour;
    s.thickness = 0.02f;
    for (int i = 0; i < 16; ++i)
    {
        const float t = i / 15.0f;
        const float x = juce::jmap(t, from.x, to.x);
        const float y = std::round(juce::jmap(t, from.y, to.y) * 8.0f) / 8.0f;
        s.points.add(makePoint(x, y));
    }
    return s;
}
PaintStroke GestureBrushEngine::makeChordBrushStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour) const
{
    auto s = makeRhythmBrushStroke(from, to, colour.brighter(0.2f));
    s.tool = PaintStroke::Tool::triangle;
    return s;
}
PaintStroke GestureBrushEngine::makeDrumLaneStroke(const juce::Point<float>& from, const juce::Point<float>& to, juce::Colour colour, int laneIndex) const
{
    auto s = makeRhythmBrushStroke(from, to, colour);
    const float laneY = juce::jlimit(0.0f, 1.0f, (laneIndex + 0.5f) / 8.0f);
    for (auto& p : s.points) p.position.y = laneY;
    s.tool = PaintStroke::Tool::line;
    return s;
}
