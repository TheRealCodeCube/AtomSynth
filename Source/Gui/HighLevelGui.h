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

namespace AtomSynth
{

class AtomController;
class AtomControllerPreview;

class AtomNetworkWidget : public Component
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void currentAtomChanged(AtomController * oldController, AtomController * newController) = 0;
	};

private:
	enum class DragStatus { MOVING, IN_TO_OUT, OUT_TO_IN };
	std::vector<Listener *> m_listeners;
	AtomController * m_currentAtom;
	int m_xOffset, m_yOffset, m_currentTab;
	int m_px, m_py;
	bool m_shouldShowGuis;
	DragStatus m_dragStatus;

	void drawAtomController(Graphics & g, AtomSynth::AtomController * controller);

public:
	AtomNetworkWidget();
	virtual ~AtomNetworkWidget();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent & event);
	void setShouldShowGuis(bool shouldShow) { m_shouldShowGuis = shouldShow; }
	void addListener(Listener * listener) { m_listeners.push_back(listener); }
	AtomController * getCurrentAtom() { return m_currentAtom; }
	void setCurrentAtom(AtomController * current);
	int getXOffset() { return m_xOffset; }
	int getYOffset() { return m_yOffset; }
	void setOffset(int ox, int oy) { m_xOffset = ox; m_yOffset = oy; repaint(); }
};

class PresetBrowser : public Component
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void selectedFileChanged(PresetBrowser * browser) = 0;
	};
private:
	struct GuiElement
	{
		std::string m_name;
		juce::File m_path;
		bool m_isFolder, m_expanded;
		int m_depth;
		GuiElement * m_parent;
		std::vector<GuiElement *> m_contains;

		~GuiElement() { for(GuiElement * e : m_contains) delete(e); }
	};
	std::vector<Listener *> m_listeners;
	static Image s_closedFolder, s_openFolder, s_preset;
	GuiElement m_root;
	GuiElement * m_selected, * m_savePath;
	std::string m_saveName;

	void update(GuiElement & toUpdate);
	GuiElement * find(int & index, GuiElement * element);
	GuiElement * find(juce::File path, GuiElement * root);
	void expandParent(GuiElement * leaf);
	int paintElement(Graphics & g, GuiElement & toPaint, int yOffset);
public:
	PresetBrowser() : Component(), m_root(), m_selected(& m_root), m_savePath(& m_root), m_saveName("new synth") { }
	virtual ~PresetBrowser() {};
	void setRoot(juce::File newRoot);
	void updateList();

	juce::File getSelectedFile() { return (m_selected == nullptr) ? m_root.m_path : m_selected->m_path; }
	juce::File getSavePath() { return (m_savePath == nullptr) ? m_root.m_path : m_savePath->m_path; }
	void setSavePath(juce::File savePath);

	void rename(std::string name);
	void remove();
	void addFolder();
	bool selectFile(juce::File toSelect);
	bool addFile(std::string name, bool select = false);
	bool saveFile();
	void setSaveName(std::string name, bool force = false);
	std::string getSaveName() { return m_saveName; }

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }
};

class AtomSynthEditor :
		public Component,
		public KeyListener,
		public ImageButton::Listener,
		public MultiButton::Listener,
		public TextEntry::Listener,
		public PresetBrowser::Listener,
		public AtomNetworkWidget::Listener,
		public TextButton::Listener
{
private:
	Rectangle m_addAtom;
    Component m_properties, m_buttons;
    enum class Sidepane {NOTHING, EDITOR, ADD};
    Sidepane m_sidepane;
    AtomNetworkWidget m_network;
    MultiButton m_synthSaveLoadButton;
    TextEntry m_synthNameEntry;
    ImageButton m_noSidepane, m_propertySidepane, m_save, m_addSidepane;
    AtomSynth::ImageButton m_saveLoadButton, m_addDeleteButton;
    void switchView(std::string name);
    void createNewAtom(std::string name);
    void presetLoaded();
public:
	AtomSynthEditor();
    ~AtomSynthEditor();

    //==============================================================================
    virtual void paint (Graphics & g);
    virtual void paintOverChildren (Graphics & g);

    void updateScreen();

    virtual bool keyPressed(const juce::KeyPress & key, juce::Component * originatingComponent);
    virtual bool keyStateChanged(bool isKeyDown, Component * originatingComponent);

    virtual void imageButtonPressed(AtomSynth::ImageButton * button);
    virtual void multiButtonPressed(MultiButton * button);
    virtual void textEntryChanged(TextEntry * entry);
    virtual void selectedFileChanged(PresetBrowser * browser);
    virtual void currentAtomChanged(AtomController * oldController, AtomController * newController);
    virtual void textButtonPressed(TextButton * button);

    void setOffset(int ox, int oy) { m_network.setOffset(ox, oy); }
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_HIGHLEVELGUI_H_ */
