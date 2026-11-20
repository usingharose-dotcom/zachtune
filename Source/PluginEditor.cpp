#include "PluginEditor.h"
#include <cmath>
namespace { const juce::Colour bg(0xff09090b),panel(0xff101013),edge(0xff29292f),red(0xfff02032),white(0xfff4f4f5),muted(0xff9b9ba2); }
ZachTuneLookAndFeel::ZachTuneLookAndFeel(){
 setColour(juce::Slider::textBoxTextColourId,white); setColour(juce::Slider::textBoxBackgroundColourId,juce::Colour(0xff08080a)); setColour(juce::Slider::textBoxOutlineColourId,edge);
 setColour(juce::ComboBox::backgroundColourId,juce::Colour(0xff08080a)); setColour(juce::ComboBox::textColourId,white); setColour(juce::ComboBox::outlineColourId,edge); setColour(juce::ComboBox::arrowColourId,muted);
 setColour(juce::TextButton::buttonColourId,juce::Colour(0xff141418)); setColour(juce::TextButton::buttonOnColourId,red); setColour(juce::TextButton::textColourOffId,muted); setColour(juce::TextButton::textColourOnId,white);
}
void ZachTuneLookAndFeel::drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float p,float start,float end,juce::Slider&){
 float s=(float)juce::jmin(w,h)-12,cx=x+w*.5f,cy=y+h*.5f,r=s*.5f; auto b=juce::Rectangle<float>(cx-r,cy-r,s,s);
 g.setColour(juce::Colour(0xff39393f));g.drawArc(b,start,end,true,7);g.setColour(red);g.drawArc(b,start,start+(end-start)*p,true,7);
 g.setColour(juce::Colour(0xff19191e));g.fillEllipse(b.reduced(11));g.setColour(juce::Colour(0xff44444b));g.drawEllipse(b.reduced(11),1.5f);
 float a=start+(end-start)*p;g.setColour(white);g.fillEllipse(cx+std::cos(a)*(r-2)-6,cy+std::sin(a)*(r-2)-6,12,12);
}
ZachTuneAudioProcessorEditor::ZachTuneAudioProcessorEditor(ZachTuneAudioProcessor& p):AudioProcessorEditor(&p),processor(p){
 setLookAndFeel(&lf);setSize(1440,810);setResizable(true,true);setResizeLimits(1000,563,1920,1080);
 logo.setText("ZachTune",juce::dontSendNotification);logo.setFont(juce::Font(38,juce::Font::bold));logo.setColour(juce::Label::textColourId,white);addAndMakeVisible(logo);
 subtitle.setText("PRO  /  VOCAL TUNING",juce::dontSendNotification);subtitle.setFont(juce::Font(10,juce::Font::bold));subtitle.setColour(juce::Label::textColourId,muted);addAndMakeVisible(subtitle);
 for(auto* s:{&retune,&humanize,&flexTune,&vibrato,&mix,&formant,&transpose,&output})slider(*s);
 retune.setTextValueSuffix(" ms");humanize.setTextValueSuffix(" %");flexTune.setTextValueSuffix(" %");mix.setTextValueSuffix(" %");formant.setTextValueSuffix(" %");output.setTextValueSuffix(" dB");
 key.addItemList({"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"},1);
 scale.addItemList({"Major","Minor","Chromatic","Dorian","Phrygian","Lydian","Mixolydian","Locrian","Harmonic Minor","Melodic Minor","Pentatonic Major","Pentatonic Minor"},1);
 style.addItemList({"Classic","Modern","Subtle","Hard Tune"},1);latency.addItemList({"Low","Normal","High"},1);preset.addItemList({"Default Preset","Natural Vocal","Modern Pop","Hard Tune"},1);
 for(auto* c:{&key,&scale,&style,&latency,&preset}){combo(*c);addAndMakeVisible(*c);}
 for(auto* b:{&autoTab,&graphTab,&advancedTab,&undo,&redo,&copy,&paste,&init,&ab,&modeAuto,&modeGraph}){button(*b);addAndMakeVisible(*b);}
 autoTab.setToggleState(true,juce::dontSendNotification);autoTab.onClick=[this]{tab(0);};graphTab.onClick=[this]{tab(1);};advancedTab.onClick=[this]{tab(2);};
 init.onClick=[this]{reset();};copy.onClick=[this]{copied=processor.parameters.copyState();};paste.onClick=[this]{if(copied.isValid())processor.parameters.replaceState(copied.createCopy());};
 ab.onClick=[this]{if(!useB){slotA=processor.parameters.copyState();if(slotB.isValid())processor.parameters.replaceState(slotB.createCopy());}else{slotB=processor.parameters.copyState();if(slotA.isValid())processor.parameters.replaceState(slotA.createCopy());}useB=!useB;};
 undo.onClick=[this]{if(slotA.isValid())processor.parameters.replaceState(slotA.createCopy());};redo.onClick=[this]{if(slotB.isValid())processor.parameters.replaceState(slotB.createCopy());};
 auto& v=processor.parameters;
 aRetune=std::make_unique<SA>(v,"retune",retune);aHuman=std::make_unique<SA>(v,"humanize",humanize);aFlex=std::make_unique<SA>(v,"flex",flexTune);aVibrato=std::make_unique<SA>(v,"vibrato",vibrato);
 aMix=std::make_unique<SA>(v,"mix",mix);aFormant=std::make_unique<SA>(v,"formant",formant);aTranspose=std::make_unique<SA>(v,"transpose",transpose);aOutput=std::make_unique<SA>(v,"output",output);
 aKey=std::make_unique<CA>(v,"key",key);aScale=std::make_unique<CA>(v,"scale",scale);aStyle=std::make_unique<CA>(v,"style",style);aLatency=std::make_unique<CA>(v,"latency",latency);startTimerHz(24);
}
ZachTuneAudioProcessorEditor::~ZachTuneAudioProcessorEditor(){setLookAndFeel(nullptr);}
void ZachTuneAudioProcessorEditor::slider(juce::Slider& s){s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);s.setTextBoxStyle(juce::Slider::TextBoxBelow,false,94,27);s.setRotaryParameters(juce::MathConstants<float>::pi*1.25f,juce::MathConstants<float>::pi*2.75f,true);addAndMakeVisible(s);}
void ZachTuneAudioProcessorEditor::combo(juce::ComboBox& c){c.setJustificationType(juce::Justification::centredLeft);c.setColour(juce::ComboBox::backgroundColourId,juce::Colour(0xff08080a));c.setColour(juce::ComboBox::textColourId,white);c.setColour(juce::ComboBox::outlineColourId,edge);c.setColour(juce::ComboBox::arrowColourId,muted);}
void ZachTuneAudioProcessorEditor::button(juce::TextButton& b){b.setColour(juce::TextButton::buttonColourId,juce::Colour(0xff141418));b.setColour(juce::TextButton::buttonOnColourId,red);b.setColour(juce::TextButton::textColourOffId,muted);b.setColour(juce::TextButton::textColourOnId,white);}
void ZachTuneAudioProcessorEditor::tab(int t){activeTab=t;autoTab.setToggleState(t==0,juce::dontSendNotification);graphTab.setToggleState(t==1,juce::dontSendNotification);advancedTab.setToggleState(t==2,juce::dontSendNotification);repaint();}
void ZachTuneAudioProcessorEditor::reset(){const std::pair<const char*,float>d[]={{"retune",20},{"humanize",50},{"flex",0},{"vibrato",0},{"mix",100},{"formant",100},{"transpose",0},{"output",0}};for(auto& x:d)if(auto* q=processor.parameters.getParameter(x.first))q->setValueNotifyingHost(q->convertTo0to1(x.second));processor.parameters.getParameter("key")->setValueNotifyingHost(0);processor.parameters.getParameter("scale")->setValueNotifyingHost(0);}
void ZachTuneAudioProcessorEditor::place(juce::Component& c,float x,float y,float w,float h){c.setBounds(juce::roundToInt(x*getWidth()/1440.f),juce::roundToInt(y*getHeight()/810.f),juce::roundToInt(w*getWidth()/1440.f),juce::roundToInt(h*getHeight()/810.f));}
void ZachTuneAudioProcessorEditor::resized(){
 place(logo,42,23,260,48);place(subtitle,45,67,220,17);place(undo,390,30,46,44);place(redo,438,30,46,44);place(preset,500,30,280,45);place(ab,808,30,92,45);place(copy,925,30,85,45);place(paste,1010,30,92,45);place(init,1120,30,92,45);
 place(autoTab,22,110,143,42);place(graphTab,174,110,149,42);place(advancedTab,332,110,138,42);place(retune,42,193,140,166);place(humanize,222,193,140,166);place(flexTune,42,422,140,144);place(vibrato,222,422,140,144);
 place(key,452,157,176,42);place(scale,833,157,179,42);place(mix,1130,210,225,300);place(output,1188,505,135,55);place(modeAuto,438,752,82,30);place(modeGraph,508,752,88,30);place(formant,622,744,112,48);place(style,750,751,205,31);place(transpose,1028,745,130,48);place(latency,1238,751,150,31);
}
void ZachTuneAudioProcessorEditor::paint(juce::Graphics& g){
 g.fillAll(bg);g.addTransform(juce::AffineTransform::scale(getWidth()/1440.f,getHeight()/810.f));
 auto panelBox=[&](float x,float y,float w,float h){g.setColour(panel);g.fillRoundedRectangle(x,y,w,h,13);g.setColour(edge);g.drawRoundedRectangle(x,y,w,h,13,1);};
 auto text=[&](juce::String s,int x,int y,int w,int h,int sz,juce::Colour c){g.setColour(c);g.setFont(juce::Font((float)sz,juce::Font::bold));g.drawText(s,x,y,w,h,juce::Justification::centredLeft,false);};
 g.setColour(red);g.fillRect(10,10,1420,3);g.setColour(juce::Colour(0xff111114));g.fillRoundedRectangle(18,18,1404,73,12);g.setColour(edge);g.drawRoundedRectangle(18,18,1404,73,12,1);
 panelBox(22,162,369,548);panelBox(405,110,653,600);panelBox(1073,110,345,600);panelBox(405,720,1013,70);
 text("RETUNE SPEED",34,180,145,20,13,white);text("HUMANIZE",214,180,145,20,13,white);text("FLEX TUNE",34,405,145,20,13,white);text("NATURAL VIBRATO",214,405,160,20,13,white);
 text("KEY",452,134,100,20,13,muted);text("SCALE",833,134,100,20,13,muted);text("MIX",1180,137,100,24,17,white);text("OUTPUT",1350,137,65,20,12,muted);
 text("MODE",435,731,80,18,11,muted);text("FORMANT",595,731,100,18,11,muted);text("CORRECTION STYLE",750,731,180,18,11,muted);text("TRANSPOSE",1005,731,120,18,11,muted);text("LATENCY",1215,731,100,18,11,muted);
 float cx=731,cy=405,r=177;g.setColour(juce::Colour(0xff070709));g.fillEllipse(cx-r-8,cy-r-8,(r+8)*2,(r+8)*2);g.setColour(juce::Colour(0xff292930));g.drawEllipse(cx-r-8,cy-r-8,(r+8)*2,(r+8)*2,5);g.setColour(juce::Colour(0xff111115));g.fillEllipse(cx-r,cy-r,r*2,r*2);g.setColour(juce::Colour(0xff4b252b));g.drawEllipse(cx-r,cy-r,r*2,r*2,2);
 for(int i=0;i<72;++i){float a=juce::degreesToRadians(-135.f+i*270.f/71.f);g.setColour(i>=16&&i<=25?red:juce::Colour(0xff44444b));g.drawLine(cx+std::cos(a)*(r-16),cy+std::sin(a)*(r-16),cx+std::cos(a)*(r-4),cy+std::sin(a)*(r-4),2);}
 g.setColour(juce::Colour(0xff08080a));g.fillEllipse(cx-125,cy-125,250,250);g.setColour(edge);g.drawEllipse(cx-125,cy-125,250,250,2);text("C",675,344,112,82,68,red);text("MINOR",680,427,110,27,19,white);text("PITCH TARGET",655,457,150,18,10,muted);
 g.setColour(juce::Colour(0xff08080a));g.fillRoundedRectangle(452,610,560,67,9);g.setColour(edge);g.drawRoundedRectangle(452,610,560,67,9,1);for(int i=0;i<81;++i){float x=466+i*6.55f,hh=12+16*(1-std::abs(i-40)/40.f);g.setColour(i>=28&&i<=52?red:juce::Colour(0xff34343a));g.drawLine(x,649-hh*.5f,x,649+hh*.5f,2);}g.setColour(white);g.drawLine(731,621,731,666,3);text("-50¢",470,654,50,18,11,muted);text("0¢",712,654,40,18,11,white);text("+50¢",963,654,50,18,11,muted);
 g.setColour(juce::Colour(0xff050506));g.fillRoundedRectangle(1360,195,34,355,5);g.setColour(edge);g.drawRoundedRectangle(1360,195,34,355,5,1);float pk=juce::jlimit(0.f,1.f,processor.getOutputPeak());g.setColour(red);g.fillRoundedRectangle(1368,543-pk*215,18,pk*215,3);
 const juce::String marks[]={"+12","+6","0","-6","-12"};for(int i=0;i<5;++i)text(marks[i],1390,188+i*88,40,18,10,muted);
 if(activeTab==1)text("GRAPH MODE  •  PITCH EDITOR PREVIEW",520,570,420,24,13,red);else if(activeTab==2)text("ADVANCED CONTROLS",520,570,420,24,13,red);
}
void ZachTuneAudioProcessorEditor::timerCallback(){repaint();}
