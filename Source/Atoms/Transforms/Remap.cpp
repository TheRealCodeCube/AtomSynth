/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[82.000000:1.000000:1.000000:sRemap:sTransforms:ssignal:ssignal:swave:swave:[[[24.000000:6.000000:0.500000:6.000000:1.000000:1.000000:sMultiButton:soutType:sOutput Type:]][0.000000:0.000000:sAudio:sHz (Lin):sHz (Log):]][[[24.000000:0.000000:0.500000:6.000000:1.000000:1.000000:sMultiButton:sinType:sInput Type:]][0.000000:0.000000:sAudio:sHz (Lin):sHz (Log):]][[[24.000000:10.000000:11.500000:1.000000:2.500000:0.000000:sArrow:sArrow:s:]][0.000000:0.000000:]][[[24.000000:0.000000:2.000000:2.000000:1.000000:1.000000:sArrow:sArrow:sInput:]][2.000000:1.000000:]][[[24.000000:10.000000:13.000000:2.000000:1.000000:0.000000:sArrow:sArrow:sOutput:]][2.000000:3.000000:]]]]

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

	m_gui.addComponent(&m_outType);
	m_outType.setBounds(CB(6.000000, 0.500000, 6.000000, 1.000000));
	m_outType.addListener(this);
	m_outType.addLabel("Audio");
	m_outType.addLabel("Hz (Lin)");
	m_outType.addLabel("Hz (Log)");
	m_gui.addComponent(m_outType.createLabel("Output Type", true));

	m_gui.addComponent(&m_inType);
	m_inType.setBounds(CB(0.000000, 0.500000, 6.000000, 1.000000));
	m_inType.addListener(this);
	m_inType.addLabel("Audio");
	m_inType.addLabel("Hz (Lin)");
	m_inType.addLabel("Hz (Log)");
	m_gui.addComponent(m_inType.createLabel("Input Type", true));

	m_gui.addComponent(&m_arrow0);
	m_arrow0.setBounds(CB(10.000000, 11.500000, 1.000000, 2.500000));
	m_arrow0.setCaps(0, 0);

	m_gui.addComponent(&m_arrow1);
	m_arrow1.setBounds(CB(0.000000, 2.000000, 2.000000, 1.000000));
	m_arrow1.setCaps(1, 2);
	m_gui.addComponent(m_arrow1.createLabel("Input", true));

	m_gui.addComponent(&m_arrow2);
	m_arrow2.setBounds(CB(10.000000, 13.000000, 2.000000, 1.000000));
	m_arrow2.setCaps(3, 2);
	m_gui.addComponent(m_arrow2.createLabel("Output", false));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_gui.addAndMakeVisible(m_plot);
	m_plot.setBounds(CB(2, 2, 10, 10));
	m_plot.createDiagonalLine();
	m_gui.addComponent(m_plot.createLabel("Curve"));
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
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_outType.saveSaveState());
	extraData.addState(m_inType.saveSaveState());
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
	if(version == 1) {
		m_outType.loadSaveState(extraData.getNextState());
		m_inType.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void RemapController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
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

