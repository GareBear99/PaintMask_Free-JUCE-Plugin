#include "LayerManager.h"
int LayerManager::addLayer(const juce::String& name) { PaintLayerState s; s.name = name; layers.push_back(s); return (int) layers.size() - 1; }
void LayerManager::clear() { layers.clear(); }
int LayerManager::getNumLayers() const noexcept { return (int) layers.size(); }
PaintLayerState& LayerManager::getLayer(int index) { return layers[(size_t) index]; }
const PaintLayerState& LayerManager::getLayer(int index) const { return layers[(size_t) index]; }
