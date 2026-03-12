#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ScanVisualizer.h"

PaintMaskAudioProcessor::PaintMaskAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMS", createParameterLayout())
{
    for (const auto& param : apvts.processor.getParameters())
        apvts.addParameterListener(param->getParameterID(), this);

    seedDefaultLayersIfNeeded();
    rebuildGeneratedEvents();
}

PaintMaskAudioProcessor::~PaintMaskAudioProcessor()
{
    for (const auto& param : apvts.processor.getParameters())
        apvts.removeParameterListener(param->getParameterID(), this);
}

juce::AudioProcessorValueTreeState::ParameterLayout PaintMaskAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterChoice>("scanMode", "Scan Mode", juce::StringArray{ "L->R", "PingPong", "Circular", "Spiral", "Wave" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("loopBeats", "Loop Beats", juce::NormalisableRange<float>(1.0f, 16.0f, 1.0f), 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("rootNote", "Root Note", 24, 84, 48));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("swing", "Swing", 0.0f, 0.5f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("density", "Density", 0.0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("previewGain", "Preview Gain", 0.0f, 1.0f, 0.18f));
    return { params.begin(), params.end() };
}

void PaintMaskAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    previewSynth.prepare(sampleRate, samplesPerBlock);
    clearPlaybackState();
}

void PaintMaskAudioProcessor::releaseResources()
{
    clearPlaybackState();
}

bool PaintMaskAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void PaintMaskAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    const auto playHead = getPlayHead();
    juce::AudioPlayHead::CurrentPositionInfo info;
    double bpm = 120.0;
    bool isPlaying = false;

    if (playHead != nullptr && playHead->getCurrentPosition(info))
    {
        if (info.bpm > 0.0)
            bpm = info.bpm;

        currentPpq = info.ppqPosition;
        isPlaying = info.isPlaying;
    }

    previewSynth.setGain(*apvts.getRawParameterValue("previewGain"));

    if (! isPlaying)
    {
        clearPlaybackState();
        return;
    }

    if (currentPpq + 0.0001 < lastPpq)
        clearPlaybackState();

    pushEventsForBlock(buffer, midi, buffer.getNumSamples(), bpm, isPlaying);
    routeMidiToPreviewSynth(midi, buffer);
    lastPpq = currentPpq;
}

void PaintMaskAudioProcessor::flushPendingNoteOffs(juce::MidiBuffer& midi,
                                                   int numSamples,
                                                   double blockStartPpq,
                                                   double blockEndPpq,
                                                   double samplesPerBeat)
{
    for (int i = pendingNoteOffs.size(); --i >= 0;)
    {
        const auto& pending = pendingNoteOffs.getReference(i);
        if (pending.ppqPosition < blockStartPpq)
        {
            midi.addEvent(juce::MidiMessage::noteOff(pending.channel, pending.note), 0);
            pendingNoteOffs.remove(i);
            continue;
        }

        if (pending.ppqPosition >= blockStartPpq && pending.ppqPosition < blockEndPpq)
        {
            const auto relBeats = pending.ppqPosition - blockStartPpq;
            const int samplePos = juce::jlimit(0, numSamples - 1, int(std::round(relBeats * samplesPerBeat)));
            midi.addEvent(juce::MidiMessage::noteOff(pending.channel, pending.note), samplePos);
            pendingNoteOffs.remove(i);
        }
    }
}

void PaintMaskAudioProcessor::pushEventsForBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi, int numSamples, double bpm, bool)
{
    juce::ignoreUnused(buffer);
    if (generatedEvents.isEmpty())
        return;

    const auto loopBeats = juce::jmax(1.0, (double) *apvts.getRawParameterValue("loopBeats"));
    const auto density = juce::jlimit(0.0f, 1.0f, *apvts.getRawParameterValue("density"));
    const auto swing = juce::jlimit(0.0f, 0.5f, *apvts.getRawParameterValue("swing"));
    const auto densityBoost = performanceMode.isEnabled() ? performanceMode.getSceneA().densityBoost : 0.0f;
    const auto finalDensity = juce::jlimit(0.0f, 1.0f, density + densityBoost);

    const double samplesPerBeat = currentSampleRate * (60.0 / juce::jmax(1.0, bpm));
    const double blockStartPpq = currentPpq;
    const double blockLengthBeats = numSamples / juce::jmax(1.0, samplesPerBeat);
    const double blockEndPpq = blockStartPpq + blockLengthBeats;

    flushPendingNoteOffs(midi, numSamples, blockStartPpq, blockEndPpq, samplesPerBeat);

    auto emitEventWindow = [&](double localLoopStart, double absoluteLoopOffset)
    {
        const double localLoopEnd = localLoopStart + blockLengthBeats;

        for (int index = 0; index < generatedEvents.size(); ++index)
        {
            const auto& event = generatedEvents.getReference(index);
            if (event.isMask)
                continue;

            juce::Random rng((event.note * 131) + int(event.beatPosition * 1000.0) + int(loopBeats * 97.0));
            if (rng.nextFloat() > finalDensity)
                continue;

            double eventBeat = event.beatPosition;

            if (swing > 0.0f)
            {
                const auto eighths = std::floor(eventBeat * 2.0);
                const bool isOffbeat = (int(eighths) % 2) == 1;
                if (isOffbeat)
                    eventBeat += 0.5 * swing;
            }

            if (eventBeat < localLoopStart || eventBeat >= localLoopEnd)
                continue;

            const double absoluteEventPpq = absoluteLoopOffset + eventBeat;
            const double relBeats = absoluteEventPpq - blockStartPpq;
            const int samplePos = juce::jlimit(0, numSamples - 1, int(std::round(relBeats * samplesPerBeat)));
            const auto velocity = (juce::uint8) juce::jlimit(1, 127, event.velocity);

            midi.addEvent(juce::MidiMessage::noteOn(1, event.note, velocity), samplePos);

            const double noteOffPpq = absoluteEventPpq + juce::jmax(0.03125, event.durationBeats);
            if (noteOffPpq < blockEndPpq)
            {
                const int offSamplePos = juce::jlimit(0, numSamples - 1, int(std::round((noteOffPpq - blockStartPpq) * samplesPerBeat)));
                midi.addEvent(juce::MidiMessage::noteOff(1, event.note), juce::jmax(samplePos, offSamplePos));
            }
            else
            {
                pendingNoteOffs.add({ 1, event.note, noteOffPpq });
            }
        }
    };

    const double blockStartWithinLoop = std::fmod(blockStartPpq, loopBeats);
    const double blockStartBasePpq = blockStartPpq - blockStartWithinLoop;

    emitEventWindow(blockStartWithinLoop, blockStartBasePpq);

    if (blockStartWithinLoop + blockLengthBeats > loopBeats)
        emitEventWindow(0.0, blockStartBasePpq + loopBeats);
}

