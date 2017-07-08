/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[10.000000:1.000000:1.000000:3.000000:1.000000:sBasic Osc:sGeneration:[[24.000000:0.000000:1.000000:10.000000:10.000000:sWaveformPlot:splot:s:]][[[24.000000:0.000000:0.000000:12.000000:1.000000:sMultiButton:swaveSelect:s:]][0.000000:0.000000:sSin:sSqr:sTri:sExp:s2xExp:sTanh:s2xTanh:]][[[4.000000:10.000000:1.500000:2.000000:2.000000:sAtomKnob:stop:sTop:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:0.000000:14.000000:2.000000:2.000000:sSimpleKnob:suVoices:sVoices:]][-10.000000:10.000000:0.000000:1.000000:1.000000:s:]][[[4.000000:10.000000:4.000000:2.000000:2.000000:sAtomKnob:sbottom:sBottom:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:4.000000:11.500000:2.000000:2.000000:sAtomKnob:span:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:11.500000:2.000000:2.000000:sAtomKnob:sphase:sPhase:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:0.000000:11.500000:2.000000:2.000000:sAtomKnob:samplitude:sAmplitude:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:9.000000:2.000000:2.000000:sAtomKnob:svar:sVariable:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:6.500000:2.000000:2.000000:sAtomKnob:scenter:sCenter:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:4.000000:14.000000:2.000000:2.000000:sAtomKnob:suPan:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:14.000000:2.000000:2.000000:sAtomKnob:suPhase:s�Phase:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:10.000000:11.500000:2.000000:2.000000:sOctavesKnob:soctaves:sOctaves:]][-4.000000:4.000000:0.000000:1.000000:0.000000:so:]][[[4.000000:6.000000:11.500000:2.000000:2.000000:sCentsKnob:scents:sDetune:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:6.000000:14.000000:2.000000:2.000000:sCentsKnob:suCents:sDetune�:]][-100.000000:100.000000:0.000000:1.000000:0.000000:sc:]][[[4.000000:10.000000:14.000000:2.000000:2.000000:sSemitonesKnob:ssemis:sSemitones:]][-12.000000:12.000000:0.000000:1.000000:0.000000:ss:]]]]
#include "BasicOsc.h"
#include "Technical/SaveState.h"

