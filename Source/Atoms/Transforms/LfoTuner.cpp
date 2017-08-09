/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[80.000000:1.000000:1.000000:sLFO Tuner:sTransforms:s:sfreq:s:shz:[[[4.000000:3.000000:1.000000:2.000000:2.000000:1.000000:sBpmMultiple:sbpc:sBeats Per Cycle:]][1.000000:1.000000:]][[[4.000000:7.000000:1.000000:2.000000:2.000000:1.000000:sAtomKnob:smult:sMultiplier:]][0.000000:2.000000:1.000000:0.000000:1.000000:s:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "LfoTuner.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

LfoTunerController::LfoTunerController() :
		AtomController(AtomParameters(80, 0, true, 1)) {
	init();

	addOutputIcon("hz");

	m_gui.addComponent(&m_bpc);
	m_bpc.setBounds(CB(3.000000, 1.000000, 2.000000, 2.000000));
	m_bpc.addListener(this);
	m_gui.addComponent(m_bpc.createLabel("Beats Per Cycle", true));

	m_gui.addComponent(&m_mult);
	m_mult.setBounds(CB(7.000000, 1.000000, 2.000000, 2.000000));
	m_mult.addListener(this);
	m_mult.setRange(0.000000, 2.000000);
	m_mult.setValue(1.000000);
	m_mult.setBounded(true);
	addAutomatedControl(m_mult, m_multIter);
	m_gui.addComponent(m_mult.createLabel("Multiplier", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * LfoTunerController::createAtom(int index) {
	return new LfoTunerAtom(*this, index);
}

SaveState LfoTunerController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_bpc.saveSaveState());
	extraData.addState(m_mult.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void LfoTunerController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1) {
		m_bpc.loadSaveState(extraData.getNextState());
		m_mult.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void LfoTunerController::bpmMultipleChanged(BpmMultiple * multiple) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void LfoTunerController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

LfoTunerAtom::LfoTunerAtom(LfoTunerController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void LfoTunerAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & multIter = m_parent.m_multIter;

	IOSet io = IOSet();
	DVecIter & freqOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	bool constant = m_parent.m_mult.getResult().isConstant();
	m_outputs[0].setConstant(constant);
	double base = m_parent.m_bpc.getHertz();
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		if(constant) {
			m_outputs[0].setValue(c, 0, base * *multIter);
		} else {
			for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
				*freqOutput = base * *multIter;
				automation.incrementPosition();
				io.incrementPosition();
			}
		}
		automation.incrementChannel();
		io.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void LfoTunerAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

