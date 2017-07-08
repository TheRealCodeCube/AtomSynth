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

namespace AtomSynth
{

class AtomGui : public Component
{
private:
	AtomController * m_atomController;
public:
	AtomGui();
	virtual ~AtomGui();
	void paint(Graphics & g);
	void addComponent(Component * toAdd) { addAndMakeVisible(toAdd); }
	void setParent(Component * parent);
	void setAtomController(AtomController * atomController) { m_atomController = atomController; }
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_ATOMGUI_H_ */
