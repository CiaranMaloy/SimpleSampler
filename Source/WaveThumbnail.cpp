/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 3 Nov 2022 7:49:08am
    Author:  ciaran maloy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(HelloSamplerAudioProcessor& p) : audioProcessor (p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::cadetblue.darker());
    auto waveForm = audioProcessor.getWaveForm();
    
    if (waveForm.getNumSamples() > 0)
    {
        juce::Path p;
        mAudioPoints.clear();
        
        // 1. get the waveform from the processor
        auto ratio = waveForm.getNumSamples() / getWidth();
        auto buffer = waveForm.getReadPointer(0);
        
        // Scale on x axis
        for (int sample=0; sample < waveForm.getNumSamples(); sample+=ratio) // += ratio is useful
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        g.setColour(juce::Colours::yellow);
        p.startNewSubPath(0, getHeight()/2);
        // Scale on y axis
        for (int sample=0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.0f, 1.0f, getHeight(), 0); // this should be better organised
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(2));
        
        g.setColour(juce::Colours::white);
        g.setFont(15.0f);
        
        auto textBounds = getLocalBounds().reduced(10, 10); // inserts a margin
        
        g.drawFittedText(mFileName, textBounds, juce::Justification::topRight, 1);
        //mShouldBePainting = false;
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.setFont(40.0f);
        g.drawFittedText("Drop and Audio File to Load!", getLocalBounds(), juce::Justification::centred, 1);
    }
}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}


bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
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

void WaveThumbnail::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(files))
        {
            // creates a ile pointer that is on the stack and is destroyed when it goes out of scope
            auto myFile = std::make_unique<juce::File> (file);
            mFileName = myFile->getFileNameWithoutExtension(); // need member variable to store this in
            
            audioProcessor.loadFile(file); // i feel like there is a bug here where it'll read whatever files you give it so long as one of them is an audio file???
        }
    }
}
