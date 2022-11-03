/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HelloSamplerAudioProcessor::HelloSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), mAPVTS(*this, nullptr, "PARAMETERS", createParameters())
#endif
{
    mFormatManager.registerBasicFormats();
    mAPVTS.state.addListener(this); // add a listener to the value tree inside 'this' processor
    
    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }
}

HelloSamplerAudioProcessor::~HelloSamplerAudioProcessor()
{
}

//==============================================================================
const juce::String HelloSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HelloSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HelloSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HelloSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HelloSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HelloSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HelloSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HelloSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HelloSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HelloSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    
    updateADSR();
}

void HelloSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HelloSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HelloSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //updateADSR(); - I dont think I need to do this now that it's called whenever the listener class is activated

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (mShouldUpdate)
    {
        updateADSR();
        //mShouldUpdate = false;
    }
    
    // 1. how do I know that there is a sound being played at all
    // an earlier tutorial, a simple midi plugin
    juce::MidiMessage m;
    // midi buffer iterator
    //::MidiBuffer::Iterator it { midiMessages };

    for (const auto meta : midiMessages) // I'll be impressed if this works
    {
        m = meta.getMessage();
        if (m.isNoteOn())
        {
            // hey note is on, start the playhead
            mIsNotePlayed = true;
        }
        else if (m.isNoteOff())
        {
            // stop the playhead
            mIsNotePlayed = false;
        }
    }
    
    //DBG("Note : " << mIsNotePlayed);

    mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool HelloSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HelloSamplerAudioProcessor::createEditor()
{
    return new HelloSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void HelloSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HelloSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//=========== // part of this should be called during the drag and drop
void HelloSamplerAudioProcessor::loadFile()
{
    juce::FileChooser chooser("Load File");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult(); // should be an audio file
        mFormatReader = mFormatManager.createReaderFor(file);
    }
    
    // add the sound to the sampler
    juce::BigInteger range;
    range.setRange(0, 128, true);
    double attack = 0.1;
    double release = 0.1;
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, attack, release, 10.0));
    updateADSR();
}

void HelloSamplerAudioProcessor::loadFile(const juce::String& pathToFile)
{
    // if there is already a sond in the sampler then clear it
    mSampler.clearSounds();
    
    auto file = juce::File(pathToFile);
    mFormatReader = mFormatManager.createReaderFor(file);
    
    //
    // get the waveform from the sample
    int sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    mWaveForm.setSize(1, sampleLength);
    mFormatReader->read(&mWaveForm, 0, sampleLength, 0, true, false);
    
//    // check that the waveform has been written properly
//    auto buffer = mWaveForm.getReadPointer(0);
//    for (int sample=0; sample < mWaveForm.getNumSamples(); ++sample)
//    {
//        DBG(buffer[sample]);
//    }
    
    //
    juce::BigInteger range;
    range.setRange(0, 128, true);
    double attack = 0.1;
    double release = 0.1;
    mSampler.addSound(new juce::SamplerSound ("Sample", *mFormatReader, range, 60, attack, release, 10.0));
    updateADSR();
}

void HelloSamplerAudioProcessor::updateADSR()
{
    mADSRParams.attack = mAPVTS.getRawParameterValue("ATTACK")->load(); // load is what actually gets the value
    mADSRParams.decay = mAPVTS.getRawParameterValue("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue("RELEASE")->load();
    
    for (int i=0; i<mSampler.getNumSounds(); ++i)
    {
        // chec that these are sampler sounds not synthesisersounds
        // using dynamic casting
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get())) // the get method gets us the pointer sice it is of type SamplerSound pointer, and
        {
            // Set Envelope Parameters
            sound->setEnvelopeParameters(mADSRParams); // no adsr parameters object
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout HelloSamplerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters; // jesus christ this is a lot of ypes
    
    // now what we need to do is create our parameters that we're going to get from the slider values
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 3.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 2.0f));
    
    return {parameters.begin(), parameters.end()};
}

// if anything changes in the value tree then we want to update
void HelloSamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
{
    mShouldUpdate = true;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelloSamplerAudioProcessor();
}
