#ifndef ATOM_Input_HEADER
#define ATOM_Input_HEADER

/* BEGIN AUTO-GENERATED HEADER */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED HEADER */

namespace AtomSynth
{

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

class InputController : public AtomController
{
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	InputController();
	virtual AtomController * createNewInstance() { return new InputController(); }
	virtual ~InputController() { }

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() { return "Basic"; }
	virtual std::string getName() { return "Input"; }
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class InputAtom;
};

class InputAtom : public Atom
{
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	InputController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */

	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	InputAtom(InputController & parent, int index);
	virtual ~InputAtom() { }
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class InputController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Input_HEADER */

