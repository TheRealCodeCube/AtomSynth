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
	static Synth * s_instance;
	static void initialize(int sampleRate, int bufferSize);

	Synth();
	virtual ~Synth();

	void setup();

	void openRightClickMenu(Component * newRightClickMenu);
	void closeRightClickMenu();
	Component * getRightClickMenu() {
		return m_currentRightClickMenu;
	}
	void setRootComponent(Component * newRootComponent) {
		m_rootComponent = newRootComponent;
	}

	virtual void mouseDown(const MouseEvent &);

	AudioBuffer execute();
	void updateExecutionOrder();

	void setAtomsReloaded(bool reloaded = true) {
		m_atomsReloaded = reloaded;
	}
	bool getAtomsReloaded() {
		return m_atomsReloaded;
	}

	void addAtom(AtomController * atom);
	std::vector<AtomController *> & getAtoms() {
		return m_atoms;
	}
	std::vector<AtomController *> & getAvailableAtoms() {
		return m_availableAtoms;
	}

	void loadSaveState(SaveState state);
	SaveState saveSaveState();
};

} /* namespace AtomSynth */

#endif /* SOURCE_SYNTH_H_ */
