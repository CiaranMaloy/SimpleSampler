/*
  ==============================================================================

    ADSRComponent.h
    Created: 3 Nov 2022 8:58:25am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(HelloSamplerAudioProcessor& p);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider; // m because they're private globals of the class
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mReleaseAttachment;
    
    HelloSamplerAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
