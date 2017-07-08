/*
 * _Template.h
 *
 *  Created on: Nov 25, 2016
 *      Author: josh
 */

#ifndef ATOM__Template_HEADER
#define ATOM__Template_HEADER

#include "Atoms/Atom.h"

namespace AtomSynth
{

class _TemplateController : public AtomController
{
public:
	_TemplateController();
	virtual AtomController * createNewInstance() { return new _TemplateController(); }
	virtual ~_TemplateController() { }
	virtual Atom * createAtom(int index);
	virtual std::string getCategory() { return "ungrouped"; } //Replace this with the category your atom belongs to.
	virtual std::string getName() { return "_Template"; }
	virtual SaveState saveSaveState();
	virtual void loadSaveState(SaveState state);

	friend class _TemplateAtom;
};

class _TemplateAtom : public Atom
{
private:
	_TemplateController & m_parent;
public:
	_TemplateAtom(_TemplateController & parent, int index);
	virtual ~_TemplateAtom() { }
	virtual void execute();
	virtual void reset();
};

}

#endif /* ATOM__Template_HEADER */
