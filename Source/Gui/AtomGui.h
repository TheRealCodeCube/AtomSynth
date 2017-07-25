/*
 * Widgets.h
 *
 *  Created on: Apr 3, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_ATOMGUI_H_
#define SOURCE_GUI_ATOMGUI_H_

#include "AnimatedWidgets.h"
#include "SimpleWidgets.h"
#include "Colours.h"

namespace AtomSynth {

/**
 * A GUI used for user interaction
 * with an AtomController.
 */
class AtomGui: public Component {
private:
	AtomController * m_atomController;
public:
	AtomGui();
	virtual ~AtomGui();
	void paint(Graphics & g);
	/**
	 * Add a component to the GUI. It is
	 * preferable to use addAndMakeVisible().
	 * @param toAdd
	 */
	void addComponent(Component * toAdd) {
		addAndMakeVisible(toAdd);
	}
	/**
	 * Sets the parent component of this gui.
	 * This is used to add the gui to the main
	 * VST component.
	 * @param parent The parent component to add this to.
	 */
	void setParent(Component * parent);
	/**
	 * Stores a reference to the AtomController
	 * that is using this GUI. Currently, it is
	 * unused.
	 * @param atomController The AtomController that is using this GUI.
	 */
	void setAtomController(AtomController * atomController) {
		m_atomController = atomController;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_ATOMGUI_H_ */
