/*
 * Synth.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: josh
 */

#include "Synth.h"

#include "Atoms/Atom.h"
#include "Atoms/AtomList.h"
#include "Technical/AudioBuffer.h"
#include "Technical/GlobalNoteStates.h"
#include "Technical/SaveState.h"

namespace AtomSynth {

Synth * Synth::s_instance = nullptr;

Synth::Synth() :
		m_atomsReloaded(false),
		m_output(AtomSynth::AudioBuffer()),
		m_currentRightClickMenu(nullptr),
		m_rootComponent(nullptr) {
	// TODO Auto-generated constructor stub
	s_instance = this;
	//Desktop::getInstance().addGlobalMouseListener(this);
}

Synth::~Synth() {
	// TODO Auto-generated destructor stub
}

void Synth::initialize(int sampleRate, int bufferSize) {
	if (s_instance != nullptr) {
		delete (s_instance);
	}
	GlobalNoteStates::s_sampleRate = sampleRate;
	GlobalNoteStates::setPolyphony(32);
	GlobalNoteStates::init();
	AudioBuffer::setDefaultSize(bufferSize);
	AudioBuffer::setDefaultChannels(2);
	new Synth();
	s_instance->setup();
	s_instance->loadSaveState(AtomSynth::SaveState("[[0.000000:[[-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:]][[225.000000:115.000000:][0.000000:]]][1.000000:[[3.000000:0.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:]][[225.000000:511.000000:][0.000000:]]][12.000000:[[0.000000:1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:]][[253.000000:246.000000:][5.000000:[1.000000:2.000000:][0.000000:2.000000:][1.000000:2.000000:][0.000000:2.000000:][0.000000:2.000000:][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]]]]][10.000000:[[0.000000:0.000000:][2.000000:0.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:][-1.000000:-1.000000:]][[225.000000:373.000000:][3.000000:[0.000000:][1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:][-1.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][1.000000:0.000000:[0.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]][0.000000:0.000000:[-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:][-1.000000:0.000000:1.000000:]]]]]]"));
}

void Synth::setup() {
	m_atoms.clear();
	m_availableAtoms = getAllAtoms();
	AtomSynth::GlobalNoteStates::init();
	updateExecutionOrder();
	m_atomsReloaded = true;
}

void Synth::openRightClickMenu(Component * newRightClickMenu) {
	if (m_rootComponent == nullptr) {
		return;
	}

	if (m_currentRightClickMenu != nullptr) {
		closeRightClickMenu();
	}

	if (newRightClickMenu != nullptr) {
		m_currentRightClickMenu = newRightClickMenu;
		m_rootComponent->addAndMakeVisible(m_currentRightClickMenu);
		m_currentRightClickMenu->setAlwaysOnTop(true);
	}
}

void Synth::closeRightClickMenu() {
	if ((m_rootComponent == nullptr) || (m_currentRightClickMenu == nullptr)) {
		return;
	}

	m_rootComponent->removeChildComponent(m_currentRightClickMenu);
	delete (m_currentRightClickMenu);
	m_currentRightClickMenu = nullptr;
}

void Synth::mouseDown(const MouseEvent & event) {
	if ((m_rootComponent != nullptr) && (m_currentRightClickMenu != nullptr)) {
		if (!((event.originalComponent == m_currentRightClickMenu) || (m_currentRightClickMenu->isParentOf(event.originalComponent)))) {
			closeRightClickMenu();
		}
	}
}

void Synth::addAtom(AtomSynth::AtomController* controller) {
	m_atoms.push_back(controller);
	m_atomExecutionOrder.resize(m_atoms.size(), 0);
	updateExecutionOrder();
	m_atomsReloaded = true;
}

struct ReallySimpleAtomProxy {
	std::vector<ReallySimpleAtomProxy *> m_inputs;
	bool m_requirementsSatisfied;

	ReallySimpleAtomProxy();
	bool update();
};

ReallySimpleAtomProxy::ReallySimpleAtomProxy() :
		m_inputs(std::vector<ReallySimpleAtomProxy *>()),
		m_requirementsSatisfied(false) {

}

void Synth::updateExecutionOrder() {
	int toDelete = 0;
	while (toDelete != -1) {
		toDelete = -1;
		for (int i = 0; i < m_atoms.size(); i++) {
			if (m_atoms[i]->getIsMarkedForDeletion()) {
				toDelete = i;
			}
		}
		if (toDelete != -1) {
			for (AtomController * atom : m_atoms) {
				atom->cleanupInputsFromAtom(m_atoms[toDelete]);
			}
			m_atoms.erase(m_atoms.begin() + toDelete);
		}
	}

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
				progress = true;
				proxy.m_requirementsSatisfied = true;
				for (ReallySimpleAtomProxy * input : proxy.m_inputs) {
					proxy.m_requirementsSatisfied = proxy.m_requirementsSatisfied && input->m_requirementsSatisfied; //Perform an && operation with all inputs. If one is not satisfied, the current proxy is not satisfied.
				}
				if (proxy.m_requirementsSatisfied) {
					remaining--;
					m_atomExecutionOrder.push_back(index);
				}
			}
			index++;
		}
//#define PRINT_COMPILE
		if (remaining == 0) {
#ifdef PRINT_COMPILE
			std::cout << "Compile succeeded" << std::endl;
			for(int i : m_atomExecutionOrder)
			std::cout << i << std::endl;
#endif
			running = false;
		}
		if (progress == false) {
#ifdef PRINT_COMPILE
			std::cout << "Compile failed" << std::endl;
			for(int i : m_atomExecutionOrder)
			std::cout << i << std::endl;
#endif
			running = false;
		}
	}
}

AudioBuffer Synth::execute() {
	if (m_atoms.size() != 0) {
		AtomSynth::AtomController * output = nullptr;
		for (AtomSynth::AtomController * controller : m_atoms) {
			if (controller->getName() == "Output")
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
		totals.resize(AtomSynth::GlobalNoteStates::getPolyphony(), 0.0);
		m_output.fill(0.0);
		//Sum up all polyphony.
		for (int note = 0; note < GlobalNoteStates::getPolyphony(); note++) {
			std::vector<double>::iterator bufferIterator = input.first->getAtom(note)->getOutput(input.second)->getData().begin();
			if (AtomSynth::GlobalNoteStates::getIsActive(note)) {
				for (int s = 0; s < AudioBuffer::getDefaultChannels() * AudioBuffer::getDefaultSize(); s++) {
					sample = (*bufferIterator);
					totals[note] += fabs(sample);
					m_output.getData()[s] += sample;
					bufferIterator++;
				}
			}
		}

		//Copy values to output.
		for (int s = 0; s < AudioBuffer::getDefaultChannels() * AudioBuffer::getDefaultSize(); s++) {
			m_output.getData()[s] /= 2.0;
		}

		int index = 0;
		for (double total : totals) {
			if (AtomSynth::GlobalNoteStates::getIsStopped(index)) {
				AtomSynth::GlobalNoteStates::end(index);
			} else if (AtomSynth::GlobalNoteStates::getNoteState(index).status == AtomSynth::NoteState::RELEASING) {
				if (total <= 0.00001) {
					AtomSynth::GlobalNoteStates::stop(index);
				}
			}
			index++;
		}
	}
	return m_output;
}

void Synth::loadSaveState(SaveState state) {
	m_atoms.clear();
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
	m_atomsReloaded = true;
}

SaveState Synth::saveSaveState() {
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
