/*
 * GlobalNoteState.h
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#ifndef SOURCE_NOTEMANAGER_H_
#define SOURCE_NOTEMANAGER_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "Technical/AudioBuffer.h"

namespace AtomSynth {

class Synth;

/**
 * Describes the state of a single note.
 */
struct NoteState {
	/**
	 * Describes the state of a note.
	 */
	enum Status {
		ACTIVE, ///< The key is being pressed.
		RELEASING, ///< The key has been released, but audio is still being outputted.
		STOPPED,  ///< The synth just stopped outputting audio for this note.
		SILENT ///< There is nothing happening for this note.
	};
	double frequency = 440.0; ///< The frequency of the note.
	double velocity = 1.0; ///< The velocity of the note.
	double panning = 0.0; ///< The panning of the note. (-1 is left, 1 is right.)
	unsigned long int timestamp = 0; ///< The time that the note started playing.
	Status status = SILENT; ///< The current Status of the note.
};

/**
 * Stores information about the state of notes.
 * See NoteState for more details about what it
 * stores.
 */
class NoteManager {
private:
	std::vector<NoteState> m_notes;
	Synth * m_parent = nullptr;
	void setup();

	friend class Synth;
public:
	NoteManager();
	virtual ~NoteManager();
	/**
	 * Starts a note at a particular polyphony index.
	 * If that polyphony index is already taken by another
	 * note, you just killed that note. Don't worry, they
	 * aren't sentient. Yet.
	 * @param index The polyphony index to start the note at.
	 * @param newState A note state containing data about the new note.
	 */
	void start(int index, NoteState newState);
	/**
	 * Stops a note at a particular polyphony index.
	 * This will also give any AtomController objects
	 * rendering this note a chance to reset. See
	 * start() for ethical ramifications.
	 * @param index The note to stop's polyphony index.
	 */
	void stop(int index);
	/**
	 * Completely kills a note at a particular polyphony
	 * index. This will not give any AtomController
	 * objects rendering this note a chance to reset.
	 * It is reccomended to use stop() instead. Also
	 * see start() for ethical ramifications.
	 * @param index The note to end's polyphony index.
	 */
	void end(int index);
	/**
	 * Adds a note from a MidiMessage.
	 * @param midi The MidiMessage to convert into a note and add.
	 */
	void addMidiNote(MidiMessage midi);
	/**
	 * Removes a note from a MidiMessage.
	 * @param midi The MidiMessage to find and remove.
	 */
	void removeMidiNote(MidiMessage midi);
	/**
	 * Adds a note from a frequency.
	 * @param frequency The frequency to convert into a note and add.
	 */
	void addFrequency(double frequency);
	/**
	 * Removes a note from a frequency.
	 * @param frequency The frequency to find and remove.
	 */
	void removeFrequency(double frequency);
	/**
	 * Finds if any notes are playing at a particular frequency.
	 * @param frequency The frequency to search for.
	 * @return True if a note on that frequency is currently being played.
	 */
	bool getIsFrequencyActive(double frequency);
	/**
	 * Dumps all note states to the console. Use
	 * this if you don't have access to a real
	 * debugger.
	 */
	void listNotes();
	/**
	 * Gets a note state at a particular polyphony index.
	 * @param index The polyphony index to retrieve the NoteState from.
	 * @return The NoteState at that particular index.
	 */
	NoteState getNoteState(int index);
	/**
	 * Gets if a note at a particular polyphony index is
	 * stopped. Also see NoteState::Status::STOPPED.
	 * @param index The polyphony index to check.
	 * @return True if the selected note state is stopped.
	 */
	bool getIsStopped(int index);
	/**
	 * Gets if a note at a particular polyphony index is
	 * active (the status is not NoteState::Status::SILENT.)
	 * @param index The polyphony index to check.
	 * @return True if the selected note state is active.
	 */
	bool getIsActive(int index);
};

} /* namespace AtomSynth */

#endif /* SOURCE_NOTEMANAGER_H_ */
