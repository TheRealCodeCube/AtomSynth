/*
 * GlobalNoteState.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#include "GlobalNoteStates.h"

#include <cmath>
#include <iostream>

#include "Atoms/Atom.h"

namespace AtomSynth {

std::vector<NoteState> GlobalNoteStates::s_notes = std::vector<NoteState>();

double GlobalNoteStates::s_bpm = 128.0;
unsigned long int GlobalNoteStates::s_currentTimestamp = 0;
int GlobalNoteStates::s_sampleRate = 44100, GlobalNoteStates::s_polyphony = 32;

GlobalNoteStates::GlobalNoteStates() {

}

GlobalNoteStates::~GlobalNoteStates() {
	// TODO Auto-generated destructor stub
}

void GlobalNoteStates::init() {
	s_notes.resize((unsigned int) s_polyphony, NoteState());
}

void GlobalNoteStates::start(int index, NoteState newState) {
	newState.status = NoteState::ACTIVE;
	s_notes[index] = newState;
}

void GlobalNoteStates::stop(int index) {
	s_notes[index].status = NoteState::STOPPED;
}

void GlobalNoteStates::end(int index) {
	s_notes[index].status = NoteState::SILENT;
}

NoteState GlobalNoteStates::getNoteState(int index) {
	return s_notes[index];
}

void GlobalNoteStates::addMidiNote(MidiMessage midi) {
	//removeMidiNote(midi); //Prevents duplicates
	double frequency = pow(2.0, (double(midi.getNoteNumber()) - 69.0) / 12.0) * 440.0; //Thank you, Wikipedia.
	for (NoteState s : s_notes)
		if ((s.status == NoteState::ACTIVE) && (fabs(s.frequency - frequency) < 0.01f))
			return; //Do not add duplicates.
	int index = 0;
	for (NoteState s : s_notes) {
		if (!getIsActive(index))
			break;
		index++;
	}
	if (index == s_polyphony)
		return;
	NoteState note;
	note.timestamp = s_currentTimestamp;
	note.frequency = frequency;
	note.velocity = midi.getFloatVelocity();
	start(index, note);
}

void GlobalNoteStates::removeMidiNote(MidiMessage midi) {
	int index = 0;
	double frequency = pow(2.0, (double(midi.getNoteNumber()) - 69.0) / 12.0) * 440.0; //Thank you, Wikipedia.
	for (NoteState n : s_notes) {
		//No ==, because slight floating-point errors might make a significant difference.
		if ((fabs(n.frequency - frequency) < 0.01f) && (n.status == NoteState::ACTIVE))
			break;
		index++;
	}
	if (index == s_polyphony)
		return;
	s_notes[index].status = NoteState::RELEASING;
}

void GlobalNoteStates::addFrequency(double frequency) {
	for (NoteState s : s_notes)
		if ((s.status == NoteState::ACTIVE) && (fabs(s.frequency - frequency) < 0.01f))
			return; //Do not add duplicates.
	int index = 0;
	for (NoteState s : s_notes) {
		if (!getIsActive(index))
			break;
		index++;
	}
	if (index == s_polyphony)
		return;
	NoteState note;
	note.timestamp = s_currentTimestamp;
	note.frequency = frequency;
	note.velocity = 1.0;
	start(index, note);
}

void GlobalNoteStates::removeFrequency(double frequency) {
	int index = 0;
	for (NoteState n : s_notes) {
		//No ==, because slight floating-point errors might make a significant difference.
		if ((fabs(n.frequency - frequency) < 0.01f) && (n.status == NoteState::ACTIVE))
			break;
		index++;
	}
	if (index == s_polyphony)
		return;
	s_notes[index].status = NoteState::RELEASING;
}

bool GlobalNoteStates::getIsFrequencyActive(double frequency) {
	for (NoteState n : s_notes) {
		if ((fabs(n.frequency - frequency) < 0.0001) && (n.status == NoteState::ACTIVE)) {
			return true;
		}
	}
	return false;
}

void GlobalNoteStates::listNotes() {
	int index = 0;
	for (NoteState n : s_notes) {
		if (n.status != NoteState::SILENT)
			std::cout << "[" << n.frequency << "] ";
		else
			std::cout << "off ";
		index++;
	}
	std::cout << std::endl;
}

bool GlobalNoteStates::getIsStopped(int index) {
	return (s_notes[index].status == NoteState::STOPPED);
}

bool GlobalNoteStates::getIsActive(int index) {
	return s_notes[index].status != NoteState::SILENT;
}

} /* namespace AtomSynth */
