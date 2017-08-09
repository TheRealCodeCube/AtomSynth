/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[81.000000:1.000000:1.000000:sPitch Mixer:sTransforms:sf1, f2:sf:shz, hz:shz:[[[24.000000:8.000000:1.000000:2.000000:2.000000:1.000000:sMultiButton:srightSource:sSource:]][1.000000:0.000000:sInput 1:sInput 2:s440Hz:]][[[4.000000:8.000000:3.500000:2.000000:2.000000:1.000000:sOctavesKnob:srightOctaves:sOctave Detune:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]][[[4.000000:8.000000:6.000000:2.000000:2.000000:1.000000:sSemitonesKnob:srightSemis:sSemitone Detune:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]][[[4.000000:8.000000:8.500000:2.000000:2.000000:1.000000:sCentsKnob:srightCents:sCents Detune:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:2.000000:8.500000:2.000000:2.000000:1.000000:sCentsKnob:sleftCents:sCents Detune:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:2.000000:6.000000:2.000000:2.000000:1.000000:sSemitonesKnob:sleftSemis:sSemitone Detune:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]][[[4.000000:2.000000:3.500000:2.000000:2.000000:1.000000:sOctavesKnob:sleftOctaves:sOctave Detune:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]][[[24.000000:2.000000:1.000000:2.000000:2.000000:1.000000:sMultiButton:sleftSource:sSource:]][1.000000:0.000000:sInput 1:sInput 2:s440Hz:]][[[24.000000:7.000000:9.000000:1.500000:1.000000:1.000000:sArrow:sArrow:s:]][0.000000:2.000000:]][[[24.000000:3.500000:9.000000:1.500000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[4.000000:5.000000:8.500000:2.000000:2.000000:1.000000:sAtomKnob:sblend:sBlend:]][0.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[24.000000:0.000000:1.500000:2.000000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:1.000000:]][[[24.000000:5.500000:10.000000:1.000000:2.000000:0.000000:sArrow:sArrow:sOutput:]][2.000000:0.000000:]][[[24.000000:10.000000:1.500000:2.000000:1.000000:1.000000:sArrow:sArrow:s:]][1.000000:2.000000:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Pitch.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Adsp/Interpolation.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

PitchController::PitchController() :
		AtomController(AtomParameters(81, 2, true, 1)) {
	init();

	addInputIcon("hz");
	addInputIcon("hz");
	addOutputIcon("hz");

	m_gui.addComponent(&m_rightSource);
	m_rightSource.setBounds(CB(8.000000, 1.000000, 2.000000, 2.000000));
	m_rightSource.addListener(this);
	m_rightSource.setVertical(true);
	m_rightSource.addLabel("Input 1");
	m_rightSource.addLabel("Input 2");
	m_rightSource.addLabel("440Hz");
	m_gui.addComponent(m_rightSource.createLabel("Source", true));

	m_gui.addComponent(&m_rightOctaves);
	m_rightOctaves.setBounds(CB(8.000000, 3.500000, 2.000000, 2.000000));
	m_rightOctaves.addListener(this);
	addAutomatedControl(m_rightOctaves, m_rightOctavesIter);
	m_gui.addComponent(m_rightOctaves.createLabel("Octave Detune", true));

	m_gui.addComponent(&m_rightSemis);
	m_rightSemis.setBounds(CB(8.000000, 6.000000, 2.000000, 2.000000));
	m_rightSemis.addListener(this);
	addAutomatedControl(m_rightSemis, m_rightSemisIter);
	m_gui.addComponent(m_rightSemis.createLabel("Semitone Detune", true));

	m_gui.addComponent(&m_rightCents);
	m_rightCents.setBounds(CB(8.000000, 8.500000, 2.000000, 2.000000));
	m_rightCents.addListener(this);
	addAutomatedControl(m_rightCents, m_rightCentsIter);
	m_gui.addComponent(m_rightCents.createLabel("Cents Detune", true));

	m_gui.addComponent(&m_leftCents);
	m_leftCents.setBounds(CB(2.000000, 8.500000, 2.000000, 2.000000));
	m_leftCents.addListener(this);
	addAutomatedControl(m_leftCents, m_leftCentsIter);
	m_gui.addComponent(m_leftCents.createLabel("Cents Detune", true));

	m_gui.addComponent(&m_leftSemis);
	m_leftSemis.setBounds(CB(2.000000, 6.000000, 2.000000, 2.000000));
	m_leftSemis.addListener(this);
	addAutomatedControl(m_leftSemis, m_leftSemisIter);
	m_gui.addComponent(m_leftSemis.createLabel("Semitone Detune", true));

	m_gui.addComponent(&m_leftOctaves);
	m_leftOctaves.setBounds(CB(2.000000, 3.500000, 2.000000, 2.000000));
	m_leftOctaves.addListener(this);
	addAutomatedControl(m_leftOctaves, m_leftOctavesIter);
	m_gui.addComponent(m_leftOctaves.createLabel("Octave Detune", true));

	m_gui.addComponent(&m_leftSource);
	m_leftSource.setBounds(CB(2.000000, 1.000000, 2.000000, 2.000000));
	m_leftSource.addListener(this);
	m_leftSource.setVertical(true);
	m_leftSource.addLabel("Input 1");
	m_leftSource.addLabel("Input 2");
	m_leftSource.addLabel("440Hz");
	m_gui.addComponent(m_leftSource.createLabel("Source", true));

	m_gui.addComponent(&m_arrow0);
	m_arrow0.setBounds(CB(7.000000, 9.000000, 1.500000, 1.000000));
	m_arrow0.setCaps(2, 0);

	m_gui.addComponent(&m_arrow1);
	m_arrow1.setBounds(CB(3.500000, 9.000000, 1.500000, 1.000000));
	m_arrow1.setCaps(0, 2);

	m_gui.addComponent(&m_blend);
	m_blend.setBounds(CB(5.000000, 8.500000, 2.000000, 2.000000));
	m_blend.addListener(this);
	m_blend.setRange(0.000000, 1.000000);
	m_blend.setBounded(true);
	addAutomatedControl(m_blend, m_blendIter);
	m_gui.addComponent(m_blend.createLabel("Blend", true));

	m_gui.addComponent(&m_arrow2);
	m_arrow2.setBounds(CB(0.000000, 1.500000, 2.000000, 1.000000));
	m_arrow2.setCaps(1, 2);

	m_gui.addComponent(&m_arrow3);
	m_arrow3.setBounds(CB(5.500000, 10.000000, 1.000000, 2.000000));
	m_arrow3.setCaps(0, 2);
	m_gui.addComponent(m_arrow3.createLabel("Output", false));

	m_gui.addComponent(&m_arrow4);
	m_arrow4.setBounds(CB(10.000000, 1.500000, 2.000000, 1.000000));
	m_arrow4.setCaps(2, 1);

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * PitchController::createAtom(int index) {
	return new PitchAtom(*this, index);
}

SaveState PitchController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_rightSource.saveSaveState());
	extraData.addState(m_rightOctaves.saveSaveState());
	extraData.addState(m_rightSemis.saveSaveState());
	extraData.addState(m_rightCents.saveSaveState());
	extraData.addState(m_leftCents.saveSaveState());
	extraData.addState(m_leftSemis.saveSaveState());
	extraData.addState(m_leftOctaves.saveSaveState());
	extraData.addState(m_leftSource.saveSaveState());
	extraData.addState(m_blend.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void PitchController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1) {
		m_rightSource.loadSaveState(extraData.getNextState());
		m_rightOctaves.loadSaveState(extraData.getNextState());
		m_rightSemis.loadSaveState(extraData.getNextState());
		m_rightCents.loadSaveState(extraData.getNextState());
		m_leftCents.loadSaveState(extraData.getNextState());
		m_leftSemis.loadSaveState(extraData.getNextState());
		m_leftOctaves.loadSaveState(extraData.getNextState());
		m_leftSource.loadSaveState(extraData.getNextState());
		m_blend.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void PitchController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void PitchController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

PitchAtom::PitchAtom(PitchController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void PitchAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & rightOctavesIter = m_parent.m_rightOctavesIter;
	DVecIter & rightSemisIter = m_parent.m_rightSemisIter;
	DVecIter & rightCentsIter = m_parent.m_rightCentsIter;
	DVecIter & leftCentsIter = m_parent.m_leftCentsIter;
	DVecIter & leftSemisIter = m_parent.m_leftSemisIter;
	DVecIter & leftOctavesIter = m_parent.m_leftOctavesIter;
	DVecIter & blendIter = m_parent.m_blendIter;

	IOSet io = IOSet();
	DVecIter * f1Input = io.addInput(m_primaryInputs[0]);
	DVecIter * f2Input = io.addInput(m_primaryInputs[1]);
	DVecIter & fOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	double leftBase = 1.0, rightBase = 1.0;
	bool leftOctavesConst = m_parent.m_leftOctaves.getResult().isConstant(),
			leftSemisConst = m_parent.m_leftSemis.getResult().isConstant(),
			leftCentsConst = m_parent.m_leftCents.getResult().isConstant(),
			rightOctavesConst = m_parent.m_rightOctaves.getResult().isConstant(),
			rightSemisConst = m_parent.m_rightSemis.getResult().isConstant(),
			rightCentsConst = m_parent.m_rightCents.getResult().isConstant();
	int leftSource = m_parent.m_leftSource.getSelectedLabel(),
			rightSource = m_parent.m_rightSource.getSelectedLabel();
	double leftFreq, rightFreq;
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		leftBase = (leftSource == 2) ? 440.0 : 1.0;
		rightBase = (rightSource == 2) ? 440.0 : 1.0;
		if(leftOctavesConst) leftBase = OctavesKnob::detune(leftBase, *leftOctavesIter);
		if(leftSemisConst) leftBase = SemitonesKnob::detune(leftBase, *leftSemisIter);
		if(leftCentsConst) leftBase = CentsKnob::detune(leftBase, *leftCentsIter);
		if(rightOctavesConst) rightBase = OctavesKnob::detune(rightBase, *rightOctavesIter);
		if(rightSemisConst) rightBase = SemitonesKnob::detune(rightBase, *rightSemisIter);
		if(rightCentsConst) rightBase = CentsKnob::detune(rightBase, *rightCentsIter);
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			leftFreq = leftBase;
			rightFreq = rightBase;
			//Calculate left source.
			if(leftSource == 0) {
				if(f1Input != nullptr) {
					leftFreq *= **f1Input;
				} else {
					leftFreq *= 440.0; //Default to 440 if there is no input.
				}
			} else if(leftSource == 1) {
				if(f2Input != nullptr) {
					leftFreq *= **f2Input;
				} else {
					leftFreq *= 440.0;
				}
			} //If the source is 2, it has already been multiplied by 440.
			//Calculate right source.
			if(rightSource == 0) {
				if(f1Input != nullptr) {
					rightFreq *= **f1Input;
				} else {
					rightFreq *= 440.0; //Default to 440 if there is no input.
				}
			} else if(rightSource == 1) {
				if(f2Input != nullptr) {
					rightFreq *= **f2Input;
				} else {
					rightFreq *= 440.0;
				}
			} //If the source is 2, it has already been multiplied by 440.
			//Multiply all the multipliers.
			if(!leftOctavesConst) leftFreq = OctavesKnob::detune(leftFreq, *leftOctavesIter);
			if(!leftSemisConst) leftFreq = SemitonesKnob::detune(leftFreq, *leftSemisIter);
			if(!leftCentsConst) leftFreq = CentsKnob::detune(leftFreq, *leftCentsIter);
			if(!rightOctavesConst) rightFreq = OctavesKnob::detune(rightFreq, *rightOctavesIter);
			if(!rightSemisConst) rightFreq = SemitonesKnob::detune(rightFreq, *rightSemisIter);
			if(!rightCentsConst) rightFreq = CentsKnob::detune(rightFreq, *rightCentsIter);
			*fOutput = Adsp::linearInterp(leftFreq, rightFreq, *blendIter);
			automation.incrementPosition();
			io.incrementPosition();
		}
		automation.incrementChannel();
		io.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void PitchAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

