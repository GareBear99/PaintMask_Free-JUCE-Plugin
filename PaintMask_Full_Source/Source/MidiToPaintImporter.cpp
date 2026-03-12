#include "MidiToPaintImporter.h"

namespace
{
static float clamp01(float v)
{
    return juce::jlimit(0.0f, 1.0f, v);
}
}

float MidiToPaintImporter::beatToX(double beat, const Settings& settings) const
{
    if (settings.loopLengthBeats <= 0.0)
        return 0.0f;
    return clamp01((float) (beat / settings.loopLengthBeats));
}

float MidiToPaintImporter::noteToY(int midiNote, const Settings& settings) const
{
    const int span = juce::jmax(1, settings.maxMidiNote - settings.minMidiNote);
    const float t = (float) (midiNote - settings.minMidiNote) / (float) span;
    return clamp01(1.0f - t);
}

PaintStroke MidiToPaintImporter::buildStrokeForSingleNote(const juce::MidiMessage& noteOn,
                                                          double endBeat,
                                                          int channel,
                                                          const Settings& settings) const
{
    PaintStroke stroke;
    stroke.tool = PaintStroke::Tool::line;
    stroke.thickness = 0.01f + ((float) noteOn.getVelocity() / 127.0f) * 0.03f;
    stroke.colour = juce::Colour::fromHSV(
        juce::jlimit(0.0f, 1.0f, (float) (channel - 1) / 15.0f),
        0.75f,
        0.95f,
        1.0f);

    PaintPoint a;
    a.position.x = beatToX(noteOn.getTimeStamp(), settings);
    a.position.y = noteToY(noteOn.getNoteNumber(), settings);
    a.opacity = juce::jlimit(0.2f, 1.0f, (float) noteOn.getVelocity() / 127.0f);
    a.pressure = 1.0f;

    PaintPoint b = a;
    b.position.x = beatToX(endBeat, settings);

    stroke.points.add(a);
    stroke.points.add(b);
    return stroke;
}

PaintDocument MidiToPaintImporter::importSequence(const juce::MidiMessageSequence& seq,
                                                  const Settings& settings) const
{
    PaintDocument doc;

    for (int i = 0; i < seq.getNumEvents(); ++i)
    {
        auto* ev = seq.getEventPointer(i);
        if (ev == nullptr || ! ev->message.isNoteOn())
            continue;

        const auto endTime = ev->noteOffObject != nullptr
            ? ev->noteOffObject->message.getTimeStamp()
            : (ev->message.getTimeStamp() + 0.25);

        auto stroke = buildStrokeForSingleNote(
            ev->message,
            endTime,
            ev->message.getChannel(),
            settings);

        doc.addStroke(stroke);
    }

    return doc;
}
