/*
  ==============================================================================

    WaveThumbnail.h
    Created: 3 Nov 2022 7:49:08am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(HelloSamplerAudioProcessor& p);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray &files, int x, int y) override;

private:
    std::vector<float> mAudioPoints;
    bool mShouldBePainting {false};
    
    juce::String mFileName {""};
    
    HelloSamplerAudioProcessor& audioProcessor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
