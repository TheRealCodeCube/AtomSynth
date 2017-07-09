/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[11.000000:0.000000:1.000000:3.000000:1.000000:sNoise:sGeneration:[[24.000000:0.000000:1.000000:10.000000:10.000000:sWaveformPlot:splot:s:]][[[4.000000:10.000000:1.500000:2.000000:2.000000:sAtomKnob:samplitude:sAmplitude:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:4.500000:2.000000:2.000000:sAtomKnob:stime:sTime:]][0.000000:1.000000:0.000000:0.000000:1.000000:sms:]][[[24.000000:10.000000:8.000000:2.000000:3.000000:sMultiButton:stimeFrame:sT. Range:]][1.000000:0.000000:s0-1ms:s0-10ms:s0-0.1s:s0-1s:]][[[24.000000:0.000000:0.000000:12.000000:1.000000:sMultiButton:stype:s:]][0.000000:0.000000:sSharp:sLinear:sSmooth:]]]]
/* BEGIN AUTO-GENERATED INCLUDES */
#include "Noise.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Adsp/interpolation.h"
#include "Technical/GlobalNoteStates.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */
void NoiseController::updatePlot() {
	m_plot.clear();
	srand(0); //So that the noise is consistent every time, and does not fluctuate wildly while tinkering with controls.

	double lengthTimer;
	std::vector<double> previousValues(3, 0.0);
	for(double & value : previousValues) {
		value = ((double(rand()) / double(RAND_MAX)) * 2.0 - 1.0); //Randomize starting values, so that interpolation doesn't always start from 0. (That would look weird)
	}
	//This function will calculate, the amount of time traversed per pixel so that the display will show, e.g., 10*5ms of noise when the 0-10ms time range is selected.
	double timePerPixel = m_time.getMax() / (m_time.getDisplayValue() + 1e-64) / double(m_plot.getWidth()) * 5.0; //1e-64 is to prevent /0 errors.
	double amplitude = m_amplitude.getDisplayValue();
	int type = m_type.getSelectedLabel();

	for(int i = 0; i <= m_plot.getWidth(); i++) {
		lengthTimer += timePerPixel;
		if (lengthTimer >= 1.0) {
			lengthTimer = 0.0;
			previousValues[2] = previousValues[1];
			previousValues[1] = previousValues[0];
			previousValues[0] = ((double(rand()) / double(RAND_MAX)) * 2.0 - 1.0);
		}

		if (type == 0) { //sharp
			m_plot.addPoint(previousValues[0] * amplitude);
		} else if (type == 1) { //linear
			m_plot.addPoint(Adsp::linearInterp(previousValues[1], previousValues[0], lengthTimer) * amplitude);
		} else if (type == 2) { //smooth
			m_plot.addPoint(Adsp::smoothInterp(previousValues[1], previousValues[0], lengthTimer) * amplitude);
		}
	}

	m_plot.repaintAsync();
}
/* END MISC. USER-DEFINED CODE */

