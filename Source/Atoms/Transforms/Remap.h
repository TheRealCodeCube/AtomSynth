#ifndef ATOM_Remap_HEADER
#define ATOM_Remap_HEADER

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
class RemapController: public AtomController {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	RemapController();
	virtual AtomController * createNewInstance() {
		return new RemapController();
	}
	virtual ~RemapController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Transforms";
	}
	virtual std::string getName() {
		return "Remap";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class RemapAtom;
};

/**
* YOUR DOCUMENTATION HERE
*/
class RemapAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	RemapController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	RemapAtom(RemapController & parent, int index);
	virtual ~RemapAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class RemapController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Remap_HEADER */

