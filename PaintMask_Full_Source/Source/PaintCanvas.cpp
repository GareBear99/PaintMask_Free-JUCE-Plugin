#include "PaintCanvas.h"

namespace
{
static juce::Point<float> toPixelPoint(const PaintPoint& p, juce::Rectangle<float> area)
{
    return { area.getX() + p.position.x * area.getWidth(),
             area.getY() + p.position.y * area.getHeight() };
}

static PaintPoint makeDerivedPoint(juce::Point<float> normalizedPosition)
{
    PaintPoint p;
    p.position.x = juce::jlimit(0.0f, 1.0f, normalizedPosition.x);
    p.position.y = juce::jlimit(0.0f, 1.0f, normalizedPosition.y);
    p.pressure = 1.0f;
    p.opacity = 1.0f;
    p.timestampMs = juce::Time::getMillisecondCounterHiRes();
    return p;
}
}

void PaintDocument::clear()
{
    strokes.clear();
}

void PaintDocument::addStroke(const PaintStroke& stroke)
{
    strokes.add(stroke);
}

bool PaintDocument::undoLast()
{
    if (strokes.isEmpty())
        return false;

    strokes.removeLast();
    return true;
}

PaintCanvasComponent::PaintCanvasComponent(PaintDocument& d) : document(d)
{
    setOpaque(true);
    overlay.setMode(ScanVisualizer::Mode::line);
}

void PaintCanvasComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(8, 8, 12));

    auto bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colour::fromRGBA(255, 255, 255, 12));
    for (int i = 1; i < 8; ++i)
    {
        const auto x = bounds.getX() + bounds.getWidth() * (float(i) / 8.0f);
        const auto y = bounds.getY() + bounds.getHeight() * (float(i) / 8.0f);
        g.drawVerticalLine(int(x), bounds.getY(), bounds.getBottom());
        g.drawHorizontalLine(int(y), bounds.getX(), bounds.getRight());
    }

    for (const auto& s : document.getStrokes())
        drawStroke(g, s, false);

    if (activeStroke.hasValue())
        drawStroke(g, finaliseStrokeGeometry(*activeStroke), true);

    overlay.paint(g, getLocalBounds().toFloat().reduced(8.0f));
}

void PaintCanvasComponent::resized() {}

PaintPoint PaintCanvasComponent::makePoint(const juce::MouseEvent& e) const
{
    const auto bounds = getLocalBounds().toFloat();
    auto pos = e.position;
    pos.x = juce::jlimit(bounds.getX(), bounds.getRight(), pos.x);
    pos.y = juce::jlimit(bounds.getY(), bounds.getBottom(), pos.y);

    PaintPoint p;
    p.position = { pos.x / bounds.getWidth(), pos.y / bounds.getHeight() };
    p.pressure = e.pressure > 0.0f ? e.pressure : 1.0f;
    p.opacity = 1.0f;
    p.timestampMs = juce::Time::getMillisecondCounterHiRes();
    return p;
}

void PaintCanvasComponent::mouseDown(const juce::MouseEvent& e)
{
    PaintStroke s;
    s.id = juce::Uuid();
    s.tool = currentTool;
    s.colour = currentColour;
    s.thickness = currentThickness;
    s.isMask = (currentTool == PaintStroke::Tool::mask);
    s.seed = int(juce::Time::getMillisecondCounter() & 0x7fffffff);
    s.points.add(makePoint(e));
    activeStroke = s;
    repaint();
}

void PaintCanvasComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (! activeStroke.hasValue())
        return;

    activeStroke->points.add(makePoint(e));
    repaint();
}

void PaintCanvasComponent::mouseUp(const juce::MouseEvent& e)
{
    if (! activeStroke.hasValue())
        return;

    activeStroke->points.add(makePoint(e));
    auto finished = finaliseStrokeGeometry(*activeStroke);
    document.addStroke(finished);
    activeStroke.reset();
    if (listener != nullptr)
        listener->documentChanged();
    repaint();
}

void PaintCanvasComponent::setCurrentTool(PaintStroke::Tool newTool)
{
    currentTool = newTool;
}

void PaintCanvasComponent::setCurrentColour(juce::Colour newColour)
{
    currentColour = newColour;
}

void PaintCanvasComponent::setCurrentThickness(float newThickness)
{
    currentThickness = juce::jlimit(0.002f, 0.2f, newThickness);
}

