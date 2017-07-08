/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[0.000000:0.000000:3.000000:0.000000:0.000000:sInput:sBasic:]]
#include "Input.h"
#include "Technical/GlobalNoteStates.h"
#include "Technical/SaveState.h"

namespace AtomSynth
{

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

InputController::InputController()
	: AtomController(AtomParameters(0, 0, false, 3))
{
	init();


	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * InputController::createAtom(int index)
{
	return new InputAtom(* this, index);
}

SaveState InputController::saveSaveState()
{
	SaveState & toReturn = * new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = * new SaveState();
	extraData.addValue(0); //Store the revision this was saved with, to preserve backwards compatibility.
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void InputController::loadSaveState(SaveState state)
{
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	/* END LOAD CODE */
}

InputAtom::InputAtom(InputController & parent, int index)
	: Atom(parent, index),
	  m_parent(parent)
{
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void InputAtom::execute()
{
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();

	/* BEGIN USER-DEFINED EXECUTION CODE */
	int multiplier = 0;
	switch(GlobalNoteStates::getNoteState(getId()).status)
	{
	case NoteState::ACTIVE:
		multiplier = 1;
		break;
	case NoteState::RELEASING:
		multiplier = -1;
		break;
	case NoteState::SILENT:
		multiplier = 0;
	}

	unsigned long int base = GlobalNoteStates::s_currentTimestamp - GlobalNoteStates::getNoteState(getId()).timestamp;
	double time, frequency = GlobalNoteStates::getNoteState(getId()).frequency;
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++)
	{
		for(int s = 0; s < AudioBuffer::getDefaultSize(); s++)
		{
			time = (double(s + base) / m_sampleRate_f) * multiplier;
			m_outputs[0].setValue(c, s, frequency);
			m_outputs[1].setValue(c, s, time);
		}
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void InputAtom::reset()
{
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

