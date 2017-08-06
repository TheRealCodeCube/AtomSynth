#ifndef ATOM_Mixer_HEADER
#define ATOM_Mixer_HEADER

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
* Mixes together up to four inputs. Provides
* volume and panning controls for each input,
* as well as control over which method is used
* to combine the inputs to create the output.
*/
class MixerController: public AtomController, public AutomatedControl::Listener, public MultiButton::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	AtomSlider m_vol4;
	DVecIter m_vol4Iter;
	MultiButton m_mixMode3;
	AtomSlider m_vol1;
	DVecIter m_vol1Iter;
	MultiButton m_mixMode1;
	AtomSlider m_vol2;
	DVecIter m_vol2Iter;
	AtomSlider m_vol3;
	DVecIter m_vol3Iter;
	AtomKnob m_pan1;
	DVecIter m_pan1Iter;
	AtomKnob m_pan2;
	DVecIter m_pan2Iter;
	AtomKnob m_pan3;
	DVecIter m_pan3Iter;
	MultiButton m_mixMode2;
	AtomKnob m_pan4;
	DVecIter m_pan4Iter;
	Arrow m_arrow12;
	Arrow m_arrow13;
	Arrow m_arrow14;
	Arrow m_arrow15;
	Arrow m_arrow16;
	Arrow m_arrow17;
	Arrow m_arrow18;
	Arrow m_arrow19;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	MixerController();
	virtual AtomController * createNewInstance() {
		return new MixerController();
	}
	virtual ~MixerController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Processing";
	}
	virtual std::string getName() {
		return "4x Mixer";
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

	friend class MixerAtom;
};

/**
* See MixerController.
*/
class MixerAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	MixerController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	MixerAtom(MixerController & parent, int index);
	virtual ~MixerAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class MixerController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Mixer_HEADER */

