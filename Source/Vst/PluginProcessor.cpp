/*
 ==============================================================================

 This file was auto-generated!

 It contains the basic framework code for a JUCE plugin processor.

 ==============================================================================
 */

#include "Vst/PluginProcessor.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <fenv.h> //I'm getting NaN results that are hard to find.

#include "JuceLibraryCode/JuceHeader.h"
#include "Atoms/Atom.h"
#include "Gui/Global.h"
#include "Technical/AudioBuffer.h"
#include "Technical/SaveState.h"
#include "Technical/Synth.h"
#include "Vst/PluginEditor.h"

//==============================================================================
AtomSynthAudioProcessor::AtomSynthAudioProcessor() :
		m_frequency(100.0f) {
	C::loadFont();
	//feenableexcept(FE_INVALID|FE_OVERFLOW); //I'm getting NaN results that are hard to find.
	AtomSynth::Synth::getInstance()->initialize(441000, 256); //Initialize with dummy values, so that if any other code that has to run after initialization is called before we know the real values, it will still work.
}

AtomSynthAudioProcessor::~AtomSynthAudioProcessor() {
	AtomSynth::Synth::deleteInstance();
}

//==============================================================================
const String AtomSynthAudioProcessor::getName() const {
	return JucePlugin_Name;
}

bool AtomSynthAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool AtomSynthAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

double AtomSynthAudioProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int AtomSynthAudioProcessor::getNumPrograms() {
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int AtomSynthAudioProcessor::getCurrentProgram() {
	return 0;
}

void AtomSynthAudioProcessor::setCurrentProgram(int index) {
}

const String AtomSynthAudioProcessor::getProgramName(int index) {
	return String(AtomSynth::Synth::getInstance()->getSaveManager().getPatchName());
}

void AtomSynthAudioProcessor::changeProgramName(int index, const String& newName) {
	AtomSynth::Synth::getInstance()->getSaveManager().setPatchName(newName.toStdString());
}

//==============================================================================
void AtomSynthAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	AtomSynth::Synth::initialize(sampleRate, samplesPerBlock);
}

void AtomSynthAudioProcessor::releaseResources() {
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AtomSynthAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
	// Reject any bus arrangements that are not compatible with your plugin

	const int numChannels = preferredSet.size();

#if JucePlugin_IsMidiEffect
	if (numChannels != 0)
	return false;
#elif JucePlugin_IsSynth
	if (isInput || (numChannels != 1 && numChannels != 2))
	return false;
#else
	if (numChannels != 1 && numChannels != 2)
	return false;

	if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
	return false;
#endif

	return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void AtomSynthAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
	const int totalNumInputChannels = getTotalNumInputChannels();
	const int totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples();
	AtomSynth::Synth::getInstance()->getParameters().m_timestamp += numSamples;

//#define LMMS_COMPATIBILITY
#ifdef LMMS_COMPATIBILITY //LMMS does weird things, the prepareToPlay function says 1024 samples / block, but actually gives 256 samples / block.
	if (numSamples != AtomSynth::AudioBuffer::getDefaultSize())
		prepareToPlay(AtomSynth::GlobalNoteStates::s_sampleRate, numSamples);
#endif /* LMMS_COMPATIBILITY */

	int time;
	MidiMessage message;
	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(message, time);) {
		if (message.isNoteOn()) {
			double frequency = pow(2.0, (double(message.getNoteNumber()) - 69.0) / 12.0) * 440.0;
			AtomSynth::Synth::getInstance()->getNoteManager().addMidiNote(message);
		} else if (message.isNoteOff()) {
			double frequency = pow(2.0, (double(message.getNoteNumber()) - 69.0) / 12.0) * 440.0;
			AtomSynth::Synth::getInstance()->getNoteManager().removeMidiNote(message);
		}
	}

	AtomSynth::AudioBuffer result = AtomSynth::Synth::getInstance()->getAtomManager().execute();
	for (int c = 0; c < AtomSynth::AudioBuffer::getDefaultChannels(); c++) {
		for (int s = 0; s < AtomSynth::AudioBuffer::getDefaultSize(); s++) {
			buffer.setSample(c, s, result.getValue(c, s));
		}
	}

	AudioPlayHead::CurrentPositionInfo info = AudioPlayHead::CurrentPositionInfo();
	if (AudioPlayHead * head = getPlayHead())
		head->getCurrentPosition(info);
	if (info.bpm <= 1.0)
		AtomSynth::Synth::getInstance()->getParameters().m_bpm = 140.0f;
	else
		AtomSynth::Synth::getInstance()->getParameters().m_bpm = info.bpm;
}

//==============================================================================
bool AtomSynthAudioProcessor::hasEditor() const {
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AtomSynthAudioProcessor::createEditor() {
	return new AtomSynthAudioProcessorEditor(*this);
}

//==============================================================================
void AtomSynthAudioProcessor::getStateInformation(MemoryBlock& destData) {
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	int size;
	char * data = AtomSynth::Synth::getInstance()->getSaveManager().exportBytes(size);
	destData.setSize(size);
	destData.replaceWith(data, size);
}

void AtomSynthAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	std::cout << "Attempting to load state information!" << std::endl;
	AtomSynth::Synth::getInstance()->getSaveManager().importBytes((char*) data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new AtomSynthAudioProcessor();
}
