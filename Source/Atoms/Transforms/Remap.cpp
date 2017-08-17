/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[82.000000:2.000000:1.000000:sRemap:sTransforms:ssignal:ssignal:swave:swave:[[[24.000000:6.000000:0.500000:6.000000:1.000000:1.000000:sMultiButton:soutType:sOutput Type:]][0.000000:0.000000:sAudio:sHz (Lin):sHz (Log):]][[[24.000000:0.000000:0.500000:6.000000:1.000000:1.000000:sMultiButton:sinType:sInput Type:]][0.000000:0.000000:sAudio:sHz (Lin):sHz (Log):]][[[24.000000:1.000000:12.000000:1.000000:1.500000:0.000000:sArrow:sArrow:sInput:]][1.000000:2.000000:]][[[24.000000:10.000000:2.000000:2.000000:1.000000:1.000000:sArrow:sArrow:sOutput:]][2.000000:1.000000:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Remap.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Adsp/Basic.h"
#include "Adsp/Remap.h"
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
	m_arrow0.setBounds(CB(1.000000, 12.000000, 1.000000, 1.500000));
	m_arrow0.setCaps(2, 1);
	m_gui.addComponent(m_arrow0.createLabel("Input", false));

	m_gui.addComponent(&m_arrow1);
	m_arrow1.setBounds(CB(10.000000, 2.000000, 2.000000, 1.000000));
	m_arrow1.setCaps(1, 2);
	m_gui.addComponent(m_arrow1.createLabel("Output", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_gui.addAndMakeVisible(m_plot);
	m_plot.setBounds(CB(0, 2, 10, 10));
	m_plot.setLength(128);
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
	extraData.addValue(2); //Store the revision this was saved with, to preserve backwards compatibility.
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
	if(version == 2) {
		m_outType.loadSaveState(extraData.getNextState());
		m_inType.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void RemapController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	if(button == &m_inType) {
		int selected = m_inType.getSelectedLabel();
		m_plot.setXSkew((selected == 2) ? 2.0 : 1.0);
		setInputIcon(0, (selected == 0) ? "wave" : "hz");
	} else if(button == &m_outType) {
		int selected = m_outType.getSelectedLabel();
		m_plot.setYSkew((selected == 2) ? 2.0 : 1.0);
		setOutputIcon(0, (selected == 0) ? "wave" : "hz");
	}
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
	if(signalInput == nullptr) return;
	double value;
	int inType = m_parent.m_inType.getSelectedLabel(),
			outType = m_parent.m_outType.getSelectedLabel();
	if(m_shouldUpdateParent) {
		value = **signalInput;
		if(inType == 0) { //Audio
			value = Adsp::remap(value, -1.0, 1.0, 0.0, 1.0);
		} else if(inType == 1) { //Linear Hz
			value = Adsp::remap(value, 0, 20000.0, 0.0, 1.0);
		} else if(inType == 2) { //Logarithmic Hz
			value = std::log2(value) / LOG_MAX;
		}
		double y = m_parent.m_plot.getValueAt(value);
		y = Adsp::remap(y, -1.0, 1.0, 1.0, 0.0);
		m_parent.m_plot.setCursorPos(value, y);
		m_parent.m_plot.setCursorMode(PlotBase::CursorMode::CROSSHAIR);
		m_parent.m_plot.repaintAsync();
	}
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			value = **signalInput;
			if(inType == 0) { //Audio
				value = Adsp::remap(value, -1.0, 1.0, 0.0, 1.0);
			} else if(inType == 1) { //Linear Hz
				value = Adsp::remap(value, 0, 20000.0, 0.0, 1.0);
			} else if(inType == 2) { //Logarithmic Hz
				value = std::log2(value) / LOG_MAX;
			}
			value = m_parent.m_plot.getValueAt(Adsp::clip(value, 0.0, 1.0));
			if(outType == 1) {
				value = Adsp::remap(value, -1.0, 1.0, 0.0, 20000.0);
			} else if(outType == 2) {
				value = std::pow(2.0, Adsp::remap(value, -1.0, 1.0, 0.0, 1.0) * LOG_MAX);
			}
			*signalOutput = value;
			io.incrementPosition();
		}
		io.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void RemapAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	if(m_shouldUpdateParent) {
		m_parent.m_plot.setCursorMode(PlotBase::CursorMode::NONE);
		m_parent.m_plot.repaintAsync();
	}
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

