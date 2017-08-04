/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[1.000000:1.000000:0.000000:sOutput:sBasic:saudio:s:swave:s:]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Output.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

OutputController::OutputController() :
		AtomController(AtomParameters(1, 1, false, 0)) {
	init();

	addInputIcon("wave");


	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * OutputController::createAtom(int index) {
	return new OutputAtom(*this, index);
}

SaveState OutputController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void OutputController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	/* END LOAD CODE */
}

OutputAtom::OutputAtom(OutputController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void OutputAtom::execute() {
	Atom::execute();

	IOSet io = IOSet();
	DVecIter * audioInput = io.addInput(m_primaryInputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */

	/* END USER-DEFINED EXECUTION CODE */
}

void OutputAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

