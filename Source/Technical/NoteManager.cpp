/*
 * GlobalNoteState.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#include "NoteManager.h"

#include <cmath>
#include <iostream>

#include "Atoms/Atom.h"
#include "Technical/Synth.h"

namespace AtomSynth {

NoteManager::NoteManager() {

}

NoteManager::~NoteManager() {

}

void NoteManager::setup() {
	m_notes.resize((unsigned int) m_parent->getParameters().m_polyphony, NoteState());
}

void NoteManager::start(int index, NoteState newState) {
	newState.status = NoteState::ACTIVE;
	m_notes[index] = newState;
}

void NoteManager::stop(int index) {
	m_notes[index].status = NoteState::STOPPED;
}

void NoteManager::end(int index) {
	m_notes[index].status = NoteState::SILENT;
}

NoteState NoteManager::getNoteState(int index) {
	return m_notes[index];
}

void NoteManager::addMidiNote(MidiMessage midi) {
	//removeMidiNote(midi); //Prevents duplicates
	double frequency = pow(2.0, (double(midi.getNoteNumber()) - 69.0) / 12.0) * 440.0; //Thank you, Wikipedia.
	for (NoteState s : m_notes)
		if ((s.status == NoteState::ACTIVE) && (fabs(s.frequency - frequency) < 0.01f))
			return; //Do not add duplicates.
	int index = 0;
	for (NoteState s : m_notes) {
		if (!getIsActive(index))
			break;
		index++;
	}
	if (index == m_parent->getParameters().m_polyphony)
		return;
	NoteState note;
	note.timestamp = m_parent->getParameters().m_timestamp;
	note.frequency = frequency;
	note.velocity = midi.getFloatVelocity();
	start(index, note);
}

void NoteManager::removeMidiNote(MidiMessage midi) {
	int index = 0;
	double frequency = pow(2.0, (double(midi.getNoteNumber()) - 69.0) / 12.0) * 440.0; //Thank you, Wikipedia.
	for (NoteState n : m_notes) {
		//No ==, because slight floating-point errors might make a significant difference.
		if ((fabs(n.frequency - frequency) < 0.01f) && (n.status == NoteState::ACTIVE))
			break;
		index++;
	}
	if (index == m_parent->getParameters().m_polyphony)
		return;
	m_notes[index].status = NoteState::RELEASING;
}

void NoteManager::addFrequency(double frequency) {
	for (NoteState s : m_notes)
		if ((s.status == NoteState::ACTIVE) && (fabs(s.frequency - frequency) < 0.01f))
			return; //Do not add duplicates.
	int index = 0;
	for (NoteState s : m_notes) {
		if (!getIsActive(index))
			break;
		index++;
	}
	if (index == m_parent->getParameters().m_polyphony)
		return;
	NoteState note;
	note.timestamp = m_parent->getParameters().m_timestamp;
	note.frequency = frequency;
	note.velocity = 1.0;
	start(index, note);
}

void NoteManager::removeFrequency(double frequency) {
	int index = 0;
	for (NoteState n : m_notes) {
		//No ==, because slight floating-point errors might make a significant difference.
		if ((fabs(n.frequency - frequency) < 0.01f) && (n.status == NoteState::ACTIVE))
			break;
		index++;
	}
	if (index == m_parent->getParameters().m_timestamp)
		return;
	m_notes[index].status = NoteState::RELEASING;
}

bool NoteManager::getIsFrequencyActive(double frequency) {
	for (NoteState n : m_notes) {
		if ((fabs(n.frequency - frequency) < 0.0001) && (n.status == NoteState::ACTIVE)) {
			return true;
		}
	}
	return false;
}

void NoteManager::listNotes() {
	int index = 0;
	for (NoteState n : m_notes) {
		if (n.status != NoteState::SILENT)
			std::cout << "[" << n.frequency << "] ";
		else
			std::cout << "off ";
		index++;
	}
	std::cout << std::endl;
}

bool NoteManager::getIsStopped(int index) {
	return (m_notes[index].status == NoteState::STOPPED);
}

bool NoteManager::getIsActive(int index) {
	return m_notes[index].status != NoteState::SILENT;
}

} /* namespace AtomSynth */