PaintStroke PaintCanvasComponent::finaliseStrokeGeometry(const PaintStroke& seedStroke) const
{
    PaintStroke out(seedStroke);

    if (seedStroke.points.isEmpty())
        return out;

    const auto first = seedStroke.points.getFirst().position;
    const auto last = seedStroke.points.getLast().position;
    const auto dx = last.x - first.x;
    const auto dy = last.y - first.y;
    const auto radius = juce::jmax(0.01f, std::sqrt(dx * dx + dy * dy));

    auto setSingleSegment = [&]()
    {
        out.points.clearQuick();
        out.points.add(makeDerivedPoint(first));
        out.points.add(makeDerivedPoint(last));
    };

    switch (seedStroke.tool)
    {
        case PaintStroke::Tool::line:
        case PaintStroke::Tool::motion:
        case PaintStroke::Tool::mask:
        case PaintStroke::Tool::brush:
            if (seedStroke.tool == PaintStroke::Tool::line && seedStroke.points.size() > 1)
                setSingleSegment();
            return out;

        case PaintStroke::Tool::circle:
        {
            out.points.clearQuick();
            constexpr int steps = 40;
            for (int i = 0; i <= steps; ++i)
            {
                const auto angle = juce::MathConstants<float>::twoPi * (float(i) / float(steps));
                const juce::Point<float> p { first.x + std::cos(angle) * radius,
                                             first.y + std::sin(angle) * radius };
                out.points.add(makeDerivedPoint(p));
            }
            return out;
        }

        case PaintStroke::Tool::triangle:
        {
            out.points.clearQuick();
            constexpr int verts = 3;
            const auto baseAngle = std::atan2(dy, dx);
            for (int i = 0; i <= verts; ++i)
            {
                const auto angle = baseAngle - juce::MathConstants<float>::halfPi
                                 + juce::MathConstants<float>::twoPi * (float(i % verts) / float(verts));
                const juce::Point<float> p { first.x + std::cos(angle) * radius,
                                             first.y + std::sin(angle) * radius };
                out.points.add(makeDerivedPoint(p));
            }
            return out;
        }

        case PaintStroke::Tool::star:
        {
            out.points.clearQuick();
            constexpr int points = 5;
            const auto baseAngle = std::atan2(dy, dx) - juce::MathConstants<float>::halfPi;
            const auto innerRadius = radius * 0.45f;
            for (int i = 0; i <= points * 2; ++i)
            {
                const bool outer = (i % 2 == 0);
                const auto localRadius = outer ? radius : innerRadius;
                const auto angle = baseAngle + juce::MathConstants<float>::pi * (float(i) / float(points));
                const juce::Point<float> p { first.x + std::cos(angle) * localRadius,
                                             first.y + std::sin(angle) * localRadius };
                out.points.add(makeDerivedPoint(p));
            }
            return out;
        }

        case PaintStroke::Tool::spiral:
        {
            out.points.clearQuick();
            constexpr int steps = 64;
            const auto baseAngle = std::atan2(dy, dx);
            for (int i = 0; i < steps; ++i)
            {
                const auto t = float(i) / float(steps - 1);
                const auto localRadius = radius * t;
                const auto angle = baseAngle + juce::MathConstants<float>::twoPi * 2.75f * t;
                const juce::Point<float> p { first.x + std::cos(angle) * localRadius,
                                             first.y + std::sin(angle) * localRadius };
                out.points.add(makeDerivedPoint(p));
            }
            return out;
        }

        case PaintStroke::Tool::splash:
        {
            out.points.clearQuick();
            juce::Random rng(seedStroke.seed);
            constexpr int particles = 28;
            for (int i = 0; i < particles; ++i)
            {
                const auto angle = rng.nextFloat() * juce::MathConstants<float>::twoPi;
                const auto localRadius = radius * (0.15f + 0.85f * rng.nextFloat());
                const juce::Point<float> p { first.x + std::cos(angle) * localRadius,
                                             first.y + std::sin(angle) * localRadius };
                out.points.add(makeDerivedPoint(first));
                out.points.add(makeDerivedPoint(p));
            }
            return out;
        }
    }

    return out;
}

juce::Path PaintCanvasComponent::buildStrokePath(const PaintStroke& s, juce::Rectangle<float> area) const
{
    juce::Path path;
    if (s.points.isEmpty())
        return path;

    const auto first = toPixelPoint(s.points.getFirst(), area);
    path.startNewSubPath(first);

    for (int i = 1; i < s.points.size(); ++i)
        path.lineTo(toPixelPoint(s.points.getReference(i), area));

    return path;
}

void PaintCanvasComponent::drawStroke(juce::Graphics& g, const PaintStroke& s, bool active) const
{
    if (s.points.isEmpty())
        return;

    auto area = getLocalBounds().toFloat();
    auto path = buildStrokePath(s, area);

    auto c = s.isMask ? juce::Colours::black.withAlpha(0.85f)
                      : s.colour.withAlpha(active ? 0.95f : 0.78f);

    const float width = juce::jmax(1.0f, s.thickness * juce::jmin(area.getWidth(), area.getHeight()));

    if (! s.isMask)
    {
        g.setColour(s.colour.withAlpha(active ? 0.14f : 0.08f));
        g.strokePath(path, juce::PathStrokeType(width * 2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    g.setColour(c);
    g.strokePath(path, juce::PathStrokeType(width, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (! s.isMask)
    {
        g.setColour(juce::Colours::white.withAlpha(active ? 0.18f : 0.08f));
        g.strokePath(path, juce::PathStrokeType(juce::jmax(1.0f, width * 0.22f), juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
}

void PaintCanvasComponent::setOverlayMode(ScanVisualizer::Mode newMode) noexcept
{
    overlay.setMode(newMode);
    repaint();
}

void PaintCanvasComponent::setOverlayBeatPosition(double newBeatPosition) noexcept
{
    overlay.setBeatPosition(newBeatPosition);
    repaint();
}
