/*
 * AtomManager.cpp
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#include "AtomManager.h"

#include "Atoms/AtomList.h"
#include "Technical/Log.h"
#include "Technical/Synth.h"

namespace AtomSynth {

void AtomManager::clearAtoms() {
	for(auto atom : m_atoms) {
		delete(atom);
	}
	m_atoms.clear();
}

void AtomManager::clearAvailableAtoms() {
	for(auto atom : m_availableAtoms) {
		delete(atom);
	}
	m_availableAtoms.clear();
}

void AtomManager::setup() {
	//m_atoms.clear();
	clearAvailableAtoms();
	//Because memory management.
	for(auto atom : getAllAtoms()) {
		m_availableAtoms.push_back(atom);
	}
	updateExecutionOrder();
	m_output = AudioBuffer();
}

AtomManager::AtomManager() {

}

AtomManager::~AtomManager() {
	clearAtoms();
	clearAvailableAtoms();
}

/**
 * This is used in AtomManager::updateExecutionOrder()
 * to help figure out in what order the atoms should be
 * executed.
 */
struct ReallySimpleAtomProxy {
	std::vector<ReallySimpleAtomProxy *> m_inputs; ///< Proxies of atoms that are linked to each input of this proxy.
	bool m_requirementsSatisfied; ///< Set this to true if all the inputs were previously calculated.

	ReallySimpleAtomProxy();
};

ReallySimpleAtomProxy::ReallySimpleAtomProxy() :
		m_inputs(std::vector<ReallySimpleAtomProxy *>()),
		m_requirementsSatisfied(false) {

}

void AtomManager::updateExecutionOrder() {
	//Delete any atoms marked for deletion.
	for(int i = m_atoms.size() - 1; i >= 0; i--) {
		if(m_atoms[i]->getIsMarkedForDeletion()) {
			for(auto atom : m_atoms) {
				atom->cleanupInputsFromAtom(m_atoms[i]);
			}
			delete(m_atoms[i]);
			m_atoms.erase(m_atoms.begin() + i);
		}
	}

	//Create proxies for all atoms, to simplify the compilation process.
	std::vector<ReallySimpleAtomProxy> proxies = std::vector<ReallySimpleAtomProxy>();
	proxies.resize(m_atoms.size(), ReallySimpleAtomProxy());
	m_atomExecutionOrder.clear();
	for (int i = 0; i < m_atoms.size(); i++) {
		for (std::pair<AtomSynth::AtomController *, int> & input : m_atoms[i]->getAllInputs()) {
			if (input.first != nullptr) {
				std::vector<AtomSynth::AtomController *>::const_iterator index = std::find(m_atoms.begin(), m_atoms.end(), input.first);
				if (index != m_atoms.end()) {
					proxies[i].m_inputs.push_back(&proxies[index - m_atoms.begin()]);
				}
			}
		}
	}

	bool running = true;
	int remaining = proxies.size();
	while (running) {
		int index = 0;
		bool progress = false;
		for (ReallySimpleAtomProxy & proxy : proxies) {
			if (!proxy.m_requirementsSatisfied) {
				bool satisfied = true;
				for (ReallySimpleAtomProxy * input : proxy.m_inputs) {
					if(!input->m_requirementsSatisfied) {
						satisfied = false;
						break;
					}
				}
				if (satisfied) {
					proxy.m_requirementsSatisfied = true;
					remaining--;
					m_atomExecutionOrder.push_back(index);
					progress = true;
				}
			}
			index++;
		}
		if(!remaining) {
			info("Network successfully compiled");
			running = false;
		} else if(!progress) {
			warn("Network compile failed");
			warn("Unable to compute paths for " + std::to_string(remaining) + " atoms.");
			m_parent->getGuiManager().addMessage("Your patch is invalid! Make sure that it does not have any feedback loops.");
			running = false;
		}
	}
}