NoiseController::NoiseController() :
		AtomController(AtomParameters(11, 0, true, 1)) {
	init();

	m_gui.addComponent(&m_plot);
	m_plot.setBounds(CB(0.000000, 1.000000, 10.000000, 10.000000));

	m_gui.addComponent(&m_amplitude);
	m_amplitude.setBounds(CB(10.000000, 1.500000, 2.000000, 2.000000));
	m_amplitude.addListener(this);
	m_amplitude.setRange(0.000000, 1.000000);
	m_amplitude.setValue(1.000000);
	m_amplitude.setBounded(true);
	addAutomatedControl(m_amplitude, m_amplitudeIter);
	m_gui.addComponent(m_amplitude.createLabel("Amplitude", true));

	m_gui.addComponent(&m_time);
	m_time.setBounds(CB(10.000000, 4.500000, 2.000000, 2.000000));
	m_time.addListener(this);
	m_time.setRange(0.000000, 1.000000);
	m_time.setBounded(true);
	m_time.setSuffix("ms");
	addAutomatedControl(m_time, m_timeIter);
	m_gui.addComponent(m_time.createLabel("Time", true));

	m_gui.addComponent(&m_timeFrame);
	m_timeFrame.setBounds(CB(10.000000, 8.000000, 2.000000, 3.000000));
	m_timeFrame.addListener(this);
	m_timeFrame.setVertical(true);
	m_timeFrame.addLabel("0-1ms");
	m_timeFrame.addLabel("0-10ms");
	m_timeFrame.addLabel("0-0.1s");
	m_timeFrame.addLabel("0-1s");
	m_gui.addComponent(m_timeFrame.createLabel("T. Range", true));

	m_gui.addComponent(&m_type);
	m_type.setBounds(CB(0.000000, 0.000000, 12.000000, 1.000000));
	m_type.addListener(this);
	m_type.addLabel("Sharp");
	m_type.addLabel("Linear");
	m_type.addLabel("Smooth");

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * NoiseController::createAtom(int index) {
	return new NoiseAtom(*this, index);
}

SaveState NoiseController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(3); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_amplitude.saveSaveState());
	extraData.addState(m_time.saveSaveState());
	extraData.addState(m_timeFrame.saveSaveState());
	extraData.addState(m_type.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void NoiseController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if (version == 3) {
		m_amplitude.loadSaveState(extraData.getNextState());
		m_time.loadSaveState(extraData.getNextState());
		m_timeFrame.loadSaveState(extraData.getNextState());
		m_type.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void NoiseController::automatedControlChanged(AutomatedControl * control,
		bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	updatePlot();
	/* END USER-DEFINED LISTENER CODE */
}

void NoiseController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	updatePlot();
	/* END USER-DEFINED LISTENER CODE */
}

NoiseAtom::NoiseAtom(NoiseController & parent, int index) :
		Atom(parent, index), m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_lengthTimer.resize(2, 2.0);
	m_previousValues.resize(AudioBuffer::getDefaultChannels(), std::vector<double>());
	m_previousValues[0].resize(3, 0.0);
	m_previousValues[1].resize(3, 0.0);
	/* END USER-DEFINED CONSTRUCTION CODE */
}

void NoiseAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	std::vector<double>::iterator & amplitudeIter = m_parent.m_amplitudeIter;
	std::vector<double>::iterator & timeIter = m_parent.m_timeIter;

	/* BEGIN USER-DEFINED EXECUTION CODE */
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	srand(t.tv_nsec);

	double time = 1000.0 / (m_sampleRate_f * *timeIter + 0.00001);
	if (m_parent.m_timeFrame.getSelectedLabel() > 1) //Labels 2 and 3 are measured in seconds rather than milliseconds.
			{
		time /= 1000.0;
	}

	int type = m_parent.m_type.getSelectedLabel();

	for (int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		for (int s = 0; s < AudioBuffer::getDefaultSize(); s++) {
			m_lengthTimer[c] += time;
			if (m_lengthTimer[c] >= 1.0) {
				m_lengthTimer[c] = 0.0;
				m_previousValues[c][2] = m_previousValues[c][1];
				m_previousValues[c][1] = m_previousValues[c][0];
				m_previousValues[c][0] = ((double(rand()) / double(RAND_MAX)) * 2.0 - 1.0);
			}

			if (type == 0) { //sharp
				m_outputs[0].setValue(c, s, m_previousValues[c][0] * *amplitudeIter);
			} else if (type == 1) { //linear
				m_outputs[0].setValue(c, s, Adsp::linearInterp(m_previousValues[c][1], m_previousValues[c][0], m_lengthTimer[c]) * *amplitudeIter);
			} else if (type == 2) { //smooth
				m_outputs[0].setValue(c, s, Adsp::smoothInterp(m_previousValues[c][1], m_previousValues[c][0], m_lengthTimer[c]) * *amplitudeIter);
			}
			automation.incrementPosition();
		}
		automation.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void NoiseAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	for (std::vector<double> & vec : m_previousValues) {
		for (double & value : vec) {
			value = 0.0;
		}
	}

	for (double & time : m_lengthTimer) {
		time = 2.0;
	}
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

