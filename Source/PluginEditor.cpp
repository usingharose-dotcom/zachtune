#include "PluginEditor.h"

ZachTuneAudioProcessorEditor::ZachTuneAudioProcessorEditor(ZachTuneAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(720, 440);
    auto setupLabel = [this](juce::Label& label, const juce::String& text, int size, juce::Colour colour)
    {
        label.setText(text, juce::dontSendNotification);
        label.setFont(juce::Font((float) size, juce::Font::bold));
        label.setColour(juce::Label::textColourId, colour);
        addAndMakeVisible(label);
    };
    setupLabel(title, "ZACHTUNE", 30, juce::Colours::white);
    setupLabel(subtitle, "REAL-TIME VOCAL TUNING  /  VST3", 13, juce::Colour(0xffa8a8a8));
    setupLabel(status, "ENGINE PREVIEW  •  PITCH DSP NOT ENABLED", 12, juce::Colour(0xffff5555));

    auto setupSlider = [this](juce::Slider& s)
    {
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 22);
        s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffed202b));
        s.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff393939));
        s.setColour(juce::Slider::thumbColourId, juce::Colours::white);
        s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff393939));
        addAndMakeVisible(s);
    };
    setupSlider(retune); setupSlider(humanize); setupSlider(mix);
    setupLabel(retuneLabel, "RETUNE SPEED", 12, juce::Colours::white);
    setupLabel(humanizeLabel, "HUMANIZE", 12, juce::Colours::white);
    setupLabel(mixLabel, "MIX", 12, juce::Colours::white);

    key.addItemList({ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }, 1);
    scale.addItemList({ "Chromatic", "Major", "Minor" }, 1);
    for (auto* box : { &key, &scale })
    {
        box->setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff171717));
        box->setColour(juce::ComboBox::textColourId, juce::Colours::white);
        box->setColour(juce::ComboBox::outlineColourId, juce::Colour(0xffed202b));
        addAndMakeVisible(*box);
    }
    setupLabel(keyLabel, "KEY", 12, juce::Colours::white);
    setupLabel(scaleLabel, "SCALE", 12, juce::Colours::white);

    auto& state = audioProcessor.parameters;
    retuneAttachment = std::make_unique<SliderAttachment>(state, "retune", retune);
    humanizeAttachment = std::make_unique<SliderAttachment>(state, "humanize", humanize);
    mixAttachment = std::make_unique<SliderAttachment>(state, "mix", mix);
    keyAttachment = std::make_unique<ComboAttachment>(state, "key", key);
    scaleAttachment = std::make_unique<ComboAttachment>(state, "scale", scale);
}

void ZachTuneAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff090909));
    g.setColour(juce::Colour(0xffed202b));
    g.fillRect(0, 0, getWidth(), 5);
    g.setColour(juce::Colour(0xff202020));
    g.drawRoundedRectangle(18.0f, 18.0f, (float) getWidth() - 36.0f, (float) getHeight() - 36.0f, 12.0f, 1.0f);
    g.setColour(juce::Colour(0xffed202b));
    g.fillRoundedRectangle(34.0f, 105.0f, (float) getWidth() - 68.0f, 1.5f, 1.0f);
}

void ZachTuneAudioProcessorEditor::resized()
{
    title.setBounds(38, 27, 300, 42);
    subtitle.setBounds(40, 68, 360, 22);
    status.setBounds(40, 116, 500, 22);
    retuneLabel.setBounds(75, 164, 150, 24);
    humanizeLabel.setBounds(285, 164, 150, 24);
    mixLabel.setBounds(495, 164, 100, 24);
    retune.setBounds(55, 190, 180, 170);
    humanize.setBounds(265, 190, 180, 170);
    mix.setBounds(475, 190, 150, 170);
    keyLabel.setBounds(85, 372, 55, 20);
    key.setBounds(85, 394, 125, 26);
    scaleLabel.setBounds(260, 372, 70, 20);
    scale.setBounds(260, 394, 180, 26);
}
