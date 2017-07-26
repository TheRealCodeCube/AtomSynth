/*
 * GlobalNoteState.h
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#ifndef SOURCE_GLOBALNOTESTATES_H_
#define SOURCE_GLOBALNOTESTATES_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "Technical/AudioBuffer.h"

namespace AtomSynth {

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
 * This should really get refactored. Stores global
 * data about the synth and active notes.
 */
class GlobalNoteStates {
private:
	static std::vector<NoteState> s_notes;
public:
	static double s_bpm; ///< The current BPM, which can be set from the plugin host.
	static unsigned long int s_currentTimestamp; ///< The current time.
	static int s_sampleRate, ///< The current sample rate, which can be set from the plugin host.
		s_polyphony; ///< The number of polyphonies to use.
	GlobalNoteStates();
	virtual ~GlobalNoteStates();
	/**
	 * Initializes one note state for every polyphony.
	 */
	static void init();
	/**
	 * Starts a note at a particular polyphony index.
	 * If that polyphony index is already taken by another
	 * note, you just killed that note. Don't worry, they
	 * aren't sentient. Yet.
	 * @param index The polyphony index to start the note at.
	 * @param newState A note state containing data about the new note.
	 */
	static void start(int index, NoteState newState);
	/**
	 * Stops a note at a particular polyphony index.
	 * This will also give any AtomController objects
	 * rendering this note a chance to reset. See
	 * start() for ethical ramifications.
	 * @param index The note to stop's polyphony index.
	 */
	static void stop(int index);
	/**
	 * Completely kills a note at a particular polyphony
	 * index. This will not give any AtomController
	 * objects rendering this note a chance to reset.
	 * It is reccomended to use stop() instead. Also
	 * see start() for ethical ramifications.
	 * @param index The note to end's polyphony index.
	 */
	static void end(int index);
	/**
	 * Adds a note from a MidiMessage.
	 * @param midi The MidiMessage to convert into a note and add.
	 */
	static void addMidiNote(MidiMessage midi);
	/**
	 * Removes a note from a MidiMessage.
	 * @param midi The MidiMessage to find and remove.
	 */
	static void removeMidiNote(MidiMessage midi);
	/**
	 * Adds a note from a frequency.
	 * @param frequency The frequency to convert into a note and add.
	 */
	static void addFrequency(double frequency);
	/**
	 * Removes a note from a frequency.
	 * @param frequency The frequency to find and remove.
	 */
	static void removeFrequency(double frequency);
	/**
	 * Finds if any notes are playing at a particular frequency.
	 * @param frequency The frequency to search for.
	 * @return True if a note on that frequency is currently being played.
	 */
	static bool getIsFrequencyActive(double frequency);
	/**
	 * Dumps all note states to the console. Use
	 * this if you don't have access to a real
	 * debugger.
	 */
	static void listNotes();
	/**
	 * Gets a note state at a particular polyphony index.
	 * @param index The polyphony index to retrieve the NoteState from.
	 * @return The NoteState at that particular index.
	 */
	static NoteState getNoteState(int index);
	/**
	 * Gets if a note at a particular polyphony index is
	 * stopped. Also see NoteState::Status::STOPPED.
	 * @param index The polyphony index to check.
	 * @return True if the selected note state is stopped.
	 */
	static bool getIsStopped(int index);
	/**
	 * Gets if a note at a particular polyphony index is
	 * active (the status is not NoteState::Status::SILENT.)
	 * @param index The polyphony index to check.
	 * @return True if the selected note state is active.
	 */
	static bool getIsActive(int index);
	/**
	 * Sets the number of notes the synth can render at
	 * a time.
	 * @param polyphony The number of notes the synth should be able to render at a time.
	 */
	static void setPolyphony(int polyphony) {
		s_polyphony = polyphony;
	}
	/**
	 * Gets the number of notes the synth can render at
	 * a time.
	 * @return The number of notes the synth can render at a time.
	 */
	static int getPolyphony() {
		return s_polyphony;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_GLOBALNOTESTATES_H_ */
