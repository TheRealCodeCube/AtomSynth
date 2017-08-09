#ifndef ATOM_LfoTuner_HEADER
#define ATOM_LfoTuner_HEADER

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
* YOUR DOCUMENTATION HERE
*/
class LfoTunerController: public AtomController, public AutomatedControl::Listener, public BpmMultiple::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	BpmMultiple m_bpc;
	AtomKnob m_mult;
	DVecIter m_multIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	LfoTunerController();
	virtual AtomController * createNewInstance() {
		return new LfoTunerController();
	}
	virtual ~LfoTunerController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Transforms";
	}
	virtual std::string getName() {
		return "LFO Tuner";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void bpmMultipleChanged(BpmMultiple * multiple);
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class LfoTunerAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class LfoTunerAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	LfoTunerController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	LfoTunerAtom(LfoTunerController & parent, int index);
	virtual ~LfoTunerAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class LfoTunerController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_LfoTuner_HEADER */

