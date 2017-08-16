/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[191.000000:1.000000:1.000000:sMultiply:sDebug:ssignal:ssignal:swave:swave:[[[4.000000:8.000000:2.000000:2.000000:2.000000:1.000000:sAtomKnob:sfac11:sMult. Factor (-1 to 1):]][-1.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:2.000000:2.000000:2.000000:1.000000:sAtomKnob:sfac01:sMult. Factor (0 to 1):]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Multiply.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

MultiplyController::MultiplyController() :
		AtomController(AtomParameters(191, 1, true, 1)) {
	init();

	addInputIcon("wave");
	addOutputIcon("wave");

	m_gui.addComponent(&m_fac11);
	m_fac11.setBounds(CB(8.000000, 2.000000, 2.000000, 2.000000));
	m_fac11.addListener(this);
	m_fac11.setValue(1.000000);
	m_fac11.setBounded(true);
	addAutomatedControl(m_fac11, m_fac11Iter);
	m_gui.addComponent(m_fac11.createLabel("Mult. Factor (-1 to 1)", true));

	m_gui.addComponent(&m_fac01);
	m_fac01.setBounds(CB(2.000000, 2.000000, 2.000000, 2.000000));
	m_fac01.addListener(this);
	m_fac01.setRange(0.000000, 1.000000);
	m_fac01.setValue(1.000000);
	m_fac01.setBounded(true);
	addAutomatedControl(m_fac01, m_fac01Iter);
	m_gui.addComponent(m_fac01.createLabel("Mult. Factor (0 to 1)", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * MultiplyController::createAtom(int index) {
	return new MultiplyAtom(*this, index);
}

SaveState MultiplyController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_fac11.saveSaveState());
	extraData.addState(m_fac01.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void MultiplyController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1)
	{
		m_fac11.loadSaveState(extraData.getNextState());
		m_fac01.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void MultiplyController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

MultiplyAtom::MultiplyAtom(MultiplyController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void MultiplyAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & fac11Iter = m_parent.m_fac11Iter;
	DVecIter & fac01Iter = m_parent.m_fac01Iter;

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if(signalInput == nullptr) {
		m_primaryInputs[0]->fill(0.0);
	} else {
		for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++)
		{
			for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++)
			{
				//PUT YOUR EXECUTION CODE HERE
				(* signalOutput) = (** signalInput) * (* fac01Iter) * (* fac11Iter);
				automation.incrementPosition();
				io.incrementPosition();
			}
			automation.incrementChannel();
			io.incrementChannel();
		}
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void MultiplyAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

