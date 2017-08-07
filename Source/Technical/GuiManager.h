/*
 * GuiManager.h
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_GUIMANAGER_H_
#define SOURCE_TECHNICAL_GUIMANAGER_H_

#include "Atoms/Atom.h"

namespace AtomSynth {

class Synth;

class GuiManager: public MouseListener {
private:
	bool m_shouldReloadGuis = false;
	Component * m_currentRightClickMenu = nullptr;
	Component * m_rootComponent = nullptr;
	Synth * m_parent = nullptr;
	void setup();

	friend class Synth;
public:
	GuiManager();
	virtual ~GuiManager();

	/**
	 * Renders a right-click menu on top of everything,
	 * and closes it once it loses focus.
	 * @param newRightClickMenu The Component to render on top of everything else.
	 */
	void openRightClickMenu(Component * newRightClickMenu);
	/**
	 * Closes any right-click menu that is open. See
	 * openRightClickMenu() for more details.
	 */
	void closeRightClickMenu();
	/**
	 * Gets the currently open right-click menu. See
	 * openRightClickMenu() for more details.
	 * @return The currently open right-click menu. Can be nullptr if no menu is opened.
	 */
	Component * getRightClickMenu() {
		return m_currentRightClickMenu;
	}
	/**
	 * This component is rendered inside of the synth,
	 * and under any right-click menus.
	 * @param newRootComponent The main Component that should be rendered.
	 */
	void setRootComponent(Component * newRootComponent) {
		m_rootComponent = newRootComponent;
	}

	/**
	 * Juce mouseDown event.
	 * @param event Juce MouseEvent object.
	 */
	virtual void mouseDown(const MouseEvent & event);

	/**
	 * This value is used by the GUI to add all
	 * AtomController objects' GUIs to itself.
	 * @param reloaded If set to true, the above will happen.
	 */
	void setReloadGuis(bool reload = true) {
		m_shouldReloadGuis = reload;
	}
	/**
	 * This value is used by the GUI to add all
	 * AtomController objects' GUIs to itself.
	 * @return True if setReloadGuis() was previously called.
	 */
	bool shouldReloadGuis() {
		return m_shouldReloadGuis;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_GUIMANAGER_H_ */
