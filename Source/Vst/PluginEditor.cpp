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
AtomSynthAudioProcessorEditor::AtomSynthAudioProcessorEditor(AtomSynthAudioProcessor& p) :
		AudioProcessorEditor(&p) {
	setSize(C::VST_WIDTH, C::VST_HEIGHT);
	addAndMakeVisible(m_editor);
	AtomSynth::Synth::getInstance()->getGuiManager().setRootComponent(&m_editor);
}

AtomSynthAudioProcessorEditor::~AtomSynthAudioProcessorEditor() {
	AtomSynth::Synth::getInstance()->getGuiManager().closeRightClickMenu();
}

bool AtomSynthAudioProcessorEditor::keyPressed(const KeyPress& key, Component* originatingComponent) {
	return false;
}

//==============================================================================
void AtomSynthAudioProcessorEditor::paint(Graphics& g) {
	g.fillAll(BACK_LAYER);
}

void AtomSynthAudioProcessorEditor::resized() {
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
}
