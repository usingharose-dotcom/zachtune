#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout makeParameterLayout()
{
    using Float = juce::AudioParameterFloat;
    using Choice = juce::AudioParameterChoice;
    juce::AudioProcessorValueTreeState::ParameterLayout p;
    p.add(std::make_unique<Float>("retune", "Retune Speed", juce::NormalisableRange<float>(0.0f, 500.0f, 0.1f), 20.0f));
    p.add(std::make_unique<Float>("humanize", "Humanize", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    p.add(std::make_unique<Float>("flex", "Flex Tune", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));
    p.add(std::make_unique<Float>("vibrato", "Natural Vibrato", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    p.add(std::make_unique<Float>("mix", "Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));
    p.add(std::make_unique<Float>("formant", "Formant", juce::NormalisableRange<float>(0.0f, 200.0f, 0.1f), 100.0f));
    p.add(std::make_unique<Float>("transpose", "Transpose", juce::NormalisableRange<float>(-12.0f, 12.0f, 1.0f), 0.0f));
    p.add(std::make_unique<Float>("output", "Output", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    p.add(std::make_unique<Choice>("key", "Key", juce::StringArray { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }, 0));
    p.add(std::make_unique<Choice>("scale", "Scale", juce::StringArray { "Major", "Minor", "Chromatic", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Locrian", "Harmonic Minor", "Melodic Minor", "Pentatonic Major", "Pentatonic Minor" }, 1));
    p.add(std::make_unique<Choice>("mode", "Mode", juce::StringArray { "Auto", "Graph" }, 0));
    p.add(std::make_unique<Choice>("style", "Correction Style", juce::StringArray { "Classic", "Modern", "Subtle", "Hard Tune" }, 0));
    p.add(std::make_unique<Choice>("latency", "Latency", juce::StringArray { "Low", "Normal", "High" }, 0));
    return p;
}

ZachTuneAudioProcessor::ZachTuneAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "ZachTuneState", makeParameterLayout())
{
}

void ZachTuneAudioProcessor::prepareToPlay(double, int) { outputPeak.store(0.0f); }
void ZachTuneAudioProcessor::releaseResources() {}

void ZachTuneAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int inputChannels = getTotalNumInputChannels();
    const int outputChannels = getTotalNumOutputChannels();
    for (int ch = inputChannels; ch < outputChannels; ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());

    const float outDb = parameters.getRawParameterValue("output")->load();
    const float outputGain = juce::Decibels::decibelsToGain(outDb);
    float peak = 0.0f;
    for (int ch = 0; ch < inputChannels; ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            data[i] *= outputGain;
            peak = juce::jmax(peak, std::abs(data[i]));
        }
    }
    outputPeak.store(juce::jlimit(0.0f, 1.0f, peak));
    // Current engine: transparent audio pass-through with working output gain and metering.
    // Pitch detection/correction is intentionally not faked; it is the next DSP milestone.
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
