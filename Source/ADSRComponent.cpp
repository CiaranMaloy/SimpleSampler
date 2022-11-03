/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 3 Nov 2022 8:58:25am
    Author:  ciaran maloy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(HelloSamplerAudioProcessor& p) : audioProcessor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    // set the slider look and feel
    // these should all be functions really ----
    // Attack slider
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    //mAttackSlider.setRange(0.0f, 5.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    //mAttackSlider.addListener(this); // this refers to the editor and makes it a listener
    addAndMakeVisible(mAttackSlider);
    
    mAttackLabel.setFont(10.f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "ATTACK", mAttackSlider);
    
    // Decay slider
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    //mDecaySlider.setRange(0.0f, 5.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mDecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::green);
    //mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    
    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DECAY", mDecaySlider);
    
    // Sustain
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    //mSustainSlider.setRange(0.0f, 1.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mSustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blue);
    //mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "SUSTAIN", mSustainSlider);
    
    // Release
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    //mReleaseSlider.setRange(0.0f, 1.0f, 0.01f); // alwasy set the increment because otherwise things get weird
    mReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::yellow);
    //mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    
    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "RELEASE", mReleaseSlider);

}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ADSRComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ADSRComponent::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.5f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;
    
    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + dialWidth*2.0f, startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + dialWidth*3.0f, startY, dialWidth, dialHeight);
}
