/*
 * Synth.h
 *
 *  Created on: Sep 30, 2016
 *      Author: josh
 */

#ifndef SOURCE_SYNTH_H_
#define SOURCE_SYNTH_H_

#include <vector>
#include "Atoms/Atom.h"
#include "Technical/AudioBuffer.h"

namespace AtomSynth {

class Atom;
class AtomController;
class AudioBuffer;
class SaveState;

/**
 * Singleton which contains data
 * about the synth as a whole.
 */
class Synth: public MouseListener {
private:
	std::vector<AtomController *> m_atoms;
	std::vector<int> m_atomExecutionOrder;
	std::vector<AtomController *> m_availableAtoms;
	bool m_atomsReloaded;
	AtomSynth::AudioBuffer m_output;
	Component * m_currentRightClickMenu;
	Component * m_rootComponent;
public:
	static Synth * s_instance; ///< The instance. Need to replace this with a getter.
	 /**
	  * Call this once the sampleRate and bufferSize have
	  * been established (usually by the plugin host.)
	  * @param sampleRate The sample rate that should be rendered, in samples per second.
	  * @param bufferSize The size, in samples, of a single buffer of output.
	  */
	static void initialize(int sampleRate, int bufferSize);

	Synth();
	virtual ~Synth();

	/**
	 * Initializes several things. Call this after init().
	 */
	void setup();

	/**
	 * Renders a right-click menu on top of everything,
	 * and closes it once it loses focus.
	 * @param newRightClickMenu The Component to render on top of everything else.
	 */
	void openRightClickMenu(Component * newRightClickMenu);
	/**
	 * Closes any right-click menu that is open. See
	 * openRightClickMenu() for more details.
	 */
	void closeRightClickMenu();
	/**
	 * Gets the currently open right-click menu. See
	 * openRightClickMenu() for more details.
	 * @return The currently open right-click menu. Can be nullptr if no menu is opened.
	 */
	Component * getRightClickMenu() {
		return m_currentRightClickMenu;
	}
	/**
	 * This component is rendered inside of the synth,
	 * and under any right-click menus.
	 * @param newRootComponent The main Component that should be rendered.
	 */
	void setRootComponent(Component * newRootComponent) {
		m_rootComponent = newRootComponent;
	}

	/**
	 * Juce mouseDown event.
	 * @param Juce MouseEvent object.
	 */
	virtual void mouseDown(const MouseEvent &);

	/**
	 * Executes one iteration of the synth, and
	 * returns the result.
	 * @return An AudioBuffer containing the rendered audio.
	 */
	AudioBuffer execute();
	/**
	 * Updates the order in which the AtomController
	 * objects should be executed, based on how
	 * they are connected. TODO: Tell the user if
	 * there is an error (which is usually caused
	 * by circular dependencies.)
	 */
	void updateExecutionOrder();

	/**
	 * This value is used by the GUI to add all
	 * AtomController objects' GUIs to itself.
	 * @param reloaded If set to true, the above will happen.
	 */
	void setAtomsReloaded(bool reloaded = true) {
		m_atomsReloaded = reloaded;
	}
	/**
	 * This value is used by the GUI to add all
	 * AtomController objects' GUIs to itself.
	 * @return True if setAtomsReloaded() was previously set.
	 */
	bool getAtomsReloaded() {
		return m_atomsReloaded;
	}

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
	 * Loads the entire synth from a SaveState.
	 * @param state The SaveState to load from.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves the entire synth to a SaveState.
	 * @return The synth's current state, represented by a SaveState.
	 */
	SaveState saveSaveState();
};

} /* namespace AtomSynth */

#endif /* SOURCE_SYNTH_H_ */
