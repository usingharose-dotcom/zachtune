#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ZachTuneAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ZachTuneAudioProcessorEditor(ZachTuneAudioProcessor&);
    ~ZachTuneAudioProcessorEditor() override = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ZachTuneAudioProcessor& audioProcessor;
    juce::Slider retune, humanize, mix;
    juce::ComboBox key, scale;
    juce::Label title, subtitle, status, retuneLabel, humanizeLabel, mixLabel, keyLabel, scaleLabel;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    std::unique_ptr<SliderAttachment> retuneAttachment, humanizeAttachment, mixAttachment;
    std::unique_ptr<ComboAttachment> keyAttachment, scaleAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZachTuneAudioProcessorEditor)
};
