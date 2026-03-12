#include "PatternMutationEngine.h"
namespace {
static PaintStroke mutateStroke(const PaintStroke& in, std::function<juce::Point<float>(juce::Point<float>)> fn)
{
    PaintStroke out = in;
    for (auto& p : out.points) p.position = fn(p.position);
    return out;
}}
PaintDocument PatternMutationEngine::mirrorX(const PaintDocument& source) const
{
    PaintDocument out;
    for (const auto& s : source.getStrokes()) out.addStroke(mutateStroke(s, [] (juce::Point<float> p) { return juce::Point<float>(1.0f - p.x, p.y); }));
    return out;
}
PaintDocument PatternMutationEngine::mirrorY(const PaintDocument& source) const
{
    PaintDocument out;
    for (const auto& s : source.getStrokes()) out.addStroke(mutateStroke(s, [] (juce::Point<float> p) { return juce::Point<float>(p.x, 1.0f - p.y); }));
    return out;
}
PaintDocument PatternMutationEngine::rotateQuarterTurn(const PaintDocument& source) const
{
    PaintDocument out;
    for (const auto& s : source.getStrokes()) out.addStroke(mutateStroke(s, [] (juce::Point<float> p) { return juce::Point<float>(p.y, 1.0f - p.x); }));
    return out;
}
PaintDocument PatternMutationEngine::densityShift(const PaintDocument& source, float amount01) const
{
    PaintDocument out;
    const int stride = juce::jlimit(1, 8, 1 + int((1.0f - amount01) * 6.0f));
    for (const auto& s : source.getStrokes())
    {
        PaintStroke copy = s;
        juce::Array<PaintPoint> reduced;
        for (int i = 0; i < copy.points.size(); i += stride) reduced.add(copy.points.getReference(i));
        copy.points = reduced;
        out.addStroke(copy);
    }
    return out;
}
