#ifndef ATOM_FirHighpass_HEADER
#define ATOM_FirHighpass_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include <array>

#include "Adsp/CachedFir.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

/**
* YOUR DOCUMENTATION HERE
*/
class FirHighpassController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	MultiButton m_cutoffSource;
	SemitonesKnob m_semis;
	DVecIter m_semisIter;
	OctavesKnob m_octs;
	DVecIter m_octsIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	FirHighpassController();
	virtual AtomController * createNewInstance() {
		return new FirHighpassController();
	}
	virtual ~FirHighpassController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Filters";
	}
	virtual std::string getName() {
		return "Highpass (FIR)";
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

	friend class FirHighpassAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class FirHighpassAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	FirHighpassController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	static constexpr int FILTER_TYPE = Adsp::FilterType::BLACKMAN | Adsp::FilterType::HIGHPASS;
	Adsp::CachedFirFilter m_filter;
	AudioBuffer m_delayLine;

	void recalculate(double newFreq);
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	FirHighpassAtom(FirHighpassController & parent, int index);
	virtual ~FirHighpassAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class FirHighpassController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_FirHighpass_HEADER */

