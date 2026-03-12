#pragma once
#include <JuceHeader.h>
#include "PaintCanvas.h"

class MidiToPaintImporter
{
public:
    enum class Mode
    {
        lane,
        phrase,
        chord
    };

    struct Settings
    {
        double loopLengthBeats = 4.0;
        int minMidiNote = 36;
        int maxMidiNote = 84;
        int seed = 1337;
        Mode mode = Mode::phrase;
    };

    PaintDocument importSequence(const juce::MidiMessageSequence& seq, const Settings& settings) const;

private:
    PaintStroke buildStrokeForSingleNote(const juce::MidiMessage& noteOn,
                                         double endBeat,
                                         int channel,
                                         const Settings& settings) const;

    float beatToX(double beat, const Settings& settings) const;
    float noteToY(int midiNote, const Settings& settings) const;
};
