/*
 * HighLevelGui.h
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_HIGHLEVELGUI_H_
#define SOURCE_GUI_HIGHLEVELGUI_H_

#include "JuceLibraryCode/JuceHeader.h"
#include "Gui/SimpleWidgets.h"

namespace AtomSynth {

class AtomController;

/**
 * Displays a network of interconnected
 * AtomController objects.
 */
class AtomNetworkWidget: public Component {
public:
	/**
	 * Extend this class to listen for when the
	 * user selects a different AtomController in
	 * the network.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called whenever the currently selected
		 * AtomController is changed, usually because
		 * the user clicked on another AtomController.
		 * @param oldController The AtomController that was previously selected.
		 * @param newController The new AtomController that was just selected.
		 */
		virtual void currentAtomChanged(AtomController * oldController, AtomController * newController) = 0;
	};

private:
	enum class DragStatus {
		MOVING, IN_TO_OUT, OUT_TO_IN
	};
	std::vector<Listener *> m_listeners;
	AtomController * m_currentAtom;
	int m_xOffset, m_yOffset, m_currentTab;
	int m_px, m_py;
	DragStatus m_dragStatus;

	void drawAtomController(Graphics & g, AtomSynth::AtomController * controller);

public:
	AtomNetworkWidget();
	virtual ~AtomNetworkWidget();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent & event);
	/**
	 * Add a listener for changes on this component.
	 * @param listener The listener to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}
	/**
	 * Gets a pointer to the AtomController last
	 * clicked on by the user. This can return
	 * nullptr.
	 * @return A pointer to the AtomController last clicked on by the user.
	 */
	AtomController * getCurrentAtom() {
		return m_currentAtom;
	}
	/**
	 * Forcibly sets the current AtomController.
	 * @param current Which AtomController to set to be the current one. Can be nullptr.
	 */
	void setCurrentAtom(AtomController * current);
	/**
	 * Gets the x offset of the GUI. This is changed
	 * whenever the user pans around the network.
	 * @return The x offset of the GUI.
	 */
	int getXOffset() {
		return m_xOffset;
	}
	/**
	 * Gets the y offset of the GUI. This is changed
	 * whenever the user pans around the network.
	 * @return The y offset of the GUI.
	 */
	int getYOffset() {
		return m_yOffset;
	}
	/**
	 * Sets the offset of the GUI. Normally, this
	 * is changed by the user panning around the
	 * network.
	 * @param ox New x offset.
	 * @param oy New y offset.
	 */
	void setOffset(int ox, int oy) {
		m_xOffset = ox;
		m_yOffset = oy;
		repaint();
	}
};

class PropertiesSidepane: public Rectangle, public TextEntry::Listener, public TextButton::Listener {
private:
	TextEntry m_name;
#ifdef CUSTOM_SAVE_LOAD
	TextButton m_saveNow;
	Label *m_lastSaved;
#else
	TextButton m_copyToClipboard, m_pasteFromClipboard;
#endif
	TextButton m_createDebugLog;
	class UpdateContentTimer: public Timer {
	private:
		PropertiesSidepane *m_parent;
	public:
		UpdateContentTimer(PropertiesSidepane *parent):
				m_parent(parent) {

		}
		virtual void timerCallback();
	};
	UpdateContentTimer m_updateContentTimer;
	friend class UpdateContentTimer;
public:
	PropertiesSidepane();
	virtual ~PropertiesSidepane();

	virtual void textEntryChanged(TextEntry *entry);
	virtual void textButtonPressed(TextButton *button);
};

/**
 * The complete GUI to edit an entire Synth object.
 */
class AtomSynthEditor: public Component, public KeyListener, public ImageButton::Listener, public AtomNetworkWidget::Listener, public TextButton::Listener {
private:
	Rectangle m_addAtom;
	PropertiesSidepane m_properties;
	Component m_buttons;
	enum class Sidepane {
		NOTHING, EDITOR, ADD
	};
	Sidepane m_sidepane;
	AtomNetworkWidget m_network;
	MultiButton m_synthSaveLoadButton;
	TextEntry m_synthNameEntry;
	ImageButton m_toggleSidepane, m_add, m_open;
	AtomSynth::ImageButton m_saveLoadButton, m_addDeleteButton;
	void switchView(std::string name);
	void createNewAtom(std::string name);
	void presetLoaded();
public:
	AtomSynthEditor();
	~AtomSynthEditor();

	virtual void paint(Graphics & g);
	virtual void paintOverChildren(Graphics & g);

	/**
	 * Triggers a repaint.
	 */
	void updateScreen();

	virtual bool keyPressed(const KeyPress & key, Component * originatingComponent);
	virtual bool keyStateChanged(bool isKeyDown, Component * originatingComponent);

	///Event handler.
	virtual void imageButtonPressed(AtomSynth::ImageButton * button);
	///Event handler.
	virtual void currentAtomChanged(AtomController * oldController, AtomController * newController);
	///Event handler.
	virtual void textButtonPressed(TextButton * button);

	/**
	 * Sets the offset of the AtomNetworkWidget used by the
	 * AtomSynthEditor. See AtomNetworkWidget::setOffset().
	 * @param ox New x offset.
	 * @param oy New y offset.
	 */
	void setOffset(int ox, int oy) {
		m_network.setOffset(ox, oy);
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_HIGHLEVELGUI_H_ */
