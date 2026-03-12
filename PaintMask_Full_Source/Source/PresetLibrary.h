#pragma once
#include <JuceHeader.h>
struct PaintPresetInfo
{
    juce::String name;
    juce::String category;
    juce::File file;
};
class PresetLibrary
{
public:
    juce::Array<PaintPresetInfo> scanDirectory(const juce::File& dir) const;
};
