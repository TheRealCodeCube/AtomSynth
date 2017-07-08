/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include <vector>

#include "JuceLibraryCode/JuceHeader.h"
#define INCLUDE_ATOMS //Includes the builtin atoms during the compile process. Without this, there would be no atoms to use! :P
#include "Technical/Synth.h"

//==============================================================================
/**
*/

namespace AtomSynth
{
	class AtomController;
	class AtomControllerPreview;
	class Synth;
}

class AtomSynthAudioProcessor  : public AudioProcessor
{
public:
    double m_frequency;
    //std::vector<AtomSynth::AtomController *> m_atoms;
    //std::vector<int> m_atomExecutionOrder;
    //std::vector<AtomSynth::AtomControllerPreview> m_previews;
    File m_patchDirectory;

    //==============================================================================
    AtomSynthAudioProcessor();
    ~AtomSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    //void addAtom(AtomSynth::AtomController * controller);
    //void updateAtomExecutionOrder();

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AtomSynthAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
