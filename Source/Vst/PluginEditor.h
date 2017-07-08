/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include <vector>

#include "JuceLibraryCode/JuceHeader.h"
#include "Gui/HighLevelGui.h"
#include "Vst/PluginProcessor.h"


//==============================================================================
/**
*/
namespace AtomSynth
{
	class AtomGui;
}


class AtomSynthAudioProcessorEditor :
		public AudioProcessorEditor,
		public KeyListener
{
public:
    AtomSynthAudioProcessorEditor (AtomSynthAudioProcessor&);
    ~AtomSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    virtual bool keyPressed(const KeyPress & key, Component * originatingComponent);

private:
    AtomSynth::AtomSynthEditor m_editor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AtomSynthAudioProcessorEditor)
};

#endif  // PLUGINEDITOR_H_INCLUDED
