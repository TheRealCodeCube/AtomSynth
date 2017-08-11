/*
 * GuiManager.h
 *
 *  Created on: Aug 7, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_GUIMANAGER_H_
#define SOURCE_TECHNICAL_GUIMANAGER_H_

#include "Atoms/Atom.h"
#include "Gui/SimpleWidgets.h"

namespace AtomSynth {

class GuiManager;
class MessageOverlay;
class Synth;

/**
 * A worker thread for MessageOverlay which
 * repaints it every now and then to make
 * sure that the latest messages are shown.
 */
class MessageOverlayRepaintThread: public Thread {
private:
	MessageOverlay *m_parent;
public:
	MessageOverlayRepaintThread(MessageOverlay *parent);
	virtual ~MessageOverlayRepaintThread();

	virtual void run();
};

class MessageOverlay: public EnhancedComponent {
private:
	GuiManager *m_parent = nullptr;
	MessageOverlayRepaintThread m_repaintThread;
public:
	MessageOverlay();
	virtual ~MessageOverlay();

	void setParent(GuiManager *parent);
	virtual void paintOverChildren(Graphics &g);
};

class GuiManager: public MouseListener {
private:
	bool m_shouldReloadGuis = false;
	Component * m_currentRightClickMenu = nullptr;
	Component * m_rootComponent = nullptr;
	Synth * m_parent = nullptr;
	std::vector<std::pair<std::string, unsigned int>> m_messages;
	void setup();
	unsigned int getTime(); ///< The c++ time library is a mess, I just want milliseconds!

	friend class MessageOverlay;
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
	void setRootComponent(Component * newRootComponent);
	/**
	 * Triggers a repaint of the entire root component.
	 * This is used when a patch is loaded, and the entire
	 * gui needs to update.
	 */
	void repaintRootComponent();

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

	/**
	 * Adds a message to be displayed to the user. It
	 * should show on the screen and disappear after 5
	 * seconds.
	 * @param message The message to display.
	 */
	void addMessage(std::string message);
	/**
	 * Remove messages that have passed their allotted
	 * show time.
	 */
	void clearOldMessages();
	/**
	 * Returns a vector of messages to be displayed to
	 * the user. The unsigned int is the time that it
	 * should stop being displayed.
	 * @return A vector of messages.
	 */
	std::vector<std::pair<std::string, unsigned int>>& getMessages() {
		return m_messages;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_GUIMANAGER_H_ */