void PaintMaskAudioProcessor::routeMidiToPreviewSynth(const juce::MidiBuffer& midi, juce::AudioBuffer<float>& buffer)
{
    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())
            previewSynth.noteOn(msg.getNoteNumber(), msg.getFloatVelocity());
        else if (msg.isNoteOff())
            previewSynth.noteOff(msg.getNoteNumber());
    }

    previewSynth.render(buffer, 0, buffer.getNumSamples());
}

void PaintMaskAudioProcessor::clearPlaybackState()
{
    pendingNoteOffs.clearQuick();
    previewSynth.reset();
    lastPpq = 0.0;
}

void PaintMaskAudioProcessor::seedDefaultLayersIfNeeded()
{
    if (layerManager.getNumLayers() > 0)
        return;

    layerManager.addLayer("Bass");
    layerManager.addLayer("Harmony");
    layerManager.addLayer("Lead");
    layerManager.addLayer("FX");
}

juce::AudioProcessorEditor* PaintMaskAudioProcessor::createEditor()
{
    return new PaintMaskAudioProcessorEditor(*this);
}

void PaintMaskAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    serializer.writeState(destData, document, apvts.copyState(), trialManager);
}

void PaintMaskAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    serializer.readState(data, sizeInBytes, document, apvts, trialManager);
    rebuildGeneratedEvents();
}

void PaintMaskAudioProcessor::rebuildGeneratedEvents()
{
    const auto mode = static_cast<ScanEngine::Mode>((int) *apvts.getRawParameterValue("scanMode"));
    scanEngine.setMode(mode);
    generatedEvents = scanEngine.buildEvents(document,
                                             *apvts.getRawParameterValue("loopBeats"),
                                             (int) *apvts.getRawParameterValue("rootNote"),
                                             colourMapper);
}

void PaintMaskAudioProcessor::parameterChanged(const juce::String& parameterID, float)
{
    if (parameterID == "scanMode" || parameterID == "loopBeats" || parameterID == "rootNote")
        rebuildGeneratedEvents();
}

void PaintMaskAudioProcessor::applyMirrorX()
{
    document = mutationEngine.mirrorX(document);
    rebuildGeneratedEvents();
}

void PaintMaskAudioProcessor::applyMirrorY()
{
    document = mutationEngine.mirrorY(document);
    rebuildGeneratedEvents();
}

void PaintMaskAudioProcessor::applyRotateQuarterTurn()
{
    document = mutationEngine.rotateQuarterTurn(document);
    rebuildGeneratedEvents();
}

void PaintMaskAudioProcessor::applyDensityShift(float amount01)
{
    document = mutationEngine.densityShift(document, amount01);
    rebuildGeneratedEvents();
}

ScanVisualizer::Mode PaintMaskAudioProcessor::getOverlayMode() const noexcept
{
    switch (scanEngine.getMode())
    {
        case ScanEngine::Mode::leftToRight: return ScanVisualizer::Mode::line;
        case ScanEngine::Mode::pingPong:    return ScanVisualizer::Mode::pulse;
        case ScanEngine::Mode::circular:    return ScanVisualizer::Mode::radial;
        case ScanEngine::Mode::spiral:      return ScanVisualizer::Mode::spiral;
        case ScanEngine::Mode::wave:        return ScanVisualizer::Mode::pulse;
    }
    return ScanVisualizer::Mode::line;
}
