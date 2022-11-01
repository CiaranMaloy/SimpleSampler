/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessorEditor::HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    mLoadButton.onClick = [&]() {
        // small function that is executed on click
        audioProcessor.loadFile();
    };
    
    // set the slider look and feel
    // these should all be functions really ---- 
    // Attack slider
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mAttackSlider.setRange(0.0f, 5.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    mAttackSlider.addListener(this); // this refers to the editor and makes it a listener
    addAndMakeVisible(mAttackSlider);
    
    mAttackLabel.setFont(10.f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    // Decay slider
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::green);
    mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    
    // Sustain
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mSustainSlider.setRange(0.0f, 1.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blue);
    mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
    // Release
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    mReleaseSlider.setRange(0.0f, 1.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::yellow);
    mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    
    //addAndMakeVisible(mLoadButton);
    setSize (600, 200);
}

HelloSamplerAudioProcessorEditor::~HelloSamplerAudioProcessorEditor()
{
}

//==============================================================================
void HelloSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::black);
//
    g.setColour(juce::Colours::white);
    
    
    if (mShouldBePainting)
    {
        juce::Path p;
        mAudioPoints.clear();
        // New file dropped?
        //  if yes
        // get the waveform from the processor
        // sample = 44100 (1 sec) ... x axis of window = 600 == sampleLength / getWidth()
        // valus of audiofile will be going between -1 and +1 ...
        // but the size of the Y axis is 200 so we need to scale that up. -1 to 1 -> 0 to 200 (N)
        // use the ratio to take vaues from the audio buffer and put in vector to display
        // draw the waveform
        
        // if no
        // do nothing I guess
        
        // 1. get the waveform from the processor
        auto waveForm = audioProcessor.getWaveForm();
        auto ratio = waveForm.getNumSamples() / getWidth();
        auto buffer = waveForm.getReadPointer(0);
        
        // Scale on x axis
        for (int sample=0; sample < waveForm.getNumSamples(); sample+=ratio) // += ratio is useful
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight()/2);
        // Scale on y axis
        for (int sample=0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, 200, 0); // this should be better organised
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(2));
        mShouldBePainting = false;
    }
//    // if we have a sound
//        // "sound loaded"
//    // else
//        // "load a sound"
//    if (audioProcessor.getNumSamplerSounds() > 0)
//    {
//        g.fillAll(juce::Colours::red);
//        g.setColour(juce::Colours::white);
//        g.setFont(15.0f);
//        g.drawText("Sound Loaded", getWidth()/2-50, getHeight()/2-10, 100, 20, juce::Justification::centred);
//    }
//    else{
//        g.setColour(juce::Colours::white);
//        g.drawText("Load a Sound", getWidth()/2-50, getHeight()/2-10, 100, 20, juce::Justification::centred);
//    }
}

void HelloSamplerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //mLoadButton.setBounds(getWidth()/2-50, getHeight()/2-50, 100, 100);
    const auto startX = 0.6f;
    const auto startY = 0.5f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;
    
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + dialWidth*2.0f, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + dialWidth*3.0f, startY, dialWidth, dialHeight);
    
}

bool HelloSamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aiff") || file.contains(".aif"))
        {
            return true;
        }
    }
    
    return false;
}

void HelloSamplerAudioProcessorEditor::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            mShouldBePainting = true;
            audioProcessor.loadFile(file); // i feel like there is a bug here where it'll read whatever files you give it so long as one of them is an audio file???
        }
    }
}

void HelloSamplerAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    // you want to replace the slider listener stuff with an valueTreeState object 
    if (slider == &mAttackSlider)
    {
        audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.getADSRParams().decay = mDecaySlider.getValue();
    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();
    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();
    }
    
    audioProcessor.updateADSR();
}
