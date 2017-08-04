#ifndef ATOM_Output_HEADER
#define ATOM_Output_HEADER

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
 * The final output of the synth. Any audio
 * routed into here will be routed out to the
 * host.
 */
class OutputController: public AtomController {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	OutputController();
	virtual AtomController * createNewInstance() {
		return new OutputController();
	}
	virtual ~OutputController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Basic";
	}
	virtual std::string getName() {
		return "Output";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class OutputAtom;
};

/**
 * See OutputController.
 */
class OutputAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	OutputController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	OutputAtom(OutputController & parent, int index);
	virtual ~OutputAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class OutputController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Output_HEADER */

