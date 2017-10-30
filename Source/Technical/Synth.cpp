/*
 * Synth.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: josh
 */

#include "Synth.h"

#include "SaveState.h"

namespace AtomSynth {

Synth * Synth::s_instance = nullptr;

Synth::Synth() {
	m_atomManager.m_parent = this;
	m_guiManager.m_parent = this;
	m_logManager.m_parent = this;
	m_noteManager.m_parent = this;
	m_saveManager.m_parent = this;
}

Synth::~Synth() {

}

void Synth::initialize(int sampleRate, int bufferSize) {
	auto instance = getInstance();
	AudioBuffer::setDefaultSize(bufferSize);
	AudioBuffer::setDefaultChannels(2);
	instance->getParameters().m_sampleRate = sampleRate;
	instance->getParameters().m_polyphony = 32;
	instance->getAtomManager().setup();
	instance->getGuiManager().setup();
	instance->getLogManager().setup();
	instance->getNoteManager().setup();
	instance->getSaveManager().setup();
	//Only load the default patch if there isn't already another patch loaded.
	if(instance->getAtomManager().getAtoms().size() == 0) {
		instance->getSaveManager().loadDefaultPatch();
	} else {
		//Reload the current state.
		instance->getAtomManager().loadSaveState(instance->getAtomManager().saveSaveState());
	}
	info("Initialization complete");
}

} /* namespace AtomSynth */
