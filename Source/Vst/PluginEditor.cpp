/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "Vst/PluginEditor.h"

#include <iostream>

#include "Atoms/Basic/Output.h"
#include "Gui/Colours.h"
#include "Gui/Global.h"
#include "Technical/AudioBuffer.h"
#include "Technical/SaveState.h"
#include "Technical/Synth.h"
#include "Vst/PluginProcessor.h"

//==============================================================================
AtomSynthAudioProcessorEditor::AtomSynthAudioProcessorEditor (AtomSynthAudioProcessor& p)
    : AudioProcessorEditor (&p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (C::VST_WIDTH, C::VST_HEIGHT);
    addAndMakeVisible(m_editor);
    AtomSynth::Synth::s_instance->setRootComponent(& m_editor);
}

AtomSynthAudioProcessorEditor::~AtomSynthAudioProcessorEditor()
{
	AtomSynth::Synth::s_instance->closeRightClickMenu();
}

bool AtomSynthAudioProcessorEditor::keyPressed(const KeyPress& key, Component* originatingComponent)
{
	return false;
}

//==============================================================================
void AtomSynthAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(BACK_LAYER);
}

void AtomSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
