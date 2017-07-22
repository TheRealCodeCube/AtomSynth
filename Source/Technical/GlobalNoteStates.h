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

struct NoteState {
	//Active = key being pressed
	//Releasing = key released, waiting for synthesizer to quiet down
	//Silent = synthesizer is quiet, no need to calculate it any more
	enum Status {
		ACTIVE, RELEASING, STOPPED, SILENT
	};
	double frequency = 440.0f;
	double velocity = 1.0f;
	double panning = 0.0f;
	unsigned long int timestamp = 0;
	Status status = SILENT;
};

class GlobalNoteStates {
private:
	static std::vector<NoteState> s_notes;
public:
	static double s_bpm;
	static unsigned long int s_currentTimestamp;
	static int s_sampleRate, s_noteBufferSize, s_polyphony;
	GlobalNoteStates();
	virtual ~GlobalNoteStates();
	static void init();
	static void start(int index, NoteState newState);
	static void stop(int index);
	static void end(int index);
	static void addMidiNote(MidiMessage midi);
	static void removeMidiNote(MidiMessage midi);
	static void addFrequency(double frequency);
	static void removeFrequency(double frequency);
	static bool getIsFrequencyActive(double frequency);
	static void listNotes();
	static NoteState getNoteState(int index);
	static bool getIsStopped(int index);
	static bool getIsActive(int index);
	static void setPolyphony(int polyphony) {
		s_polyphony = polyphony;
	}
	static int getPolyphony() {
		return s_polyphony;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_GLOBALNOTESTATES_H_ */
