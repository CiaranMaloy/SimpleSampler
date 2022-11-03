/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class HelloSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HelloSamplerAudioProcessorEditor (HelloSamplerAudioProcessor&);
    ~HelloSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==========
    
    //void sliderValueChanged(juce::Slider* slider) override; dont need this

private:
    juce::TextButton mLoadButton{"Load"};
    
    WaveThumbnail mWaveThumbnail;
    ADSRComponent mADSR;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HelloSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloSamplerAudioProcessorEditor)
};
