/*
 * AtomManager.h
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_ATOMMANAGER_H_
#define SOURCE_TECHNICAL_ATOMMANAGER_H_

#include <vector>
#include "Atoms/Atom.h"

/*
 * Atom.h
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

namespace AtomSynth {

class Synth;

class AtomManager {
private:
	std::vector<AtomController *> m_atoms;
	std::vector<int> m_atomExecutionOrder;
	std::vector<AtomController *> m_availableAtoms;
	AtomSynth::AudioBuffer m_output;
	Synth * m_parent = nullptr;
	void setup();

	friend class Synth;
public:
	AtomManager();
	virtual ~AtomManager();

	/**
	 * Updates the order in which the AtomController
	 * objects should be executed, based on how
	 * they are connected. TODO: Tell the user if
	 * there is an error (which is usually caused
	 * by circular dependencies.)
	 */
	void updateExecutionOrder();
	/**
	 * Executes one iteration of the synth, and
	 * returns the result.
	 * @return An AudioBuffer containing the rendered audio.
	 */
	AudioBuffer & execute();

	/**
	 * Adds an AtomController to the synth.
	 * @param controller The AtomController to add.
	 */
	void addAtom(AtomController * controller);
	/**
	 * Gets a vector of all AtomControllers.
	 * @return A vector of all AtomControllers.
	 */
	std::vector<AtomController *> & getAtoms() {
		return m_atoms;
	}
	/**
	 * Gets a vector of all available types of
	 * AtomControllers, one instance for each
	 * class.
	 * @return A vector of all available types of AtomControllers.
	 */
	std::vector<AtomController *> & getAvailableAtoms() {
		return m_availableAtoms;
	}

	/**
	 * Loads atoms and their connections from a SaveState.
	 * @param state The SaveState to load from.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves atoms and their connections to a SaveState.
	 * @return A SaveState representing all the atoms and their connections to each other.
	 */
	SaveState saveSaveState();
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_ATOMMANAGER_H_ */