AudioBuffer & AtomManager::execute() {
	if (m_atoms.size() != 0) {
		AtomSynth::AtomController * output = nullptr;
		//This is used in case shouldDebugEverything is set in the middle of execution.
		bool logEverything = m_parent->getLogManager().shouldDebugEverything();
		if(logEverything) {
			m_parent->getLogManager().clear();
			m_parent->getLogManager().addLabel("Synth Properties");
				m_parent->getLogManager().addLabel("Name");
					m_parent->getLogManager().writeString(m_parent->getSaveManager().getPatchName());
				m_parent->getLogManager().endLabel();
				m_parent->getLogManager().addLabel("Sample Rate");
					m_parent->getLogManager().writeInt(m_parent->getParameters().m_sampleRate);
				m_parent->getLogManager().endLabel();
				m_parent->getLogManager().addLabel("Channels");
					m_parent->getLogManager().writeInt(AudioBuffer::getDefaultChannels());
				m_parent->getLogManager().endLabel();
				m_parent->getLogManager().addLabel("Samples per Buffer");
					m_parent->getLogManager().writeInt(AudioBuffer::getDefaultSamples());
				m_parent->getLogManager().endLabel();
				m_parent->getLogManager().addLabel("Copy-pasteable Save State");
					m_parent->getLogManager().writeString(m_parent->getSaveManager().exportString());
				m_parent->getLogManager().endLabel();
			m_parent->getLogManager().endLabel();
			m_parent->getLogManager().addLabel("Atoms");
		}
		for (AtomSynth::AtomController * controller : m_atoms) {
			if (controller->getId() == 1) //ID for OutputController.
				output = controller;
		}
		std::pair<AtomController *, int> input;
		if (output == nullptr)
			return m_output;
		else {
			input = output->getInput(0);
			if (input.first == nullptr) {
				return m_output;
			}
		}
		for (int index : m_atomExecutionOrder) {
			m_atoms[index]->execute();
		}

		double sample = 0.0;
		std::vector<double> totals;
		totals.resize(Synth::getInstance()->getParameters().m_polyphony, 0.0);
		m_output.fill(0.0);
		//Sum up all polyphony.
		for (int note = 0; note < Synth::getInstance()->getParameters().m_polyphony; note++) {
			std::vector<double>::iterator bufferIterator = input.first->getAtom(note)->getOutput(input.second)->getData().begin();
			if (Synth::getInstance()->getNoteManager().isActive(note)) {
				for (int s = 0; s < AudioBuffer::getDefaultChannels() * AudioBuffer::getDefaultSize(); s++) {
					sample = (*bufferIterator);
					totals[note] += fabs(sample);
					m_output.getData()[s] += sample;
					bufferIterator++;
				}
			}
		}

		//Ummm... not sure what this does. I should probably remove it.
		for (int s = 0; s < AudioBuffer::getDefaultChannels() * AudioBuffer::getDefaultSize(); s++) {
			m_output.getData()[s] /= 2.0;
		}

		int index = 0;
		for (double total : totals) {
			if (Synth::getInstance()->getNoteManager().isStopped(index)) {
				Synth::getInstance()->getNoteManager().end(index);
			} else if (Synth::getInstance()->getNoteManager().getNoteState(index).status == AtomSynth::NoteState::RELEASING) {
				if (total <= 0.00001) {
					Synth::getInstance()->getNoteManager().stop(index);
				}
			}
			index++;
		}
		if(logEverything) {
			m_parent->getLogManager().clearDebugEverything();
			m_parent->getLogManager().endLabel();
			m_parent->getLogManager().dumpToFile();
		}
	}
	return m_output;
}

void AtomManager::addAtom(AtomController* controller) {
	m_atoms.push_back(controller);
	m_atomExecutionOrder.resize(m_atoms.size(), 0);
	updateExecutionOrder();
	m_parent->getGuiManager().setReloadGuis();
}

void AtomManager::loadSaveState(SaveState state) {
	clearAtoms();
	for (SaveState & atomState : state.getStates()) {
		for (AtomController * controller : m_availableAtoms) {
			if (controller->getId() == int(atomState.getValue(0))) {
				AtomController * atom = controller->createNewInstance();
				atom->loadSaveState(atomState.getState(1));
				m_atoms.push_back(atom);
			}
		}
	}

	int index = 0;
	for (SaveState & atomState : state.getStates()) {
		int input = 0;
		for (SaveState & inputState : atomState.getState(0).getStates()) {
			if (inputState.getValue(0) != -1.0) {
				m_atoms[index]->linkInput(input, m_atoms[int(inputState.getValue(0))], int(inputState.getValue(1)));
			}
			input++;
		}
		index++;
	}

	updateExecutionOrder();
	m_parent->getGuiManager().setReloadGuis();
	info("Network loaded from SaveState");
}

SaveState AtomManager::saveSaveState() {
	SaveState state = SaveState();

	for (AtomController * atom : m_atoms) {
		SaveState atomState = SaveState();
		std::string name = atom->getName();
		int index = 0;
		for (AtomController * controller : m_availableAtoms) {
			if (controller->getName() == name) {
				atomState.addValue(controller->getId());
				break;
			}
			index++;
		}

		SaveState inputStates = SaveState();
		for (std::pair<AtomController *, int> & input : atom->getAllInputs()) {
			if (input.first == nullptr) {
				SaveState inputState = SaveState();
				inputState.addValue(-1.0);
				inputState.addValue(-1.0);
				inputStates.addState(inputState);
			} else {
				index = 0;
				for (AtomController * inputAtom : m_atoms) {
					if (inputAtom == input.first) {
						SaveState inputState = SaveState();
						inputState.addValue(index);
						inputState.addValue(input.second);
						inputStates.addState(inputState);
						break;
					}
					index++;
				}
			}
		}

		atomState.addState(inputStates);
		atomState.addState(atom->saveSaveState());
		state.addState(atomState);
	}

	return state;
}

} /* namespace AtomSynth */
