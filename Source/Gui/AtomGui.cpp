/*
 * AtomGui.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: josh
 */

#include "AtomGui.h"

#include "Atoms/Atom.h"
#include "Global.h"

namespace AtomSynth {

AtomGui::AtomGui() :
		m_atomController(nullptr) {
	setBounds(0, C::SPACING + C::HEADER_HEIGHT, C::GUI_WIDTH, C::GUI_HEIGHT);
	setVisible(false);
}

AtomGui::~AtomGui() {
	deleteAllChildren();
}

void AtomGui::paint(Graphics& g) {
	g.fillAll(MID_LAYER);
}

void AtomGui::setParent(Component* parent) {
	bool wasShown = isVisible();
	parent->addAndMakeVisible(this);
	if (!wasShown)
		setVisible(false); //There is no 'add and don't make visible'
}

} /* namespace AtomSynth */
