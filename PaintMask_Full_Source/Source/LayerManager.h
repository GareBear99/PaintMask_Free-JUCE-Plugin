#pragma once
#include <JuceHeader.h>
#include "ScanEngine.h"
struct PaintLayerState
{
    juce::String name { "Layer" };
    bool mute = false;
    bool solo = false;
    bool visible = true;
    int midiChannel = 1;
    int rootNote = 48;
    ScanEngine::Mode scanMode = ScanEngine::Mode::leftToRight;
};
class LayerManager
{
public:
    int addLayer(const juce::String& name);
    void clear();
    int getNumLayers() const noexcept;
    PaintLayerState& getLayer(int index);
    const PaintLayerState& getLayer(int index) const;
private:
    std::vector<PaintLayerState> layers;
};
