#ifndef ATOM_Amplifier_HEADER
#define ATOM_Amplifier_HEADER

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
class AmplifierController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	WaveformPlot m_plot;
	AtomSlider m_amp2;
	DVecIter m_amp2Iter;
	AtomSlider m_amp1;
	DVecIter m_amp1Iter;
	MultiButton m_mixMode;
	MultiButton m_clip;
	AtomKnob m_clipSkew;
	DVecIter m_clipSkewIter;
	AtomKnob m_clipLevel;
	DVecIter m_clipLevelIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	AmplifierController();
	virtual AtomController * createNewInstance() {
		return new AmplifierController();
	}
	virtual ~AmplifierController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Processing";
	}
	virtual std::string getName() {
		return "Amplifier";
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

	/* END USER-DEFINED METHODS */

	friend class AmplifierAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class AmplifierAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	AmplifierController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	AmplifierAtom(AmplifierController & parent, int index);
	virtual ~AmplifierAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class AmplifierController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Amplifier_HEADER */

