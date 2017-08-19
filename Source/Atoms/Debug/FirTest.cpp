/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[190.000000:1.000000:0.000000:sFir Tester:sDebug:ssignal:ssignal:swave:swave:[[[4.000000:9.500000:1.000000:2.000000:2.000000:1.000000:sSimpleKnob:sco4:sCoefficient 4:]][0.000000:1.000000:0.500000:0.000000:1.000000:s:]][[[4.000000:0.500000:1.000000:2.000000:2.000000:1.000000:sSimpleKnob:sco1:sCoefficient 1:]][0.000000:1.000000:0.500000:0.000000:1.000000:s:]][[[4.000000:3.500000:1.000000:2.000000:2.000000:1.000000:sSimpleKnob:sco2:sCoefficient 2:]][0.000000:1.000000:0.500000:0.000000:1.000000:s:]][[[4.000000:6.500000:1.000000:2.000000:2.000000:1.000000:sSimpleKnob:sco3:sCoefficient 3:]][0.000000:1.000000:0.500000:0.000000:1.000000:s:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "FirTest.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

FirTestController::FirTestController() :
		AtomController(AtomParameters(190, 1, false, 1)) {
	init();

	addInputIcon("wave");
	addOutputIcon("wave");

	m_gui.addComponent(&m_co4);
	m_co4.setBounds(CB(9.500000, 1.000000, 2.000000, 2.000000));
	m_co4.addListener(this);
	m_co4.setRange(0.000000, 1.000000);
	m_co4.setBounded(true);
	m_co4.setValue(0.500000);
	m_gui.addComponent(m_co4.createLabel("Coefficient 4", true));

	m_gui.addComponent(&m_co1);
	m_co1.setBounds(CB(0.500000, 1.000000, 2.000000, 2.000000));
	m_co1.addListener(this);
	m_co1.setRange(0.000000, 1.000000);
	m_co1.setBounded(true);
	m_co1.setValue(0.500000);
	m_gui.addComponent(m_co1.createLabel("Coefficient 1", true));

	m_gui.addComponent(&m_co2);
	m_co2.setBounds(CB(3.500000, 1.000000, 2.000000, 2.000000));
	m_co2.addListener(this);
	m_co2.setRange(0.000000, 1.000000);
	m_co2.setBounded(true);
	m_co2.setValue(0.500000);
	m_gui.addComponent(m_co2.createLabel("Coefficient 2", true));

	m_gui.addComponent(&m_co3);
	m_co3.setBounds(CB(6.500000, 1.000000, 2.000000, 2.000000));
	m_co3.addListener(this);
	m_co3.setRange(0.000000, 1.000000);
	m_co3.setBounded(true);
	m_co3.setValue(0.500000);
	m_gui.addComponent(m_co3.createLabel("Coefficient 3", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * FirTestController::createAtom(int index) {
	return new FirTestAtom(*this, index);
}

SaveState FirTestController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_co4.saveSaveState());
	extraData.addState(m_co1.saveSaveState());
	extraData.addState(m_co2.saveSaveState());
	extraData.addState(m_co3.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void FirTestController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1) {
		m_co4.loadSaveState(extraData.getNextState());
		m_co1.loadSaveState(extraData.getNextState());
		m_co2.loadSaveState(extraData.getNextState());
		m_co3.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void FirTestController::simpleKnobChanged(SimpleKnob * knob) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

FirTestAtom::FirTestAtom(FirTestController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_buffer.setSize(AudioBuffer::getDefaultSamples() + 260);
	/* END USER-DEFINED CONSTRUCTION CODE */
}

void FirTestAtom::execute() {
	Atom::execute();

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if(m_primaryInputs[0] != nullptr) {
		m_buffer.copyData(*m_primaryInputs[0], m_buffer.getSize() - m_primaryInputs[0]->getSize());
	}
	m_filter.compute(m_buffer, m_outputs[0]);
	m_buffer.offsetData(-AudioBuffer::getDefaultSize());
	/* END USER-DEFINED EXECUTION CODE */
}

void FirTestAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	m_buffer.fill(0.0);
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

