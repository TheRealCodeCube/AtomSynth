/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[42.000000:1.000000:1.000000:sHighpass (FIR):sFilters:ssignal, cutoff:ssignal:swave, hz:swave:[[[24.000000:1.500000:1.000000:2.000000:2.000000:1.000000:sMultiButton:scutoffSource:sCutoff Source:]][1.000000:0.000000:sInput:s+-440Hz:]][[[4.000000:8.500000:1.000000:2.000000:2.000000:1.000000:sSemitonesKnob:ssemis:sSemitones:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]][[[4.000000:5.000000:1.000000:2.000000:2.000000:1.000000:sOctavesKnob:socts:sOctaves:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "FirHighpass.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Technical/Synth.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

FirHighpassController::FirHighpassController() :
		AtomController(AtomParameters(42, 2, true, 1)) {
	init();

	addInputIcon("wave");
	addInputIcon("hz");
	addOutputIcon("wave");

	m_gui.addComponent(&m_cutoffSource);
	m_cutoffSource.setBounds(CB(1.500000, 1.000000, 2.000000, 2.000000));
	m_cutoffSource.addListener(this);
	m_cutoffSource.setVertical(true);
	m_cutoffSource.addLabel("Input");
	m_cutoffSource.addLabel("+-440Hz");
	m_gui.addComponent(m_cutoffSource.createLabel("Cutoff Source", true));

	m_gui.addComponent(&m_semis);
	m_semis.setBounds(CB(8.500000, 1.000000, 2.000000, 2.000000));
	m_semis.addListener(this);
	addAutomatedControl(m_semis, m_semisIter);
	m_gui.addComponent(m_semis.createLabel("Semitones", true));

	m_gui.addComponent(&m_octs);
	m_octs.setBounds(CB(5.000000, 1.000000, 2.000000, 2.000000));
	m_octs.addListener(this);
	addAutomatedControl(m_octs, m_octsIter);
	m_gui.addComponent(m_octs.createLabel("Octaves", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * FirHighpassController::createAtom(int index) {
	return new FirHighpassAtom(*this, index);
}

SaveState FirHighpassController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_cutoffSource.saveSaveState());
	extraData.addState(m_semis.saveSaveState());
	extraData.addState(m_octs.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void FirHighpassController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1) {
		m_cutoffSource.loadSaveState(extraData.getNextState());
		m_semis.loadSaveState(extraData.getNextState());
		m_octs.loadSaveState(extraData.getNextState());//
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void FirHighpassController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void FirHighpassController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void FirHighpassAtom::recalculate(double newFreq) {
	m_filter.set(FILTER_TYPE, newFreq, Synth::getInstance()->getParameters().m_sampleRate);
}

FirHighpassAtom::FirHighpassAtom(FirHighpassController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_delayLine.setSize(AudioBuffer::getDefaultSize() + Adsp::CachedFirFilter::SIZE);
	/* END USER-DEFINED CONSTRUCTION CODE */
}

void FirHighpassAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & semisIter = m_parent.m_semisIter;
	DVecIter & octsIter = m_parent.m_octsIter;

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter * cutoffInput = io.addInput(m_primaryInputs[1]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if(m_primaryInputs[0] == nullptr) {
		m_outputs[0].fill(0.0);
	} else {
		bool animateCutoff = false;
		int cutoffSource = m_parent.m_cutoffSource.getSelectedLabel();
		if(cutoffSource == 0) {
			//Hz input.
			if(m_primaryInputs[1] == nullptr) {
				m_outputs[0].copyData(*m_primaryInputs[0]);
				return;
			} else {
				double freq = **cutoffInput;
				freq = OctavesKnob::detune(freq, *octsIter);
				freq = SemitonesKnob::detune(freq, *semisIter);
				recalculate(freq);
				animateCutoff = !(m_primaryInputs[1]->isConstant() && m_parent.m_octs.getResult().isConstant() && m_parent.m_semis.getResult().isConstant());
			}
		} else {
			//Fixed input
			double freq = OctavesKnob::detune(440.0, *octsIter);
			freq = SemitonesKnob::detune(freq, *semisIter);
			recalculate(freq);
			animateCutoff = !(m_parent.m_octs.getResult().isConstant() && m_parent.m_semis.getResult().isConstant());
		}
		m_delayLine.copyData(*m_primaryInputs[0], Adsp::CachedFirFilter::SIZE);
		for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
			for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
				if(animateCutoff) {
					if(cutoffSource == 0) {
						//Hz input.
						double freq = **cutoffInput;
						freq = OctavesKnob::detune(freq, *octsIter);
						freq = SemitonesKnob::detune(freq, *semisIter);
						recalculate(freq);
					} else {
						//Fixed input
						double freq = OctavesKnob::detune(440.0, *octsIter);
						freq = SemitonesKnob::detune(freq, *semisIter);
						recalculate(freq);
					}

				}
				*signalOutput = m_filter.compute(m_delayLine, c, s);
				automation.incrementPosition();
				io.incrementPosition();
			}
			automation.incrementChannel();
			io.incrementChannel();
		}
		m_delayLine.offsetData(-AudioBuffer::getDefaultSamples());
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void FirHighpassAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

