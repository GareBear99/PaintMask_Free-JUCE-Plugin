#include "PluginEditor.h"

PaintMaskAudioProcessorEditor::PaintMaskAudioProcessorEditor(PaintMaskAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p), canvas(p.getDocument())
{
    setSize(1360, 860);

    titleLabel.setText("PaintMask Studio", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(30.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(titleLabel);

    subtitleLabel.setText("Visual composition instrument • preview synth • mutation tools • performance mode", juce::dontSendNotification);
    subtitleLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.72f));
    addAndMakeVisible(subtitleLabel);

    addAndMakeVisible(statusLabel);
    statusLabel.setJustificationType(juce::Justification::centredRight);

    layerInfoLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.62f));
    addAndMakeVisible(layerInfoLabel);

    addAndMakeVisible(canvas);
    canvas.setListener(this);

    for (auto* b : { &brushButton, &splashButton, &lineButton, &circleButton, &triangleButton, &starButton, &spiralButton, &maskButton, &undoButton, &clearButton, &unlockButton, &mirrorXButton, &mirrorYButton, &rotateButton, &densityLoButton, &densityHiButton, &performanceButton })
    {
        addAndMakeVisible(b);
        b->addListener(this);
    }

    addAndMakeVisible(colourBox);
    colourBox.addItem("Red", 1);
    colourBox.addItem("Green", 2);
    colourBox.addItem("Blue", 3);
    colourBox.addItem("Yellow", 4);
    colourBox.addItem("Magenta", 5);
    colourBox.addItem("Cyan", 6);
    colourBox.setSelectedId(1);
    colourBox.addListener(this);

    addAndMakeVisible(scanModeBox);
    scanModeBox.addItemList({ "L->R", "PingPong", "Circular", "Spiral", "Wave" }, 1);
    scanModeBox.addListener(this);
    scanModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.getValueTreeState(), "scanMode", scanModeBox);

    addAndMakeVisible(thicknessSlider);
    thicknessSlider.setRange(0.01, 0.10, 0.001);
    thicknessSlider.setValue(0.02);
    thicknessSlider.setTextValueSuffix(" size");
    thicknessSlider.addListener(this);

    addAndMakeVisible(previewGainSlider);
    previewGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    previewGainSlider.setTextValueSuffix(" preview");
    previewGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTreeState(), "previewGain", previewGainSlider);

    performanceButton.setClickingTogglesState(true);

    setTool(PaintStroke::Tool::brush);
    refreshStatus();
    startTimerHz(30);
}

PaintMaskAudioProcessorEditor::~PaintMaskAudioProcessorEditor()
{
    stopTimer();
    for (auto* b : { &brushButton, &splashButton, &lineButton, &circleButton, &triangleButton, &starButton, &spiralButton, &maskButton, &undoButton, &clearButton, &unlockButton, &mirrorXButton, &mirrorYButton, &rotateButton, &densityLoButton, &densityHiButton, &performanceButton })
        b->removeListener(this);
}

void PaintMaskAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(11, 10, 16));

    auto area = getLocalBounds().toFloat();
    juce::ColourGradient grad(juce::Colour::fromRGB(42, 18, 42), area.getTopLeft(),
                              juce::Colour::fromRGB(8, 8, 12), area.getBottomRight(), false);
    g.setGradientFill(grad);
    g.fillRect(area.removeFromTop(104.0f));

    g.setColour(juce::Colours::white.withAlpha(0.08f));
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8.0f), 18.0f, 1.0f);
}

void PaintMaskAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(14);
    auto header = area.removeFromTop(84);
    titleLabel.setBounds(header.removeFromLeft(300));
    subtitleLabel.setBounds(header.removeFromLeft(430));
    layerInfoLabel.setBounds(header.removeFromLeft(220));
    statusLabel.setBounds(header.removeFromRight(360));

    auto toolbarA = area.removeFromTop(44);
    auto placeA = [&toolbarA](juce::Component& c, int w) { c.setBounds(toolbarA.removeFromLeft(w).reduced(2)); };
    placeA(brushButton, 78); placeA(splashButton, 78); placeA(lineButton, 70); placeA(circleButton, 78);
    placeA(triangleButton, 86); placeA(starButton, 70); placeA(spiralButton, 78); placeA(maskButton, 72);
    placeA(undoButton, 72); placeA(clearButton, 72); placeA(performanceButton, 118);

    auto toolbarB = area.removeFromTop(44);
    auto placeB = [&toolbarB](juce::Component& c, int w) { c.setBounds(toolbarB.removeFromLeft(w).reduced(2)); };
    placeB(colourBox, 110); placeB(scanModeBox, 130); placeB(thicknessSlider, 170); placeB(previewGainSlider, 170);
    placeB(mirrorXButton, 92); placeB(mirrorYButton, 92); placeB(rotateButton, 96); placeB(densityLoButton, 72); placeB(densityHiButton, 72);
    placeB(unlockButton, 160);

    canvas.setBounds(area.reduced(0, 8));
}

