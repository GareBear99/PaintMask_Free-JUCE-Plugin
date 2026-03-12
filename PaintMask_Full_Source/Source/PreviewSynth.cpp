#include "PreviewSynth.h"

namespace
{
static double midiToHz(int midiNote)
{
    return 440.0 * std::pow(2.0, (midiNote - 69) / 12.0);
}
}

void PreviewSynth::prepare(double sampleRate, int)
{
    currentSampleRate = juce::jmax(1.0, sampleRate);
    reset();
}

void PreviewSynth::reset()
{
    for (auto& v : voices)
        v = {};
}

PreviewSynth::Voice* PreviewSynth::findVoiceFor(int midiNote)
{
    for (auto& v : voices)
        if (v.active && v.note == midiNote)
            return &v;
    return nullptr;
}

PreviewSynth::Voice* PreviewSynth::findFreeVoice()
{
    for (auto& v : voices)
        if (! v.active)
            return &v;
    return &voices.front();
}

void PreviewSynth::noteOn(int midiNote, float velocity)
{
    auto* v = findFreeVoice();
    v->active = true;
    v->releasing = false;
    v->note = midiNote;
    v->velocity = juce::jlimit(0.0f, 1.0f, velocity);
    v->phase = 0.0;
    v->phaseDelta = juce::MathConstants<double>::twoPi * midiToHz(midiNote) / currentSampleRate;
    v->env = juce::jmax(v->env, 0.001f);
}

void PreviewSynth::noteOff(int midiNote)
{
    if (auto* v = findVoiceFor(midiNote))
        v->releasing = true;
}

void PreviewSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int numSamples)
{
    if (buffer.getNumChannels() <= 0 || numSamples <= 0)
        return;

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mix = 0.0f;

        for (auto& v : voices)
        {
            if (! v.active)
                continue;

            const float attack = 0.012f;
            const float release = 0.0009f;

            if (! v.releasing)
                v.env += (1.0f - v.env) * attack;
            else
                v.env *= (1.0f - release);

            if (v.env < 0.0005f && v.releasing)
            {
                v = {};
                continue;
            }

            const float s = std::sin(v.phase) * v.velocity * v.env;
            mix += s;
            v.phase += v.phaseDelta;
            if (v.phase > juce::MathConstants<double>::twoPi)
                v.phase -= juce::MathConstants<double>::twoPi;
        }

        mix *= gain;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.addSample(ch, startSample + sample, mix);
    }
}
