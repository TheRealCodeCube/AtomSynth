/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[40.000000:4.000000:1.000000:sComb Filter:sFilters:ssignal, freq:ssignal:swave, hz:swave:[[[24.000000:1.000000:10.000000:1.000000:2.500000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:10.000000:7.000000:1.000000:4.000000:1.000000:sArrow:sArrow:s:]][3.000000:0.000000:]][[[24.000000:1.500000:10.000000:9.500000:1.000000:1.000000:sArrow:sArrow:s:]][0.000000:2.000000:]][[[24.000000:1.000000:7.000000:9.500000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:10.000000:10.000000:1.000000:2.500000:1.000000:sArrow:sArrow:s:]][1.000000:0.000000:]][[[24.000000:2.000000:1.000000:2.000000:2.000000:1.000000:sMultiButton:sdelaySource:sDelay Source:]][1.000000:0.000000:sHz Knob:sTime Knob:sHz Input:]][[[4.000000:5.000000:1.000000:2.000000:2.000000:1.000000:sAtomKnob:sdelayTime:sTime:]][0.010000:1.000000:0.500000:0.000000:1.000000:sms:]][[[24.000000:8.000000:1.000000:2.000000:2.000000:1.000000:sMultiButton:stimeRange:sTime Range:]][1.000000:0.000000:s1ms:s10ms:s100ms:s1s:]][[[24.000000:8.000000:1.000000:2.000000:2.000000:1.000000:sMultiButton:shertzRange:sHertz Range:]][1.000000:0.000000:s10Hz:s100Hz:s1kHz:s10kHz:]][[[4.000000:5.000000:1.000000:2.000000:2.000000:1.000000:sAtomKnob:sdelayHz:sHertz:]][1.000000:10.000000:5.000000:0.000000:1.000000:sHz:]][[[4.000000:2.500000:8.000000:2.000000:2.000000:0.000000:sAtomKnob:sdryMix:sDry Amplitude:]][0.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:7.500000:8.000000:2.000000:2.000000:1.000000:sAtomKnob:sfeedback:sFeedback Amplitude:]][0.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[24.000000:10.000000:6.500000:1.000000:1.500000:1.000000:sArrow:sArrow:s:]][3.000000:2.000000:]][[[24.000000:1.000000:6.000000:1.000000:2.000000:1.000000:sArrow:sArrow:s:]][3.000000:0.000000:]][[[24.000000:1.000000:7.000000:1.000000:4.000000:1.000000:sArrow:sArrow:s:]][3.000000:0.000000:]][[[4.000000:5.000000:1.000000:2.000000:2.000000:1.000000:sOctavesKnob:soctaves:sOctave Detune:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]][[[4.000000:8.000000:1.000000:2.000000:2.000000:1.000000:sSemitonesKnob:ssemitones:sSemitone Detune:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]][[24.000000:1.000000:3.500000:10.000000:3.000000:1.000000:sWaveformPlot:splot:s:]]]]

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
	m_arrow0.setBounds(CB(1.000000, 10.000000, 1.000000, 2.500000));
	m_arrow0.setCaps(0, 2);

	m_gui.addComponent(&m_arrow1);
	m_arrow1.setBounds(CB(10.000000, 7.000000, 1.000000, 4.000000));
	m_arrow1.setCaps(0, 3);

	m_gui.addComponent(&m_arrow2);
	m_arrow2.setBounds(CB(1.500000, 10.000000, 9.500000, 1.000000));
	m_arrow2.setCaps(2, 0);

	m_gui.addComponent(&m_arrow3);
	m_arrow3.setBounds(CB(1.000000, 7.000000, 9.500000, 1.000000));
	m_arrow3.setCaps(0, 2);

	m_gui.addComponent(&m_arrow4);
	m_arrow4.setBounds(CB(10.000000, 10.000000, 1.000000, 2.500000));
	m_arrow4.setCaps(0, 1);

	m_gui.addComponent(&m_delaySource);
	m_delaySource.setBounds(CB(2.000000, 1.000000, 2.000000, 2.000000));
	m_delaySource.addListener(this);
	m_delaySource.setVertical(true);
	m_delaySource.addLabel("Hz Knob");
	m_delaySource.addLabel("Time Knob");
	m_delaySource.addLabel("Hz Input");
	m_gui.addComponent(m_delaySource.createLabel("Delay Source", true));

	m_gui.addComponent(&m_delayTime);
	m_delayTime.setBounds(CB(5.000000, 1.000000, 2.000000, 2.000000));
	m_delayTime.addListener(this);
	m_delayTime.setRange(0.010000, 1.000000);
	m_delayTime.setValue(0.500000);
	m_delayTime.setBounded(true);
	m_delayTime.setSuffix("ms");
	addAutomatedControl(m_delayTime, m_delayTimeIter);
	m_gui.addComponent(m_delayTime.createLabel("Time", true));

	m_gui.addComponent(&m_timeRange);
	m_timeRange.setBounds(CB(8.000000, 1.000000, 2.000000, 2.000000));
	m_timeRange.addListener(this);
	m_timeRange.setVertical(true);
	m_timeRange.addLabel("1ms");
	m_timeRange.addLabel("10ms");
	m_timeRange.addLabel("100ms");
	m_timeRange.addLabel("1s");
	m_gui.addComponent(m_timeRange.createLabel("Time Range", true));

	m_gui.addComponent(&m_hertzRange);
	m_hertzRange.setBounds(CB(8.000000, 1.000000, 2.000000, 2.000000));
	m_hertzRange.addListener(this);
	m_hertzRange.setVertical(true);
	m_hertzRange.addLabel("10Hz");
	m_hertzRange.addLabel("100Hz");
	m_hertzRange.addLabel("1kHz");
	m_hertzRange.addLabel("10kHz");
	m_gui.addComponent(m_hertzRange.createLabel("Hertz Range", true));

	m_gui.addComponent(&m_delayHz);
	m_delayHz.setBounds(CB(5.000000, 1.000000, 2.000000, 2.000000));
	m_delayHz.addListener(this);
	m_delayHz.setRange(1.000000, 10.000000);
	m_delayHz.setValue(5.000000);
	m_delayHz.setBounded(true);
	m_delayHz.setSuffix("Hz");
	addAutomatedControl(m_delayHz, m_delayHzIter);
	m_gui.addComponent(m_delayHz.createLabel("Hertz", true));

	m_gui.addComponent(&m_dryMix);
	m_dryMix.setBounds(CB(2.500000, 8.000000, 2.000000, 2.000000));
	m_dryMix.addListener(this);
	m_dryMix.setRange(0.000000, 1.000000);
	m_dryMix.setBounded(true);
	addAutomatedControl(m_dryMix, m_dryMixIter);
	m_gui.addComponent(m_dryMix.createLabel("Dry Amplitude", false));

	m_gui.addComponent(&m_feedback);
	m_feedback.setBounds(CB(7.500000, 8.000000, 2.000000, 2.000000));
	m_feedback.addListener(this);
	m_feedback.setRange(0.000000, 1.000000);
	m_feedback.setBounded(true);
	addAutomatedControl(m_feedback, m_feedbackIter);
	m_gui.addComponent(m_feedback.createLabel("Feedback Amplitude", true));

	m_gui.addComponent(&m_arrow5);
	m_arrow5.setBounds(CB(10.000000, 6.500000, 1.000000, 1.500000));
	m_arrow5.setCaps(2, 3);

	m_gui.addComponent(&m_arrow6);
	m_arrow6.setBounds(CB(1.000000, 6.000000, 1.000000, 2.000000));
	m_arrow6.setCaps(0, 3);

	m_gui.addComponent(&m_arrow7);
	m_arrow7.setBounds(CB(1.000000, 7.000000, 1.000000, 4.000000));
	m_arrow7.setCaps(0, 3);

	m_gui.addComponent(&m_octaves);
	m_octaves.setBounds(CB(5.000000, 1.000000, 2.000000, 2.000000));
	m_octaves.addListener(this);
	addAutomatedControl(m_octaves, m_octavesIter);
	m_gui.addComponent(m_octaves.createLabel("Octave Detune", true));

	m_gui.addComponent(&m_semitones);
	m_semitones.setBounds(CB(8.000000, 1.000000, 2.000000, 2.000000));
	m_semitones.addListener(this);
	addAutomatedControl(m_semitones, m_semitonesIter);
	m_gui.addComponent(m_semitones.createLabel("Semitone Detune", true));

	m_gui.addComponent(&m_plot);
	m_plot.setBounds(CB(1.000000, 3.500000, 10.000000, 3.000000));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	multiButtonPressed(&m_delaySource); //Show / hide things based on the selected delay source.
	m_plot.setLines(0, 0);
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
	extraData.addValue(4); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_delaySource.saveSaveState());
	extraData.addState(m_delayTime.saveSaveState());
	extraData.addState(m_timeRange.saveSaveState());
	extraData.addState(m_hertzRange.saveSaveState());
	extraData.addState(m_delayHz.saveSaveState());
	extraData.addState(m_dryMix.saveSaveState());
	extraData.addState(m_feedback.saveSaveState());
	extraData.addState(m_octaves.saveSaveState());
	extraData.addState(m_semitones.saveSaveState());
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
	if(version == 4) {
		m_delaySource.loadSaveState(extraData.getNextState());
		m_delayTime.loadSaveState(extraData.getNextState());
		m_timeRange.loadSaveState(extraData.getNextState());
		m_hertzRange.loadSaveState(extraData.getNextState());
		m_delayHz.loadSaveState(extraData.getNextState());
		m_dryMix.loadSaveState(extraData.getNextState());
		m_feedback.loadSaveState(extraData.getNextState());
		m_octaves.loadSaveState(extraData.getNextState());
		m_semitones.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	else if(version == 3) {
		m_delaySource.loadSaveState(extraData.getNextState());
		m_delayTime.loadSaveState(extraData.getNextState());
		m_timeRange.loadSaveState(extraData.getNextState());
		m_hertzRange.loadSaveState(extraData.getNextState());
		m_delayHz.loadSaveState(extraData.getNextState());
		m_dryMix.loadSaveState(extraData.getNextState());
		m_feedback.loadSaveState(extraData.getNextState());
		m_octaves.loadSaveState(extraData.getNextState());
		m_semitones.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	else if(version == 2) {
		m_delaySource.loadSaveState(extraData.getNextState());
		m_delayTime.loadSaveState(extraData.getNextState());
		m_timeRange.loadSaveState(extraData.getNextState());
		m_hertzRange.loadSaveState(extraData.getNextState());
		m_delayHz.loadSaveState(extraData.getNextState());
		m_dryMix.loadSaveState(extraData.getNextState());
		m_feedback.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */
		multiButtonPressed(&m_delaySource); //Show / hide things based on the selected delay source.
		multiButtonPressed(&m_hertzRange); //Update knob range based on selected range.
		multiButtonPressed(&m_timeRange); //Update knob range based on selected range.
		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void CombController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	int selected = button->getSelectedLabel();
	if(button == &m_delaySource) {
		double hzVisible = selected == 0, //Hz knob
				timeVisible = selected == 1, //Time knob
				detuneVisible = selected == 2; //Hz input
		m_delayHz.setVisible(hzVisible);
		m_hertzRange.setVisible(hzVisible);
		m_delayTime.setVisible(timeVisible);
		m_timeRange.setVisible(timeVisible);
		m_octaves.setVisible(detuneVisible);
		m_semitones.setVisible(detuneVisible);
	} else if(button == &m_hertzRange) {
		double min = (selected > 1) ? 0.01 : 1.0, max; //Labels 2 + 3 are kHz. Delay should never be greater than 1 second.
		switch(selected) {
		case 0: //10Hz
			max = 10.0;
			break;
		case 1: //100Hz
			max = 100.0;
			break;
		case 2: //1kHz
			max = 1.0;
			break;
		case 3: //10kHz
			max = 10.0;
			break;
		}
		m_delayHz.setRange(min, max);
		m_delayHz.setValue(max / 2.0);
		m_delayHz.setSuffix((selected > 1) ? "kHz" : "Hz"); //Labels 2 + 3 are kHz instead of hZ
		m_delayHz.repaint();
	} else if(button == &m_timeRange) {
		double max;
		switch(selected) {
		case 0: //1ms
			max = 1.0;
			break;
		case 1: //10ms
			max = 10.0;
			break;
		case 2: //100ms
			max = 100.0;
			break;
		case 3: //1s
			max = 1.0;
			break;
		}
		m_delayTime.setRange(0.0, max);
		m_delayTime.setValue(max / 2.0);
		m_delayTime.setSuffix((selected > 2) ? "s" : "ms"); //Label 3 is s instead of ms
		m_delayTime.repaint();
	}
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
	DVecIter & octavesIter = m_parent.m_octavesIter;
	DVecIter & semitonesIter = m_parent.m_semitonesIter;

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter * freqInput = io.addInput(m_primaryInputs[1]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	int offset = m_delayLine.getSize() - AudioBuffer::getDefaultSamples();

	double delay = 0.0, value, detune = 1.0;
	int delayRange, delaySource = m_parent.m_delaySource.getSelectedLabel();
	bool delaySourceConstant, detuneConstant;
	if(delaySource == 0) { //Hz Knob
		delaySourceConstant = m_parent.m_delayHz.getResult().isConstant();
		delayRange = m_parent.m_hertzRange.getSelectedLabel();
	} else if(delaySource == 1) { //Time Knob
		delaySourceConstant = m_parent.m_delayTime.getResult().isConstant();
		delayRange = m_parent.m_timeRange.getSelectedLabel();
	} else if(delaySource == 2) { //Hz input
		if(m_primaryInputs[0] == nullptr) {
			delay = 0.1; //Fixed .1 second delay.
			delaySource = 10; //There is no -1, so no recalculation will be done.
		} else {
			delaySourceConstant = m_primaryInputs[0]->isConstant();
			detuneConstant = m_parent.m_octaves.getResult().isConstant() && m_parent.m_semitones.getResult().isConstant();
		}
	}

	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		if(detuneConstant) {
			detune = OctavesKnob::detune(SemitonesKnob::detune(1.0, *semitonesIter), *octavesIter);
		}
		if(delaySourceConstant) {
			if(delaySource == 0) { //Hz Knob
				delay = 1.0 / *delayHzIter;
				if(delayRange > 2) { //3 is kHz instead of Hz
					delay /= 1000.0;
				}
			} else if(delaySource == 1) { //Time Knob
				delay = *delayTimeIter;
				if(delayRange < 3) { //3 is s instead of ms
					delay /= 1000.0;
				}
			} else if(delaySource == 2) { //Hz input
				delay = 1.0 / **freqInput;
			}
		}
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			if(!delaySourceConstant || m_shouldUpdateParent) {
				if(delaySource == 0) { //Hz Knob
					delay = 1.0 / *delayHzIter;
					if(delayRange > 2) { //3 is kHz instead of Hz
						delay /= 1000.0;
					}
				} else if(delaySource == 1) { //Time Knob
					delay = *delayTimeIter;
					if(delayRange < 3) { //3 is s instead of ms
						delay /= 1000.0;
					}
				} else if(delaySource == 2) { //Hz input
					if(detuneConstant) {
						delay = 1.0 / (**freqInput * detune);
					} else {
						delay = 1.0 / (**freqInput * OctavesKnob::detune(SemitonesKnob::detune(1.0, *semitonesIter), *octavesIter));
					}
				}
			}

			value = m_delayLine.get(c, offset + s - delay * m_sampleRate_f);
			if(signalInput == nullptr) {
				m_delayLine.set(c, offset + s, value * *feedbackIter);
			} else {
				m_delayLine.set(c, offset + s, **signalInput + value * *feedbackIter);
			}
			if(signalInput == nullptr) {
				*signalOutput = value;
			} else {
				*signalOutput = value + **signalInput * *dryMixIter;
			}
			automation.incrementPosition();
			io.incrementPosition();
		}
		automation.incrementChannel();
		io.incrementChannel();
	}
	if(m_shouldUpdateParent) {
		m_parent.m_plot.drawDataFromAudioBuffer(m_delayLine, m_delayLine.getSize() - delay * m_sampleRate_f);
	}
	m_delayLine.offsetData(-AudioBuffer::getDefaultSamples());
	/* END USER-DEFINED EXECUTION CODE */
}

void CombAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	m_delayLine.fill(0.0);
	m_parent.m_plot.clear();
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

