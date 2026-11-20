#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout makeParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "retune", "Retune Speed", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 35.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "humanize", "Humanize", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 15.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "key", "Key", juce::StringArray { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "scale", "Scale", juce::StringArray { "Chromatic", "Major", "Minor" }, 2));
    return layout;
}

ZachTuneAudioProcessor::ZachTuneAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "ZachTuneState", makeParameterLayout())
{
}

void ZachTuneAudioProcessor::prepareToPlay(double, int) {}
void ZachTuneAudioProcessor::releaseResources() {}

void ZachTuneAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    // Safe initial VST3 build: audio remains transparent while the pitch-correction
    // DSP engine is developed. UI controls are saved with the project/session.
    for (int channel = getTotalNumInputChannels(); channel < getTotalNumOutputChannels(); ++channel)
        buffer.clear(channel, 0, buffer.getNumSamples());
}

juce::AudioProcessorEditor* ZachTuneAudioProcessor::createEditor()
{
    return new ZachTuneAudioProcessorEditor(*this);
}

void ZachTuneAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ZachTuneAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZachTuneAudioProcessor();
}
