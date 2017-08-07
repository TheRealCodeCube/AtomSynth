/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[40.000000:1.000000:1.000000:sComb Filter:sFilters:ssignal, freq:ssignal:swave, hz:swave:[[[24.000000:1.000000:9.500000:1.000000:3.500000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:10.000000:2.500000:1.000000:3.500000:1.000000:sArrow:sArrow:s:]][2.000000:3.000000:]][[[24.000000:1.000000:8.500000:1.000000:2.000000:1.000000:sArrow:sArrow:s:]][3.000000:0.000000:]][[[24.000000:1.000000:2.500000:10.000000:1.000000:1.000000:sArrow:sArrow:s:]][0.000000:0.000000:]][[[24.000000:1.000000:2.500000:1.000000:4.000000:1.000000:sArrow:sArrow:s:]][0.000000:3.000000:]][[[24.000000:1.500000:9.500000:9.500000:1.000000:1.000000:sArrow:sArrow:s:]][0.000000:2.000000:]][[[24.000000:10.000000:10.000000:1.000000:3.000000:1.000000:sArrow:sArrow:s:]][1.000000:1.000000:]][[[24.000000:10.000000:9.000000:1.000000:1.500000:1.000000:sArrow:sArrow:s:]][3.000000:2.000000:]][[[24.000000:2.000000:4.000000:2.000000:2.000000:1.000000:sMultiButton:sdelaySource:sDelay Source:]][1.000000:0.000000:sHz Knob:sTime Knob:sHz Input:]][[[4.000000:5.000000:4.000000:2.000000:2.000000:1.000000:sAtomKnob:sdelayTime:sTime:]][0.010000:1.000000:0.500000:0.000000:1.000000:sms:]][[[24.000000:8.000000:4.000000:2.000000:2.000000:1.000000:sMultiButton:stimeRange:sTime Range:]][1.000000:0.000000:s1ms:s10ms:s100ms:s1s:]][[[24.000000:8.000000:4.000000:2.000000:2.000000:1.000000:sMultiButton:shertzRange:sHertz Range:]][1.000000:0.000000:s1Hz:s10Hz:s100Hz:s1kHz:]][[[4.000000:5.000000:4.000000:2.000000:2.000000:1.000000:sAtomKnob:sdelayHz:sHertz:]][0.010000:1.000000:0.500000:0.000000:1.000000:sHz:]][[[4.000000:5.000000:10.500000:2.000000:2.000000:1.000000:sAtomKnob:sdryMix:sDry Mix:]][0.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:5.000000:0.500000:2.000000:2.000000:1.000000:sAtomKnob:sfeedback:sFeedback Amplitude:]][0.000000:1.000000:0.000000:0.000000:1.000000:s:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Comb.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

CombController::CombController() :
		AtomController(AtomParameters(40, 2, true, 1)) {
	init();

	addInputIcon("wave");
	addInputIcon("hz");
	addOutputIcon("wave");

	m_gui.addComponent(&m_arrow0);
	m_arrow0.setBounds(CB(1.000000, 9.500000, 1.000000, 3.500000));
	m_arrow0.setCaps(0, 2);

	m_gui.addComponent(&m_arrow1);
	m_arrow1.setBounds(CB(10.000000, 2.500000, 1.000000, 3.500000));
	m_arrow1.setCaps(3, 2);

	m_gui.addComponent(&m_arrow2);
	m_arrow2.setBounds(CB(1.000000, 8.500000, 1.000000, 2.000000));
	m_arrow2.setCaps(0, 3);

	m_gui.addComponent(&m_arrow3);
	m_arrow3.setBounds(CB(1.000000, 2.500000, 10.000000, 1.000000));
	m_arrow3.setCaps(0, 0);

	m_gui.addComponent(&m_arrow4);
	m_arrow4.setBounds(CB(1.000000, 2.500000, 1.000000, 4.000000));
	m_arrow4.setCaps(3, 0);

	m_gui.addComponent(&m_arrow5);
	m_arrow5.setBounds(CB(1.500000, 9.500000, 9.500000, 1.000000));
	m_arrow5.setCaps(2, 0);

	m_gui.addComponent(&m_arrow6);
	m_arrow6.setBounds(CB(10.000000, 10.000000, 1.000000, 3.000000));
	m_arrow6.setCaps(1, 1);

	m_gui.addComponent(&m_arrow7);
	m_arrow7.setBounds(CB(10.000000, 9.000000, 1.000000, 1.500000));
	m_arrow7.setCaps(2, 3);

	m_gui.addComponent(&m_delaySource);
	m_delaySource.setBounds(CB(2.000000, 4.000000, 2.000000, 2.000000));
	m_delaySource.addListener(this);
	m_delaySource.setVertical(true);
	m_delaySource.addLabel("Hz Knob");
	m_delaySource.addLabel("Time Knob");
	m_delaySource.addLabel("Hz Input");
	m_gui.addComponent(m_delaySource.createLabel("Delay Source", true));

	m_gui.addComponent(&m_delayTime);
	m_delayTime.setBounds(CB(5.000000, 4.000000, 2.000000, 2.000000));
	m_delayTime.addListener(this);
	m_delayTime.setRange(0.010000, 1.000000);
	m_delayTime.setValue(0.500000);
	m_delayTime.setBounded(true);
	m_delayTime.setSuffix("ms");
	addAutomatedControl(m_delayTime, m_delayTimeIter);
	m_gui.addComponent(m_delayTime.createLabel("Time", true));

	m_gui.addComponent(&m_timeRange);
	m_timeRange.setBounds(CB(8.000000, 4.000000, 2.000000, 2.000000));
	m_timeRange.addListener(this);
	m_timeRange.setVertical(true);
	m_timeRange.addLabel("1ms");
	m_timeRange.addLabel("10ms");
	m_timeRange.addLabel("100ms");
	m_timeRange.addLabel("1s");
	m_gui.addComponent(m_timeRange.createLabel("Time Range", true));

	m_gui.addComponent(&m_hertzRange);
	m_hertzRange.setBounds(CB(8.000000, 4.000000, 2.000000, 2.000000));
	m_hertzRange.addListener(this);
	m_hertzRange.setVertical(true);
	m_hertzRange.addLabel("1Hz");
	m_hertzRange.addLabel("10Hz");
	m_hertzRange.addLabel("100Hz");
	m_hertzRange.addLabel("1kHz");
	m_gui.addComponent(m_hertzRange.createLabel("Hertz Range", true));

	m_gui.addComponent(&m_delayHz);
	m_delayHz.setBounds(CB(5.000000, 4.000000, 2.000000, 2.000000));
	m_delayHz.addListener(this);
	m_delayHz.setRange(0.010000, 1.000000);
	m_delayHz.setValue(0.500000);
	m_delayHz.setBounded(true);
	m_delayHz.setSuffix("Hz");
	addAutomatedControl(m_delayHz, m_delayHzIter);
	m_gui.addComponent(m_delayHz.createLabel("Hertz", true));

	m_gui.addComponent(&m_dryMix);
	m_dryMix.setBounds(CB(5.000000, 10.500000, 2.000000, 2.000000));
	m_dryMix.addListener(this);
	m_dryMix.setRange(0.000000, 1.000000);
	m_dryMix.setBounded(true);
	addAutomatedControl(m_dryMix, m_dryMixIter);
	m_gui.addComponent(m_dryMix.createLabel("Dry Mix", true));

	m_gui.addComponent(&m_feedback);
	m_feedback.setBounds(CB(5.000000, 0.500000, 2.000000, 2.000000));
	m_feedback.addListener(this);
	m_feedback.setRange(0.000000, 1.000000);
	m_feedback.setBounded(true);
	addAutomatedControl(m_feedback, m_feedbackIter);
	m_gui.addComponent(m_feedback.createLabel("Feedback Amplitude", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * CombController::createAtom(int index) {
	return new CombAtom(*this, index);
}

SaveState CombController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_delaySource.saveSaveState());
	extraData.addState(m_delayTime.saveSaveState());
	extraData.addState(m_timeRange.saveSaveState());
	extraData.addState(m_hertzRange.saveSaveState());
	extraData.addState(m_delayHz.saveSaveState());
	extraData.addState(m_dryMix.saveSaveState());
	extraData.addState(m_feedback.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void CombController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1) {
		m_delaySource.loadSaveState(extraData.getNextState());
		m_delayTime.loadSaveState(extraData.getNextState());
		m_timeRange.loadSaveState(extraData.getNextState());
		m_hertzRange.loadSaveState(extraData.getNextState());
		m_delayHz.loadSaveState(extraData.getNextState());
		m_dryMix.loadSaveState(extraData.getNextState());
		m_feedback.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void CombController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void CombController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

CombAtom::CombAtom(CombController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void CombAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & delayTimeIter = m_parent.m_delayTimeIter;
	DVecIter & delayHzIter = m_parent.m_delayHzIter;
	DVecIter & dryMixIter = m_parent.m_dryMixIter;
	DVecIter & feedbackIter = m_parent.m_feedbackIter;

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter * freqInput = io.addInput(m_primaryInputs[1]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if(m_primaryInputs[0] != nullptr)
		m_delayLine.copyData(* m_primaryInputs[0], 5000);
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			*signalOutput = m_delayLine.get(c, s);
			automation.incrementPosition();
			io.incrementPosition();
		}
		automation.incrementChannel();
		io.incrementChannel();
	}
	m_delayLine.offsetData(-AudioBuffer::getDefaultSamples());
	/* END USER-DEFINED EXECUTION CODE */
}

void CombAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	m_delayLine.fill(0.0);
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

