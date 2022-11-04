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
    // acess image
    auto exampleImage = juce::ImageCache::getFromMemory(BinaryData::exampleImage_png, BinaryData::exampleImage_pngSize);
    
    // put this on the screen using the image component
    if (!exampleImage.isNull())
    {
        mImageComponent.setImage(exampleImage, juce::RectanglePlacement::stretchToFit);
    }
    else
    {
        jassert(! exampleImage.isNull());
    }
    
    startTimerHz(30);
    addAndMakeVisible(mWaveThumbnail);
    addAndMakeVisible(mADSR);
    addAndMakeVisible(mImageComponent);
    //addAndMakeVisible(mLoadButton);
    setSize (600, 400);
}

HelloSamplerAudioProcessorEditor::~HelloSamplerAudioProcessorEditor()
{
    stopTimer();
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
    mImageComponent.setBoundsRelative(0.0f, 0.0f, 0.25f, 0.25f);
        
}

void HelloSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}
