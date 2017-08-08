#ifndef ATOM_Comb_HEADER
#define ATOM_Comb_HEADER

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
* A comb filter.
*/
class CombController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	Arrow m_arrow0;
	Arrow m_arrow1;
	Arrow m_arrow2;
	Arrow m_arrow3;
	Arrow m_arrow4;
	MultiButton m_delaySource;
	AtomKnob m_delayTime;
	DVecIter m_delayTimeIter;
	MultiButton m_timeRange;
	MultiButton m_hertzRange;
	AtomKnob m_delayHz;
	DVecIter m_delayHzIter;
	AtomKnob m_dryMix;
	DVecIter m_dryMixIter;
	AtomKnob m_feedback;
	DVecIter m_feedbackIter;
	Arrow m_arrow5;
	Arrow m_arrow6;
	Arrow m_arrow7;
	OctavesKnob m_octaves;
	DVecIter m_octavesIter;
	SemitonesKnob m_semitones;
	DVecIter m_semitonesIter;
	WaveformPlot m_plot;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	CombController();
	virtual AtomController * createNewInstance() {
		return new CombController();
	}
	virtual ~CombController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Filters";
	}
	virtual std::string getName() {
		return "Comb Filter";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void multiButtonPressed(MultiButton * button);
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class CombAtom;
};

/**
* See CombController
*/
class CombAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	CombController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	AudioBuffer m_delayLine = AudioBuffer(45000);
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	CombAtom(CombController & parent, int index);
	virtual ~CombAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class CombController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Comb_HEADER */

