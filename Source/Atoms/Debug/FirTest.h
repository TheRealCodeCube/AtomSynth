#ifndef ATOM_FirTest_HEADER
#define ATOM_FirTest_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Adsp/Fir.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

/**
* YOUR DOCUMENTATION HERE
*/
class FirTestController: public AtomController, public SimpleKnob::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	SimpleKnob m_co4;
	SimpleKnob m_co1;
	SimpleKnob m_co2;
	SimpleKnob m_co3;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	FirTestController();
	virtual AtomController * createNewInstance() {
		return new FirTestController();
	}
	virtual ~FirTestController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Debug";
	}
	virtual std::string getName() {
		return "Fir Tester";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void simpleKnobChanged(SimpleKnob * knob);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class FirTestAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class FirTestAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	FirTestController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	Adsp::FirFilter<256> m_filter;
	AudioBuffer m_buffer;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	FirTestAtom(FirTestController & parent, int index);
	virtual ~FirTestAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class FirTestController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_FirTest_HEADER */

