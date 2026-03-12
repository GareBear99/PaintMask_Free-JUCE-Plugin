#include "PresetLibrary.h"
juce::Array<PaintPresetInfo> PresetLibrary::scanDirectory(const juce::File& dir) const
{
    juce::Array<PaintPresetInfo> out;
    if (! dir.isDirectory()) return out;
    for (const auto& f : dir.findChildFiles(juce::File::findFiles, false, "*.paintmask"))
    {
        PaintPresetInfo i;
        i.name = f.getFileNameWithoutExtension();
        i.category = "Unsorted";
        i.file = f;
        out.add(i);
    }
    return out;
}
