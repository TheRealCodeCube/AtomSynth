#ifndef ATOM_BasicOsc_HEADER
#define ATOM_BasicOsc_HEADER

/* BEGIN AUTO-GENERATED HEADER */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED HEADER */

namespace AtomSynth
{

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

class BasicOscController : public AtomController, public AutomatedControl::Listener, public MultiButton::Listener, public SimpleKnob::Listener
{
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	WaveformPlot m_plot;
	MultiButton m_waveSelect;
	AtomKnob m_top;
	std::vector<double>::iterator m_topIter;
	SimpleKnob m_uVoices;
	AtomKnob m_bottom;
	std::vector<double>::iterator m_bottomIter;
	AtomKnob m_pan;
	std::vector<double>::iterator m_panIter;
	AtomKnob m_phase;
	std::vector<double>::iterator m_phaseIter;
	AtomKnob m_amplitude;
	std::vector<double>::iterator m_amplitudeIter;
	AtomKnob m_var;
	std::vector<double>::iterator m_varIter;
	AtomKnob m_center;
	std::vector<double>::iterator m_centerIter;
	AtomKnob m_uPan;
	std::vector<double>::iterator m_uPanIter;
	AtomKnob m_uPhase;
	std::vector<double>::iterator m_uPhaseIter;
	OctavesKnob m_octaves;
	std::vector<double>::iterator m_octavesIter;
	CentsKnob m_cents;
	std::vector<double>::iterator m_centsIter;
	CentsKnob m_uCents;
	std::vector<double>::iterator m_uCentsIter;
	SemitonesKnob m_semis;
	std::vector<double>::iterator m_semisIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	BasicOscController();
	virtual AtomController * createNewInstance() { return new BasicOscController(); }
	virtual ~BasicOscController() { }

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() { return "Generation"; }
	virtual std::string getName() { return "Basic Osc"; }
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	virtual void multiButtonPressed(MultiButton * button);
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	virtual void simpleKnobChanged(SimpleKnob * knob);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class BasicOscAtom;
};

class BasicOscAtom : public Atom
{
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	BasicOscController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	BasicOscAtom(BasicOscController & parent, int index);
	virtual ~BasicOscAtom() { }
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class BasicOscController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_BasicOsc_HEADER */

