/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class HelloSamplerAudioProcessor  : public juce::AudioProcessor, public juce::ValueTree::Listener
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HelloSamplerAudioProcessor();
    ~HelloSamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //========
    void loadFile();
    void loadFile(const juce::String& pathToFile);
    
    int getNumSamplerSounds() {return mSampler.getNumSounds();}
    
    // mWaveForm is private
    juce::AudioBuffer<float>& getWaveForm() { return mWaveForm; }
    
    // adsr (primitive)
    void updateADSR();
    //float attack{0.0}, decay{0.0}, sustain{0.0}, release{0.0};
    juce::ADSR::Parameters& getADSRParams() { return mADSRParams;}
    juce::AudioProcessorValueTreeState& getAPVTS() {return mAPVTS;}

private:
    
    juce::Synthesiser mSampler;
    const int mNumVoices {3};
    juce::AudioBuffer<float> mWaveForm;
    
    juce::ADSR::Parameters mADSRParams;
    juce::AudioProcessorValueTreeState mAPVTS;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
    
    // "an atomic variable is a way a type of protection that we can use in an audio callback where if we try to change or if we are trying to read if m should change but there could be a thread conflict then the atomic variable is protection against the thread conflict, you should read up on that"
    // - Josh Hodge
    std::atomic<bool> mShouldUpdate {false};
    
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader {nullptr};
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelloSamplerAudioProcessor)
};
