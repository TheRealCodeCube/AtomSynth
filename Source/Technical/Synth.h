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
#include "AudioBuffer.h"
#include "AtomManager.h"
#include "GuiManager.h"
#include "LogManager.h"
#include "NoteManager.h"
#include "SaveManager.h"

namespace AtomSynth {

class Atom;
class AtomController;
class AudioBuffer;
class SaveManager;
class SaveState;

/**
 * Stores  simple global parameters about the
 * synth, such as BPM and sample rate.
 */
struct SynthParameters {
	int m_sampleRate = 44100, ///< How many samples are calculated per second.
			m_timestamp = 0, ///< Increased by one for every sample calculated by the synth. m_timestamp / m_sampleRate will give the number of seconds the synth has been running for.
			m_polyphony = 32; ///< The maximum number of notes that can be played at once.
	double m_bpm = 140.0; ///< How many beats per minute. This can change rapidly, if the user has automated it in the audio host.
};

/**
 * Singleton which contains data
 * about the synth as a whole.
 */
class Synth: public MouseListener {
private:
	AtomManager m_atomManager;
	GuiManager m_guiManager;
	LogManager m_logManager;
	NoteManager m_noteManager;
	SaveManager m_saveManager;
	SynthParameters m_parameters;
	static Synth * s_instance;
	Synth();
public:
	/**
	 * Returns the only instance of the synth (singleton.)
	 * @return The singleton instance.
	 */
	static Synth * getInstance() {
		if(s_instance == nullptr) {
			s_instance = new Synth();
		}
		return s_instance;
	}
	/**
	 * Deletes the current instance of the synth (singleton.)
	 */
	static void deleteInstance() {
		if(s_instance == nullptr) return;
		delete(s_instance);
		s_instance = nullptr;
	}
	/**
	 * Call this once the sampleRate and bufferSize have
	 * been established (usually by the plugin host.)
	 * @param sampleRate The sample rate that should be rendered, in samples per second.
	 * @param bufferSize The size, in samples, of a single buffer of output.
	 */
	static void initialize(int sampleRate, int bufferSize);

	virtual ~Synth();

	/**
	 * Returns this Synth's instance of AtomManager, which is
	 * responsible for managing the structure, saving / loading,
	 * and executing of atoms.
	 * @return This Synth's instance of AtomManager.
	 */
	AtomManager & getAtomManager() {
		return m_atomManager;
	}
	/**
	 * Returns this Synth's instance of GuiManager, which is
	 * responsible for keeping track of the root component of
	 * the gui as well as any right-click menus, if open.
	 * @return This Synth's instance of GuiManager.
	 */
	GuiManager & getGuiManager() {
		return m_guiManager;
	}
	/**
	 * Returns this Synth's instance of LogManager, which is
	 * used to output detailed logs upon user request to help
	 * with debugging.
	 * @return
	 */
	LogManager & getLogManager() {
		return m_logManager;
	}
	/**
	 * Returns this Synth's instance of NoteManager, which is
	 * responsible for keeping track of which notes are
	 * currently plaing, how long they shoud play for, etc.
	 * @return This Synth's instance of NoteManager.
	 */
	NoteManager & getNoteManager() {
		return m_noteManager;
	}
	/**
	 * Returns basic global parameters of the Synth such as
	 * sample rate and bpm. See SynthParameters for more details.
	 * @return Basic global parameters of the Synth.
	 */
	SynthParameters & getParameters() {
		return m_parameters;
	}
	/**
	 * Returns this Synth's instance of SaveManager, which is
	 * responsible for saving / loading the synth to / from
	 * files, as well as keeping track of the name of the patch.
	 * @return
	 */
	SaveManager & getSaveManager() {
		return m_saveManager;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_SYNTH_H_ */
