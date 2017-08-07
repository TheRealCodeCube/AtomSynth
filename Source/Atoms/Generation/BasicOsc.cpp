/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[10.000000:1.000000:1.000000:sBasic Osc:sGeneration:shz:saudio:shz:swave:[[24.000000:0.000000:1.000000:10.000000:10.000000:1.000000:sWaveformPlot:splot:s:]][[[24.000000:0.000000:0.000000:12.000000:1.000000:1.000000:sMultiButton:swaveSelect:s:]][0.000000:0.000000:sSin:sSqr:sTri:sExp:sSExp:sCExp:sTahn:sSTanh:]][[[4.000000:10.000000:1.500000:2.000000:2.000000:1.000000:sAtomKnob:stop:sTop:]][-1.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:0.000000:14.000000:2.000000:2.000000:1.000000:sSimpleKnob:suVoices:sVoices:]][1.000000:12.000000:1.000000:1.000000:1.000000:s:]][[[4.000000:10.000000:4.000000:2.000000:2.000000:1.000000:sAtomKnob:sbottom:sBottom:]][-1.000000:1.000000:-1.000000:0.000000:1.000000:s:]][[[4.000000:4.000000:11.500000:2.000000:2.000000:1.000000:sAtomKnob:span:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:11.500000:2.000000:2.000000:1.000000:sAtomKnob:sphase:sPhase:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:0.000000:11.500000:2.000000:2.000000:1.000000:sAtomKnob:samplitude:sAmplitude:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:9.000000:2.000000:2.000000:1.000000:sAtomKnob:svar:sVariable:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:6.500000:2.000000:2.000000:1.000000:sAtomKnob:scenter:sCenter:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:4.000000:14.000000:2.000000:2.000000:1.000000:sAtomKnob:suPan:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:14.000000:2.000000:2.000000:1.000000:sAtomKnob:suPhase:sPhase:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:11.500000:2.000000:2.000000:1.000000:sOctavesKnob:soctaves:sOctaves:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]][[[4.000000:6.000000:11.500000:2.000000:2.000000:1.000000:sCentsKnob:scents:sDetune:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:6.000000:14.000000:2.000000:2.000000:1.000000:sCentsKnob:suCents:sDetune�:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:10.000000:14.000000:2.000000:2.000000:1.000000:sSemitonesKnob:ssemis:sSemitones:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "BasicOsc.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include <cmath>

#include "../../Adsp/Basic.h"
#include "../../Adsp/Remap.h"
#include "../../Adsp/Waveform.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */
//Phase calculation code: y=\left\{-1<x<c:\frac{x+1}{c+1}-1,c<x<1:\frac{x-c}{1-c}\right\}
//(Where x is the input phase, c is the center, y is output phase.
void BasicOscController::updatePlot() {
	m_plot.clear();

	double top = m_top.getDisplayValue();
	double bottom = m_bottom.getDisplayValue();
	double center = m_center.getDisplayValue();
	double amplitude = m_amplitude.getDisplayValue();
	double a = Adsp::fastRemapComputeA(-1.0, 1.0, bottom, top);
	double b = Adsp::fastRemapComputeB(-1.0, 1.0, bottom, top);
	double phaseOffset = m_phase.getDisplayValue();
	double var = m_var.getDisplayValue();
	int type = m_waveSelect.getSelectedLabel();
	double phase, value;
	for (int i = 0; i <= m_plot.getWidth(); i++) {
		phase = double(i) / double(m_plot.getWidth()) + phaseOffset;
		phase = fmod(phase, 1.0);
		phase = phase * 2.0 - 1.0;

		if (phase <= center) {
			phase = (phase + 1.0) / (center + 1.0 + 1.0e-64) - 1.0; //1.0e-64 is there to prevent divide by zero errors when center == -1.0
		} else {
			phase = (phase - center) / (1.0 - center);
		}

		switch (type) {
		case 0:
			value = Adsp::sineWave(phase);
			break;
		case 1:
			value = Adsp::squareWave(phase);
			break;
		case 2:
			value = Adsp::triWave(phase);
			break;
		case 3:
			value = Adsp::expWave(phase, var);
			break;
		case 4:
			value = Adsp::expSymmWave(phase, var);
			break;
		case 5:
			value = Adsp::expCrestWave(phase, var);
			break;
		case 6:
			value = Adsp::tanhWave(phase, var);
			break;
		case 7:
			value = Adsp::tanhSymmWave(phase, var);
			break;
		default:
			value = 0.2;
		}

		value *= amplitude;
		value = Adsp::fastRemap(value, a, b);

		m_plot.addPoint(value);
	}

	//m_plot.repaint();
}

double BasicOscAtom::getUnisonFactor(int voice, int totalVoices) {
	if (totalVoices == 1)
		return 0.0;
	else
		return (double(voice) / double(totalVoices - 1)) * 2.0 - 1.0;
}
/* END MISC. USER-DEFINED CODE */

BasicOscController::BasicOscController() :
		AtomController(AtomParameters(10, 1, true, 1)) {
	init();

	addInputIcon("hz");
	addOutputIcon("wave");

	m_gui.addComponent(&m_plot);
	m_plot.setBounds(CB(0.000000, 1.000000, 10.000000, 10.000000));

	m_gui.addComponent(&m_waveSelect);
	m_waveSelect.setBounds(CB(0.000000, 0.000000, 12.000000, 1.000000));
	m_waveSelect.addListener(this);
	m_waveSelect.addLabel("Sin");
	m_waveSelect.addLabel("Sqr");
	m_waveSelect.addLabel("Tri");
	m_waveSelect.addLabel("Exp");
	m_waveSelect.addLabel("SExp");
	m_waveSelect.addLabel("CExp");
	m_waveSelect.addLabel("Tahn");
	m_waveSelect.addLabel("STanh");

	m_gui.addComponent(&m_top);
	m_top.setBounds(CB(10.000000, 1.500000, 2.000000, 2.000000));
	m_top.addListener(this);
	m_top.setValue(1.000000);
	m_top.setBounded(true);
	addAutomatedControl(m_top, m_topIter);
	m_gui.addComponent(m_top.createLabel("Top", true));

	m_gui.addComponent(&m_uVoices);
	m_uVoices.setBounds(CB(0.000000, 14.000000, 2.000000, 2.000000));
	m_uVoices.addListener(this);
	m_uVoices.setRange(1.000000, 12.000000);
	m_uVoices.setInt(true);
	m_uVoices.setBounded(true);
	m_uVoices.setValue(1.000000);
	m_gui.addComponent(m_uVoices.createLabel("Voices", true));

	m_gui.addComponent(&m_bottom);
	m_bottom.setBounds(CB(10.000000, 4.000000, 2.000000, 2.000000));
	m_bottom.addListener(this);
	m_bottom.setValue(-1.000000);
	m_bottom.setBounded(true);
	addAutomatedControl(m_bottom, m_bottomIter);
	m_gui.addComponent(m_bottom.createLabel("Bottom", true));

	m_gui.addComponent(&m_pan);
	m_pan.setBounds(CB(4.000000, 11.500000, 2.000000, 2.000000));
	m_pan.addListener(this);
	m_pan.setBounded(true);
	addAutomatedControl(m_pan, m_panIter);
	m_gui.addComponent(m_pan.createLabel("Panning", true));

	m_gui.addComponent(&m_phase);
	m_phase.setBounds(CB(2.000000, 11.500000, 2.000000, 2.000000));
	m_phase.addListener(this);
	m_phase.setBounded(true);
	addAutomatedControl(m_phase, m_phaseIter);
	m_gui.addComponent(m_phase.createLabel("Phase", true));

	m_gui.addComponent(&m_amplitude);
	m_amplitude.setBounds(CB(0.000000, 11.500000, 2.000000, 2.000000));
	m_amplitude.addListener(this);
	m_amplitude.setRange(0.000000, 1.000000);
	m_amplitude.setValue(1.000000);
	m_amplitude.setBounded(true);
	addAutomatedControl(m_amplitude, m_amplitudeIter);
	m_gui.addComponent(m_amplitude.createLabel("Amplitude", true));

	m_gui.addComponent(&m_var);
	m_var.setBounds(CB(10.000000, 9.000000, 2.000000, 2.000000));
	m_var.addListener(this);
	m_var.setBounded(true);
	addAutomatedControl(m_var, m_varIter);
	m_gui.addComponent(m_var.createLabel("Variable", true));

	m_gui.addComponent(&m_center);
	m_center.setBounds(CB(10.000000, 6.500000, 2.000000, 2.000000));
	m_center.addListener(this);
	m_center.setBounded(true);
	addAutomatedControl(m_center, m_centerIter);
	m_gui.addComponent(m_center.createLabel("Center", true));

	m_gui.addComponent(&m_uPan);
	m_uPan.setBounds(CB(4.000000, 14.000000, 2.000000, 2.000000));
	m_uPan.addListener(this);
	m_uPan.setBounded(true);
	addAutomatedControl(m_uPan, m_uPanIter);
	m_gui.addComponent(m_uPan.createLabel("Panning", true));

	m_gui.addComponent(&m_uPhase);
	m_uPhase.setBounds(CB(2.000000, 14.000000, 2.000000, 2.000000));
	m_uPhase.addListener(this);
	m_uPhase.setBounded(true);
	addAutomatedControl(m_uPhase, m_uPhaseIter);
	m_gui.addComponent(m_uPhase.createLabel("Phase", true));

	m_gui.addComponent(&m_octaves);
	m_octaves.setBounds(CB(10.000000, 11.500000, 2.000000, 2.000000));
	m_octaves.addListener(this);
	addAutomatedControl(m_octaves, m_octavesIter);
	m_gui.addComponent(m_octaves.createLabel("Octaves", true));

	m_gui.addComponent(&m_cents);
	m_cents.setBounds(CB(6.000000, 11.500000, 2.000000, 2.000000));
	m_cents.addListener(this);
	addAutomatedControl(m_cents, m_centsIter);
	m_gui.addComponent(m_cents.createLabel("Detune", true));

	m_gui.addComponent(&m_uCents);
	m_uCents.setBounds(CB(6.000000, 14.000000, 2.000000, 2.000000));
	m_uCents.addListener(this);
	addAutomatedControl(m_uCents, m_uCentsIter);
	m_gui.addComponent(m_uCents.createLabel("Detune�", true));

	m_gui.addComponent(&m_semis);
	m_semis.setBounds(CB(10.000000, 14.000000, 2.000000, 2.000000));
	m_semis.addListener(this);
	addAutomatedControl(m_semis, m_semisIter);
	m_gui.addComponent(m_semis.createLabel("Semitones", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * BasicOscController::createAtom(int index) {
	return new BasicOscAtom(*this, index);
}

SaveState BasicOscController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_waveSelect.saveSaveState());
	extraData.addState(m_top.saveSaveState());
	extraData.addState(m_uVoices.saveSaveState());
	extraData.addState(m_bottom.saveSaveState());
	extraData.addState(m_pan.saveSaveState());
	extraData.addState(m_phase.saveSaveState());
	extraData.addState(m_amplitude.saveSaveState());
	extraData.addState(m_var.saveSaveState());
	extraData.addState(m_center.saveSaveState());
	extraData.addState(m_uPan.saveSaveState());
	extraData.addState(m_uPhase.saveSaveState());
	extraData.addState(m_octaves.saveSaveState());
	extraData.addState(m_cents.saveSaveState());
	extraData.addState(m_uCents.saveSaveState());
	extraData.addState(m_semis.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void BasicOscController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if (version == 3) {
		m_waveSelect.loadSaveState(extraData.getNextState());
		m_top.loadSaveState(extraData.getNextState());
		m_uVoices.loadSaveState(extraData.getNextState());
		m_bottom.loadSaveState(extraData.getNextState());
		m_pan.loadSaveState(extraData.getNextState());
		m_phase.loadSaveState(extraData.getNextState());
		m_amplitude.loadSaveState(extraData.getNextState());
		m_var.loadSaveState(extraData.getNextState());
		m_center.loadSaveState(extraData.getNextState());
		m_uPan.loadSaveState(extraData.getNextState());
		m_uPhase.loadSaveState(extraData.getNextState());
		m_octaves.loadSaveState(extraData.getNextState());
		m_cents.loadSaveState(extraData.getNextState());
		m_uCents.loadSaveState(extraData.getNextState());
		m_semis.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void BasicOscController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	updatePlot();
	/* END USER-DEFINED LISTENER CODE */
}

void BasicOscController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	updatePlot();
	/* END USER-DEFINED LISTENER CODE */
}

void BasicOscController::simpleKnobChanged(SimpleKnob * knob) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

BasicOscAtom::BasicOscAtom(BasicOscController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_phases.resize(20, std::vector<double>(AudioBuffer::getDefaultChannels(), 0.0));
	/* END USER-DEFINED CONSTRUCTION CODE */
}

void BasicOscAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & topIter = m_parent.m_topIter;
	DVecIter & bottomIter = m_parent.m_bottomIter;
	DVecIter & panIter = m_parent.m_panIter;
	DVecIter & phaseIter = m_parent.m_phaseIter;
	DVecIter & amplitudeIter = m_parent.m_amplitudeIter;
	DVecIter & varIter = m_parent.m_varIter;
	DVecIter & centerIter = m_parent.m_centerIter;
	DVecIter & uPanIter = m_parent.m_uPanIter;
	DVecIter & uPhaseIter = m_parent.m_uPhaseIter;
	DVecIter & octavesIter = m_parent.m_octavesIter;
	DVecIter & centsIter = m_parent.m_centsIter;
	DVecIter & uCentsIter = m_parent.m_uCentsIter;
	DVecIter & semisIter = m_parent.m_semisIter;

	IOSet io = IOSet();
	DVecIter * hzInput = io.addInput(m_primaryInputs[0]);
	DVecIter & audioOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	int voices = m_parent.m_uVoices.getValue();

	double baseDetune;
	bool animateCoarseDetune = !m_parent.m_octaves.getResult().isConstant();
	bool animateCents = !m_parent.m_cents.getResult().isConstant();
	double baseFrequency;
	bool animateBaseFreq = false;
	std::vector<double> panAmplitudes;
	double pan;
	bool animatePan = !m_parent.m_pan.getResult().isConstant();
	m_outputs[0].fill(0.0);
	double basePan, freq, panAmp, phase, uFac, value;
	for (int voice = 0; voice < voices; voice++) { //Iterate over each voice.
		automation.resetPosition();
		io.resetPosition();
		for (int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
			baseDetune = 1.0;
			if (!animateCoarseDetune) {
				baseDetune *= OctavesKnob::detune(1.0, (*octavesIter)); //Detuning 1 hz up 2 octaves = 2 hz. This is the same as getting the detune factor for the number of octaves. Used later.
				baseDetune *= SemitonesKnob::detune(1.0, (*semisIter));
			}
			if (!animateCents) {
				baseDetune *= CentsKnob::detune(1.0, (*centsIter));
			}
			if (hzInput != nullptr) {
				baseFrequency = (**hzInput) * baseDetune;
				if(m_primaryInputs[0]->isConstant()) {
					baseFrequency = (**hzInput) * baseDetune;
				} else {
					animateBaseFreq = true;
				}
			} else {
				baseFrequency = 440.0 * baseDetune;
			}
			if (!animatePan) {
				if (voices > 1) {
					for (int voice = 0; voice < voices; voice++) {
						pan = (*panIter) + (getUnisonFactor(voice, voices) * (*uPanIter));
						pan = Adsp::clip(pan);
						panAmplitudes.push_back(Adsp::panLeftAmplitude(pan));
						panAmplitudes.push_back(Adsp::panRightAmplitude(pan));
					}
				} else {
					pan = Adsp::clip(*panIter);
					panAmplitudes.push_back(Adsp::panLeftAmplitude(pan));
					panAmplitudes.push_back(Adsp::panRightAmplitude(pan));
				}
			}
			std::vector<double> voiceDetunes;
			if (voices > 1) {
				for (int voice = 0; voice < voices; voice++) {
					voiceDetunes.push_back(CentsKnob::detune(1.0, (*uCentsIter) * getUnisonFactor(voice, m_phases.size())));
				}
			}
			for (int s = 0; s < AudioBuffer::getDefaultSize(); s++) {
				uFac = getUnisonFactor(voice, voices);
				if (animatePan) {
					if (voices > 1) {
						pan = *panIter + (uFac * (*uPanIter));
					} else {
						pan = *panIter;
					}
					//Sinusoidal panning
					if (c == 0) //left
						panAmp = Adsp::panLeftAmplitude(pan);
					else
						//right
						panAmp = Adsp::panRightAmplitude(pan);
				} else {
					panAmp = panAmplitudes[voice * AudioBuffer::getDefaultChannels() + c];
				}

				if(animateBaseFreq) {
					freq = (**hzInput) * baseDetune;
				} else {
					freq = baseFrequency;
				}

				if (voices > 1) {
					freq *= voiceDetunes[voice];
				}

				if (animateCoarseDetune) {
					freq = OctavesKnob::detune(freq, (*octavesIter));
					freq = SemitonesKnob::detune(freq, (*semisIter));
				}
				if (animateCents) {
					freq = CentsKnob::detune(freq, (*centsIter));
				}

				m_phases[voice][c] += freq / m_sampleRate;
				phase = m_phases[voice][c] + (*phaseIter) * 2.0 + ((*uPhaseIter) * uFac) + 10.0; //+10.0 is so that I don't have to do a branch in case the phase went into fmod being negative.
				phase = std::fmod(phase, 2.0);
				phase -= 1.0; //Convert range from 0-2 to -1-1.

				if (phase <= *centerIter) {
					phase = (phase + 1.0) / (*centerIter + 1.0 + 1.0e-64) - 1.0; //1.0e-64 is there to prevent divide by zero errors when * centerIter == -1.0
				} else {
					phase = (phase - *centerIter) / (1.0 - *centerIter);
				}

				switch (m_parent.m_waveSelect.getSelectedLabel()) {
				case 0:
					value = Adsp::sineWave(phase);
					break;
				case 1:
					value = Adsp::squareWave(phase);
					break;
				case 2:
					value = Adsp::triWave(phase);
					break;
				case 3:
					value = Adsp::expWave(phase, *varIter);
					break;
				case 4:
					value = Adsp::expSymmWave(phase, *varIter);
					break;
				case 5:
					value = Adsp::expCrestWave(phase, *varIter);
					break;
				case 6:
					value = Adsp::tanhWave(phase, *varIter);
					break;
				case 7:
					value = Adsp::tanhSymmWave(phase, *varIter);
					break;
				default:
					value = 0.2;
				}
				value *= *amplitudeIter;
				value = Adsp::remap(value, -1.0, 1.0, *bottomIter, *topIter);
				value *= panAmp;
				value /= (voices + 1.0) * 0.5; //This way, many-voice synths don't sound super loud.
				if (voice % 2 == 1) {
					value = -value; //To prevent a buildup of constructive interference / dc offsets.
				}

				(*audioOutput) += value;
				//if(animateCoarseDetune) m_outputs[0].getData()[s + (AudioBuffer::getDefaultSize() * c)] = freq / 800.0; //Debug
				automation.incrementPosition();
				io.incrementPosition();
			}
			automation.incrementChannel();
			io.incrementChannel();
		}
	}

	if (m_shouldUpdateParent) {
		m_parent.m_plot.setDataFromAudioBuffer(m_outputs[0]);
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void BasicOscAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

