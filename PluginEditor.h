/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidSidePLUGINAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidSidePLUGINAudioProcessorEditor (MidSidePLUGINAudioProcessor&);
    ~MidSidePLUGINAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidSidePLUGINAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidSidePLUGINAudioProcessorEditor)
};
