#pragma once
#include <JuceHeader.h>

class PreviewSynth
{
public:
    void prepare(double sampleRate, int samplesPerBlock);
    void reset();
    void noteOn(int midiNote, float velocity);
    void noteOff(int midiNote);
    void render(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    void setGain(float newGain) noexcept { gain = juce::jlimit(0.0f, 1.0f, newGain); }

private:
    struct Voice
    {
        bool active = false;
        bool releasing = false;
        int note = -1;
        float velocity = 0.0f;
        double phase = 0.0;
        double phaseDelta = 0.0;
        float env = 0.0f;
    };

    std::array<Voice, 12> voices {};
    double currentSampleRate = 44100.0;
    float gain = 0.12f;

    Voice* findVoiceFor(int midiNote);
    Voice* findFreeVoice();
};
