/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessorEditor::HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), mWaveThumbnail (p), mADSR(p), audioProcessor (p)
{
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    mLoadButton.onClick = [&]() {
        // small function that is executed on click
        audioProcessor.loadFile();
    };
    
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);
    //addAndMakeVisible(mLoadButton);
    setSize (600, 400);
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
    
    
}

void HelloSamplerAudioProcessorEditor::resized()
{
    mWaveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    mADSR.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
        
}
