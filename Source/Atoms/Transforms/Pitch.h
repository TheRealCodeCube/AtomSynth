#ifndef ATOM_Pitch_HEADER
#define ATOM_Pitch_HEADER

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
* Detunes and blends pitches from two sources.
*/
class PitchController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	MultiButton m_rightSource;
	OctavesKnob m_rightOctaves;
	DVecIter m_rightOctavesIter;
	SemitonesKnob m_rightSemis;
	DVecIter m_rightSemisIter;
	CentsKnob m_rightCents;
	DVecIter m_rightCentsIter;
	CentsKnob m_leftCents;
	DVecIter m_leftCentsIter;
	SemitonesKnob m_leftSemis;
	DVecIter m_leftSemisIter;
	OctavesKnob m_leftOctaves;
	DVecIter m_leftOctavesIter;
	MultiButton m_leftSource;
	Arrow m_arrow0;
	Arrow m_arrow1;
	AtomKnob m_blend;
	DVecIter m_blendIter;
	Arrow m_arrow2;
	Arrow m_arrow3;
	Arrow m_arrow4;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	PitchController();
	virtual AtomController * createNewInstance() {
		return new PitchController();
	}
	virtual ~PitchController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Transforms";
	}
	virtual std::string getName() {
		return "Pitch Mixer";
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

	friend class PitchAtom;
};

/**
* See PitchController.
*/
class PitchAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	PitchController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	PitchAtom(PitchController & parent, int index);
	virtual ~PitchAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class PitchController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Pitch_HEADER */

