#pragma once
#include <JuceHeader.h>
#include "ScanVisualizer.h"

struct PaintPoint
{
    juce::Point<float> position;
    float pressure = 1.0f;
    float opacity = 1.0f;
    double timestampMs = 0.0;
};

struct PaintStroke
{
    enum class Tool
    {
        brush,
        splash,
        line,
        circle,
        triangle,
        star,
        spiral,
        mask,
        motion
    };

    juce::Uuid id;
    Tool tool = Tool::brush;
    juce::Colour colour = juce::Colours::red;
    float thickness = 0.02f;
    bool isMask = false;
    int seed = 1337;
    juce::Array<PaintPoint> points;
};

class PaintDocument
{
public:
    void clear();
    void addStroke(const PaintStroke& stroke);
    bool undoLast();
    const juce::Array<PaintStroke>& getStrokes() const noexcept { return strokes; }
    juce::Array<PaintStroke>& getMutableStrokes() noexcept { return strokes; }

private:
    juce::Array<PaintStroke> strokes;
};

class PaintCanvasComponent : public juce::Component
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void documentChanged() = 0;
    };

    PaintCanvasComponent(PaintDocument& document);

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;
    void mouseUp(const juce::MouseEvent&) override;

    void setCurrentTool(PaintStroke::Tool newTool);
    void setCurrentColour(juce::Colour newColour);
    void setCurrentThickness(float newThickness);
    void setListener(Listener* l) { listener = l; }
    void setOverlayMode(ScanVisualizer::Mode newMode) noexcept;
    void setOverlayBeatPosition(double newBeatPosition) noexcept;

private:
    PaintDocument& document;
    Listener* listener = nullptr;
    juce::Optional<PaintStroke> activeStroke;
    PaintStroke::Tool currentTool = PaintStroke::Tool::brush;
    juce::Colour currentColour = juce::Colours::red;
    float currentThickness = 0.02f;
    ScanVisualizer overlay;

    PaintPoint makePoint(const juce::MouseEvent& e) const;
    PaintStroke finaliseStrokeGeometry(const PaintStroke& seedStroke) const;
    void drawStroke(juce::Graphics& g, const PaintStroke& s, bool active) const;
    juce::Path buildStrokePath(const PaintStroke& s, juce::Rectangle<float> area) const;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PaintCanvasComponent)
};
