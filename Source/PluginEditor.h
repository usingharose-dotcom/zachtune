#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class ZachTuneLookAndFeel final : public juce::LookAndFeel_V4 {
public:
 ZachTuneLookAndFeel();
 void drawRotarySlider(juce::Graphics&,int,int,int,int,float,float,float,juce::Slider&) override;
};
class ZachTuneAudioProcessorEditor final : public juce::AudioProcessorEditor, private juce::Timer {
public:
 explicit ZachTuneAudioProcessorEditor(ZachTuneAudioProcessor&);
 ~ZachTuneAudioProcessorEditor() override;
 void paint(juce::Graphics&) override;
 void resized() override;
private:
 using SA=juce::AudioProcessorValueTreeState::SliderAttachment;
 using CA=juce::AudioProcessorValueTreeState::ComboBoxAttachment;
 void timerCallback() override;
 void slider(juce::Slider&); void combo(juce::ComboBox&); void button(juce::TextButton&);
 void place(juce::Component&,float,float,float,float); void reset(); void tab(int);
 ZachTuneAudioProcessor& processor; ZachTuneLookAndFeel lf;
 juce::Slider retune,humanize,flexTune,vibrato,mix,formant,transpose,output;
 juce::ComboBox key,scale,style,latency,preset;
 juce::TextButton autoTab{"AUTO-TUNE"},graphTab{"GRAPH MODE"},advancedTab{"ADVANCED"};
 juce::TextButton undo{"↶"},redo{"↷"},copy{"Copy"},paste{"Paste"},init{"Init"},ab{"A | B"},modeAuto{"AUTO"},modeGraph{"GRAPH"};
 juce::Label logo,subtitle;
 std::unique_ptr<SA> aRetune,aHuman,aFlex,aVibrato,aMix,aFormant,aTranspose,aOutput;
 std::unique_ptr<CA> aKey,aScale,aStyle,aLatency;
 juce::ValueTree copied,slotA,slotB; int activeTab=0; bool useB=false;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZachTuneAudioProcessorEditor)
};