void PaintMaskAudioProcessorEditor::documentChanged()
{
    processor.rebuildGeneratedEvents();
    refreshStatus();
}

void PaintMaskAudioProcessorEditor::buttonClicked(juce::Button* b)
{
    if (b == &brushButton) setTool(PaintStroke::Tool::brush);
    else if (b == &splashButton) setTool(PaintStroke::Tool::splash);
    else if (b == &lineButton) setTool(PaintStroke::Tool::line);
    else if (b == &circleButton) setTool(PaintStroke::Tool::circle);
    else if (b == &triangleButton) setTool(PaintStroke::Tool::triangle);
    else if (b == &starButton) setTool(PaintStroke::Tool::star);
    else if (b == &spiralButton) setTool(PaintStroke::Tool::spiral);
    else if (b == &maskButton) setTool(PaintStroke::Tool::mask);
    else if (b == &undoButton)
    {
        if (processor.getDocument().undoLast())
        {
            processor.rebuildGeneratedEvents();
            canvas.repaint();
        }
    }
    else if (b == &clearButton)
    {
        processor.getDocument().clear();
        processor.rebuildGeneratedEvents();
        canvas.repaint();
    }
    else if (b == &unlockButton)
    {
        processor.getTrialManager().activateStudioLocally("PM-STUDIO-LOCAL");
        refreshStatus();
    }
    else if (b == &mirrorXButton)
    {
        processor.applyMirrorX();
        canvas.repaint();
    }
    else if (b == &mirrorYButton)
    {
        processor.applyMirrorY();
        canvas.repaint();
    }
    else if (b == &rotateButton)
    {
        processor.applyRotateQuarterTurn();
        canvas.repaint();
    }
    else if (b == &densityLoButton)
    {
        processor.applyDensityShift(0.35f);
        canvas.repaint();
    }
    else if (b == &densityHiButton)
    {
        processor.applyDensityShift(0.9f);
        canvas.repaint();
    }
    else if (b == &performanceButton)
    {
        processor.getPerformanceMode().setEnabled(performanceButton.getToggleState());
        refreshStatus();
    }
}

void PaintMaskAudioProcessorEditor::comboBoxChanged(juce::ComboBox* box)
{
    if (box == &colourBox)
    {
        switch (colourBox.getSelectedId())
        {
            case 2: canvas.setCurrentColour(juce::Colours::green); break;
            case 3: canvas.setCurrentColour(juce::Colours::blue); break;
            case 4: canvas.setCurrentColour(juce::Colours::yellow); break;
            case 5: canvas.setCurrentColour(juce::Colours::magenta); break;
            case 6: canvas.setCurrentColour(juce::Colours::cyan); break;
            default: canvas.setCurrentColour(juce::Colours::red); break;
        }
    }
}

void PaintMaskAudioProcessorEditor::sliderValueChanged(juce::Slider* s)
{
    if (s == &thicknessSlider)
        canvas.setCurrentThickness((float) thicknessSlider.getValue());
}

void PaintMaskAudioProcessorEditor::timerCallback()
{
    canvas.setOverlayMode(processor.getOverlayMode());
    canvas.setOverlayBeatPosition(std::fmod(processor.getCurrentPpq(), 1.0));
}

void PaintMaskAudioProcessorEditor::setTool(PaintStroke::Tool tool)
{
    canvas.setCurrentTool(tool);
}

void PaintMaskAudioProcessorEditor::refreshStatus()
{
    const auto mode = processor.getTrialManager().getModeName();
    juce::String perf = processor.getPerformanceMode().isEnabled() ? " • Performance ON" : "";
    if (processor.getTrialManager().isStudioActive())
        statusLabel.setText("Mode: " + mode + " • Studio access active" + perf, juce::dontSendNotification);
    else if (processor.getTrialManager().isTrialActive())
        statusLabel.setText("Mode: " + mode + " • " + juce::String(processor.getTrialManager().getDaysRemaining()) + " trial days left" + perf, juce::dontSendNotification);
    else
        statusLabel.setText("Mode: " + mode + " • Lite fallback active" + perf, juce::dontSendNotification);

    layerInfoLabel.setText("Layers seeded: " + juce::String(processor.getLayerManager().getNumLayers()) + " • Mutations armed", juce::dontSendNotification);
}
