#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ZachTuneLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    ZachTuneLookAndFeel();
    void drawRotarySlider(juce::Graphics&, int, int, int, int, float, float, float, juce::Slider&) override;
};

class ZachTuneAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                            private juce::Timer
{
public:
    explicit ZachTuneAudioProcessorEditor(ZachTuneAudioProcessor&);
    ~ZachTuneAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    void timerCallback() override;
    void setupKnob(juce::Slider&, const juce::String&, bool rotary = true);
    void setupCombo(juce::ComboBox&);
    void setupButton(juce::TextButton&);
    void drawPanel(juce::Graphics&, juce::Rectangle<float>, const juce::String&);
    void resetParameters();

    ZachTuneAudioProcessor& processor;
    ZachTuneLookAndFeel lookAndFeel;
    juce::Slider retune, humanize, flexTune, vibrato, mix, formant, transpose, output;
    juce::ComboBox key, scale, correctionStyle, latency, preset;
    juce::TextButton autoTab { "Auto-Tune" }, graphTab { "Graph Mode" }, advancedTab { "Advanced" };
    juce::TextButton undoButton { "↶" }, redoButton { "↷" }, copyButton { "Copy" }, pasteButton { "Paste" }, initButton { "Init" };
    juce::TextButton modeAuto { "Auto" }, modeGraph { "Graph" }, abButton { "A | B" };
    juce::Label logo, status;
    std::unique_ptr<SliderAttachment> retuneA, humanizeA, flexA, vibratoA, mixA, formantA, transposeA, outputA;
    std::unique_ptr<ComboAttachment> keyA, scaleA, styleA, latencyA;
    juce::ValueTree copiedState, stateB;
    bool showingGraph = false, showingAdvanced = false, activeB = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZachTuneAudioProcessorEditor)
};
