/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[82.000000:0.000000:1.000000:sRemap:sTransforms:ssignal:ssignal:swave:swave:]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Remap.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

RemapController::RemapController() :
		AtomController(AtomParameters(82, 1, true, 1)) {
	init();

	addInputIcon("wave");
	addOutputIcon("wave");

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * RemapController::createAtom(int index) {
	return new RemapAtom(*this, index);
}

SaveState RemapController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(0); //Store the revision this was saved with, to preserve backwards compatibility.
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void RemapController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	/* END LOAD CODE */
}

RemapAtom::RemapAtom(RemapController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void RemapAtom::execute() {
	Atom::execute();

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			//PUT YOUR EXECUTION CODE HERE
			
			io.incrementPosition();
		}
		io.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void RemapAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