namespace AtomSynth
{

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

BasicOscController::BasicOscController()
	: AtomController(AtomParameters(10, 1, true, 1))
{
	init();

	m_gui.addComponent(& m_plot);
	m_plot.setBounds(CB(0.000000, 1.000000, 10.000000, 10.000000));

	m_gui.addComponent(& m_waveSelect);
	m_waveSelect.setBounds(CB(0.000000, 0.000000, 12.000000, 1.000000));
	m_waveSelect.addListener(this);

	m_gui.addComponent(& m_top);
	m_top.setBounds(CB(10.000000, 1.500000, 2.000000, 2.000000));
	m_top.addListener(this);
	m_top.setBounded(true);
	addAutomatedControl(m_top, m_topIter);
	m_gui.addComponent(m_top.createLabel("Top", true));

	m_gui.addComponent(& m_uVoices);
	m_uVoices.setBounds(CB(0.000000, 14.000000, 2.000000, 2.000000));
	m_uVoices.addListener(this);
	m_uVoices.setRange(-10.000000, 10.000000);
	m_uVoices.setInt(true);
	m_uVoices.setBounded(true);
	m_gui.addComponent(m_uVoices.createLabel("Voices", true));

	m_gui.addComponent(& m_bottom);
	m_bottom.setBounds(CB(10.000000, 4.000000, 2.000000, 2.000000));
	m_bottom.addListener(this);
	m_bottom.setBounded(true);
	addAutomatedControl(m_bottom, m_bottomIter);
	m_gui.addComponent(m_bottom.createLabel("Bottom", true));

	m_gui.addComponent(& m_pan);
	m_pan.setBounds(CB(4.000000, 11.500000, 2.000000, 2.000000));
	m_pan.addListener(this);
	m_pan.setBounded(true);
	addAutomatedControl(m_pan, m_panIter);
	m_gui.addComponent(m_pan.createLabel("Panning", true));

	m_gui.addComponent(& m_phase);
	m_phase.setBounds(CB(2.000000, 11.500000, 2.000000, 2.000000));
	m_phase.addListener(this);
	m_phase.setBounded(true);
	addAutomatedControl(m_phase, m_phaseIter);
	m_gui.addComponent(m_phase.createLabel("Phase", true));

	m_gui.addComponent(& m_amplitude);
	m_amplitude.setBounds(CB(0.000000, 11.500000, 2.000000, 2.000000));
	m_amplitude.addListener(this);
	m_amplitude.setBounded(true);
	addAutomatedControl(m_amplitude, m_amplitudeIter);
	m_gui.addComponent(m_amplitude.createLabel("Amplitude", true));

	m_gui.addComponent(& m_var);
	m_var.setBounds(CB(10.000000, 9.000000, 2.000000, 2.000000));
	m_var.addListener(this);
	m_var.setBounded(true);
	addAutomatedControl(m_var, m_varIter);
	m_gui.addComponent(m_var.createLabel("Variable", true));

	m_gui.addComponent(& m_center);
	m_center.setBounds(CB(10.000000, 6.500000, 2.000000, 2.000000));
	m_center.addListener(this);
	m_center.setBounded(true);
	addAutomatedControl(m_center, m_centerIter);
	m_gui.addComponent(m_center.createLabel("Center", true));

	m_gui.addComponent(& m_uPan);
	m_uPan.setBounds(CB(4.000000, 14.000000, 2.000000, 2.000000));
	m_uPan.addListener(this);
	m_uPan.setBounded(true);
	addAutomatedControl(m_uPan, m_uPanIter);
	m_gui.addComponent(m_uPan.createLabel("Panning", true));

	m_gui.addComponent(& m_uPhase);
	m_uPhase.setBounds(CB(2.000000, 14.000000, 2.000000, 2.000000));
	m_uPhase.addListener(this);
	m_uPhase.setBounded(true);
	addAutomatedControl(m_uPhase, m_uPhaseIter);
	m_gui.addComponent(m_uPhase.createLabel("[Phase", true));

	m_gui.addComponent(& m_octaves);
	m_octaves.setBounds(CB(10.000000, 11.500000, 2.000000, 2.000000));
	m_octaves.addListener(this);
	addAutomatedControl(m_octaves, m_octavesIter);
	m_gui.addComponent(m_octaves.createLabel("Octaves", true));

	m_gui.addComponent(& m_cents);
	m_cents.setBounds(CB(6.000000, 11.500000, 2.000000, 2.000000));
	m_cents.addListener(this);
	addAutomatedControl(m_cents, m_centsIter);
	m_gui.addComponent(m_cents.createLabel("Detune", true));

	m_gui.addComponent(& m_uCents);
	m_uCents.setBounds(CB(6.000000, 14.000000, 2.000000, 2.000000));
	m_uCents.addListener(this);
	addAutomatedControl(m_uCents, m_uCentsIter);
	m_gui.addComponent(m_uCents.createLabel("Detune]", true));

	m_gui.addComponent(& m_semis);
	m_semis.setBounds(CB(10.000000, 14.000000, 2.000000, 2.000000));
	m_semis.addListener(this);
	addAutomatedControl(m_semis, m_semisIter);
	m_gui.addComponent(m_semis.createLabel("Semitones", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * BasicOscController::createAtom(int index)
{
	return new BasicOscAtom(* this, index);
}

SaveState BasicOscController::saveSaveState()
{
	SaveState & toReturn = * new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = * new SaveState();
	extraData.addValue(3); //Store the revision this was saved with, to preserve backwards compatibility.
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

void BasicOscController::loadSaveState(SaveState state)
{
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 3)
	{
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

void BasicOscController::multiButtonPressed(MultiButton * button)
{
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void BasicOscController::automatedControlChanged(AutomatedControl * control, bool byUser)
{
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void BasicOscController::simpleKnobChanged(SimpleKnob * knob)
{
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

BasicOscAtom::BasicOscAtom(BasicOscController & parent, int index)
	: Atom(parent, index),
	  m_parent(parent)
{
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void BasicOscAtom::execute()
{
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	std::vector<double>::iterator & topIter = m_parent.m_topIter;
	std::vector<double>::iterator & bottomIter = m_parent.m_bottomIter;
	std::vector<double>::iterator & panIter = m_parent.m_panIter;
	std::vector<double>::iterator & phaseIter = m_parent.m_phaseIter;
	std::vector<double>::iterator & amplitudeIter = m_parent.m_amplitudeIter;
	std::vector<double>::iterator & varIter = m_parent.m_varIter;
	std::vector<double>::iterator & centerIter = m_parent.m_centerIter;
	std::vector<double>::iterator & uPanIter = m_parent.m_uPanIter;
	std::vector<double>::iterator & uPhaseIter = m_parent.m_uPhaseIter;
	std::vector<double>::iterator & octavesIter = m_parent.m_octavesIter;
	std::vector<double>::iterator & centsIter = m_parent.m_centsIter;
	std::vector<double>::iterator & uCentsIter = m_parent.m_uCentsIter;
	std::vector<double>::iterator & semisIter = m_parent.m_semisIter;

	/* BEGIN USER-DEFINED EXECUTION CODE */
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++)
	{
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++)
		{
			//PUT YOUR EXECUTION CODE HERE
			
			automation.incrementPosition();
		}
		automation.incrementChannel();
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void BasicOscAtom::reset()
{
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

