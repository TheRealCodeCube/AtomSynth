#ifndef ATOM_BasicOsc_HEADER
#define ATOM_BasicOsc_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

/**
 * Capable of rendering several simple waveforms with various
 * modulations and effects, such as pm, am, and unison.
 */
class BasicOscController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener, public SimpleKnob::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	WaveformPlot m_plot;
	MultiButton m_waveSelect;
	AtomKnob m_top;
	DVecIter m_topIter;
	SimpleKnob m_uVoices;
	AtomKnob m_bottom;
	DVecIter m_bottomIter;
	AtomKnob m_pan;
	DVecIter m_panIter;
	AtomKnob m_phase;
	DVecIter m_phaseIter;
	AtomKnob m_amplitude;
	DVecIter m_amplitudeIter;
	AtomKnob m_var;
	DVecIter m_varIter;
	AtomKnob m_center;
	DVecIter m_centerIter;
	AtomKnob m_uPan;
	DVecIter m_uPanIter;
	AtomKnob m_uPhase;
	DVecIter m_uPhaseIter;
	OctavesKnob m_octaves;
	DVecIter m_octavesIter;
	CentsKnob m_cents;
	DVecIter m_centsIter;
	CentsKnob m_uCents;
	DVecIter m_uCentsIter;
	SemitonesKnob m_semis;
	DVecIter m_semisIter;
	AtomKnob m_fm;
	DVecIter m_fmIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	BasicOscController();
	virtual AtomController * createNewInstance() {
		return new BasicOscController();
	}
	virtual ~BasicOscController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Generation";
	}
	virtual std::string getName() {
		return "Basic Osc";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void multiButtonPressed(MultiButton * button);
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/** Listener function. */
	virtual void simpleKnobChanged(SimpleKnob * knob);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */
	void updatePlot();
	/* END USER-DEFINED METHODS */

	friend class BasicOscAtom;
};

/**
 * See BasicOscController.
 */
class BasicOscAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	BasicOscController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	std::vector<std::vector<double>> m_phases;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	BasicOscAtom(BasicOscController & parent, int index);
	virtual ~BasicOscAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */
	static double getUnisonFactor(int voice, int totalVoices);
	/* END USER-DEFINED METHODS */

	friend class BasicOscController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_BasicOsc_HEADER */

