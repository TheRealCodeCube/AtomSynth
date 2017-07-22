/*
 * _Template.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: josh
 */

#include "_Template.h"

namespace AtomSynth {

//Constructor for the controller.
_TemplateController::_TemplateController() :
		AtomController(AtomParameters(1000, 2, true, 1)) { //A unique ID for this type of controller, the number of primary inputs, whether or not to enable automation inputs, and the number of outputs.
	init(); //Must be at the beginning of the constructor. Initializes child atoms.

	m_gui.setAtomController(this); //Must be at the end of the constructor. Lets the gui know about information like the atom's name.
}

//This is a wrapper for _TemplateController's constructor
AtomController * create_TemplateController() {
	return new _TemplateController();
}

//This a wrapper for _TemplateAtom's constructor
Atom * _TemplateController::createAtom(int index) {
	return new _TemplateAtom(*this, index); //The index is which polyphony the atom is part of.
}

SaveState _TemplateController::saveSaveState() { //Write the parameters of the atom to a save state.
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());
	SaveState & extraData = *new SaveState();
	//Add code to pack your data to extraData
	toReturn.addState(extraData);
	return toReturn;
}

void _TemplateController::loadSaveState(SaveState state) { //Set all the parameters of the atom from a save state.
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	//Add code to unpack your data here
}

_TemplateAtom::_TemplateAtom(_TemplateController & parent, int index) :
		Atom(parent, index), //Superclass constructor. Takes a reference to the parent controller and a polyphony index.
		m_parent(parent) { //The superclass only keeps a reference to the parent as a generic AtomController. This allows access to _Template-specific things.

}

//This is where the atom takes inputs, does calculations, and writes outputs.
void _TemplateAtom::execute() {
	Atom::execute();
	//Put your code here.
}

//This is where the atom can reset itself after a note finishes playing.
void _TemplateAtom::reset() {
	Atom::reset();
	//Put your code here.
}

}
