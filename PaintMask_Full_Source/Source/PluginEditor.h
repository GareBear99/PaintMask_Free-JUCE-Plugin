#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PaintCanvas.h"

class PaintMaskAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      private PaintCanvasComponent::Listener,
                                      private juce::Button::Listener,
                                      private juce::ComboBox::Listener,
                                      private juce::Slider::Listener,
                                      private juce::Timer
{
public:
    explicit PaintMaskAudioProcessorEditor(PaintMaskAudioProcessor&);
    ~PaintMaskAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PaintMaskAudioProcessor& processor;
    PaintCanvasComponent canvas;

    juce::TextButton brushButton { "Brush" }, splashButton { "Splash" }, lineButton { "Line" },
                     circleButton { "Circle" }, triangleButton { "Triangle" }, starButton { "Star" },
                     spiralButton { "Spiral" }, maskButton { "Mask" }, undoButton { "Undo" },
                     clearButton { "Clear" }, unlockButton { "Unlock Studio" },
                     mirrorXButton { "Mirror X" }, mirrorYButton { "Mirror Y" },
                     rotateButton { "Rotate 90" }, densityLoButton { "Thin" }, densityHiButton { "Dense" },
                     performanceButton { "Performance" };
    juce::ComboBox colourBox, scanModeBox;
    juce::Slider thicknessSlider, previewGainSlider;
    juce::Label titleLabel, statusLabel, subtitleLabel, layerInfoLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> scanModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> previewGainAttachment;

    void documentChanged() override;
    void buttonClicked(juce::Button*) override;
    void comboBoxChanged(juce::ComboBox*) override;
    void sliderValueChanged(juce::Slider*) override;
    void timerCallback() override;

    void setTool(PaintStroke::Tool);
    void refreshStatus();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PaintMaskAudioProcessorEditor)
};
