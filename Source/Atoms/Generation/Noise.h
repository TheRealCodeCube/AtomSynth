#ifndef ATOM_Noise_HEADER
#define ATOM_Noise_HEADER

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
 * Creates noise, with the option to use an interpolation
 * algorithm and to reduce the speed at which the noise
 * changes.
 */
class NoiseController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	WaveformPlot m_plot;
	AtomKnob m_amplitude;
	DVecIter m_amplitudeIter;
	AtomKnob m_time;
	DVecIter m_timeIter;
	MultiButton m_timeFrame;
	MultiButton m_type;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	NoiseController();
	virtual AtomController * createNewInstance() {
		return new NoiseController();
	}
	virtual ~NoiseController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Generation";
	}
	virtual std::string getName() {
		return "Noise";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/** Listener function. */
	virtual void multiButtonPressed(MultiButton * button);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */
	void updatePlot();
	/* END USER-DEFINED METHODS */

	friend class NoiseAtom;
};

/**
 * See NoiseController.
 */
class NoiseAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	NoiseController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	std::vector<std::vector<double>> m_previousValues;
	std::vector<double> m_lengthTimer;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	NoiseAtom(NoiseController & parent, int index);
	virtual ~NoiseAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */
	void updatePlot();
	/* END USER-DEFINED METHODS */

	friend class NoiseController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Noise_HEADER */

