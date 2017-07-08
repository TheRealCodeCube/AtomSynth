/*
 * AnimatedWidgets.h
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_ANIMATEDWIDGETS_H_
#define SOURCE_GUI_ANIMATEDWIDGETS_H_

#include "../JuceLibraryCode/JuceHeader.h"
#include "Colours.h"
#include "Global.h"
#include "SimpleWidgets.h"
#include "Technical/AudioBuffer.h"
#include "Technical/Automation.h"
#include "Technical/GlobalNoteStates.h"
#include "Technical/SaveState.h"

namespace AtomSynth
{

class Atom;
class AtomController;
class AtomKnob;

#define AUTOMATION_INPUTS 10
#define RCM_WIDTH (260 + 16 + 8)
#define RCM_HEIGHT (64 + 74 + 12 + 16)
#define RCM_INPUT_SPACING

class AutomationEditor : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener () { };
		virtual void automationChanged(AutomationEditor * editor) = 0;
	};
private:
	enum class Dragging { NOTHING, MIN, BOTH, MAX };
	AtomKnob * m_editing;
	int m_px, m_py, m_influence;
	Dragging m_dragging;
	std::vector<Listener *> m_listeners;

	static constexpr int
		TAB_WIDTH = HS(2),
		TAB_HEIGHT = HS(1),
		TAB_SPACING = C::SPACING,
		TAB_RADIUS = TAB_HEIGHT / 2,
		TAB_DIAMETER = TAB_HEIGHT,
		LANE_WIDTH = TAB_WIDTH * AUTOMATION_INPUTS + TAB_RADIUS * 2,
		LANE_HEIGHT = TAB_HEIGHT + TAB_SPACING,
		TOP_PART_HEIGHT = C::SPACING * 2 + C::KNOB_SIZE,
		SKINNY_HEIGHT = TAB_HEIGHT / 2,
		HANDLE_RADIUS = SKINNY_HEIGHT / 2,
		HANDLE_DIAMETER = SKINNY_HEIGHT,
		GRACE_RANGE = 6;
public:
	static constexpr int
		WIDTH = TAB_SPACING * 2 + TAB_RADIUS * 2 + TAB_WIDTH * AUTOMATION_INPUTS,
		HEIGHT = TAB_SPACING * 5 + TAB_HEIGHT * 4;

	AutomationEditor();
	virtual ~AutomationEditor() { };

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & e);
	virtual void mouseDrag(const MouseEvent & e);

	virtual void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void setEditing(AtomKnob * editing) { m_editing = editing; }
	AtomKnob * getEditing() { return m_editing; }
};

class AtomKnob : public AutomatedControl, public EnhancedComponent
{
private:
	class ThisRightClickMenu : public RightClickMenu, public MultiButton::Listener
	{
	private:
		AtomKnob & m_editing;
		AutomationEditor m_automationEditor;
		MultiButton m_mixModeSelector;
		Point<int> m_previousPos, m_prevMousePos;
		Component * m_previousParent;
		int m_dragging, m_influence, m_previousSize;
	public:
		static constexpr int
			TOP_PART_HEIGHT = C::SPACING * 2 + C::KNOB_SIZE,
			BOX_X = DS_SIZE + C::SPACING,
			BOX_Y = DS_SIZE + TOP_PART_HEIGHT,
			MIXMODE_X = DS_SIZE + C::SPACING * 2 + C::KNOB_SIZE,
			TOP_PART_Y = DS_SIZE + C::SPACING,
			WIDTH = (DS_SIZE * 2 + C::SPACING * 2 + AutomationEditor::WIDTH),
			HEIGHT = (DS_SIZE * 2 + TOP_PART_HEIGHT + AutomationEditor::HEIGHT + C::SPACING);

		ThisRightClickMenu(AtomKnob & editing);
		virtual ~ThisRightClickMenu();
		virtual void multiButtonPressed(MultiButton *);
	};
	bool m_menuOpened, m_dragging;
	Point<int> m_prevMousePos;
protected:
	virtual void valueChanged() { repaintAsync(); }
public:
	static constexpr double
		ANGLE_REDUCTION = 0.96;
	static constexpr int
		X_SPACING = 8,     //X-axis spacing of automation indicators
		Y_SPACING = 4,     //Y-axis spacing of automation indicators
		Y_OFFSET = 16,     //How far up from the bottom the automation indicators are placed
		AI_SIZE = 6,       //Diameter of the automation indicators
		CAP = 0;

	AtomKnob();
	AtomKnob(std::string suffix, bool bounded = false, int isInt = false);
	virtual ~AtomKnob();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	void setMenuClosed() { m_menuOpened = false; }

	double getAngle();
};

class AtomSlider : public AutomatedControl, public EnhancedComponent
{
private:
	class ThisRightClickMenu : public RightClickMenu, public MultiButton::Listener
	{
	private:
		AtomSlider & m_editing;
		MultiButton m_mixModeSelector;
		Point<int> m_previousPos, m_prevMousePos;
		Component * m_previousParent;
		int m_dragging, m_influence;
	public:
		ThisRightClickMenu(AtomSlider & editing);
		virtual ~ThisRightClickMenu();
		virtual void multiButtonPressed(MultiButton *);
	};
	bool m_menuOpened, m_dragging;
	double m_leftLevel, m_rightLevel;
	int m_px, m_py;
protected:
	virtual void valueChanged() { repaintAsync(); }
public:
	static constexpr int
		HANDLE_WIDTH = C::CELL_SIZE - C::SPACING * 2,
		HANDLE_HEIGHT = C::HALF_CELL_SIZE,
		UNUSABLE_SPACE = HANDLE_HEIGHT + C::SPACING * 2;
	AtomSlider();
	virtual ~AtomSlider() { }

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);

	void setAudioLevels(double left, double right) { m_leftLevel = left; m_rightLevel = right; repaintAsync(); }
};

class DetuneKnob : public AtomKnob
{
public:
	DetuneKnob(int range, std::string suffix);
	virtual ~DetuneKnob();
};

#define CENTS_INTERVAL 1.00057778951 //1200th root of 2
class CentsKnob : public DetuneKnob
{
public:
	CentsKnob();
	virtual ~CentsKnob();
	static double detune(double hz, double cents) { return hz * pow(CENTS_INTERVAL, cents); }
};

#define SEMITONES_INTERVAL 1.05946309436 //12th root of 2
class SemitonesKnob : public DetuneKnob
{
public:
	SemitonesKnob();
	virtual ~SemitonesKnob();
	static double detune(double hz, double semitones) { return hz * pow(SEMITONES_INTERVAL, semitones); }
};

#define OCTAVES_INTERVAL 2.0 //1th root of 2
class OctavesKnob : public DetuneKnob
{
public:
	OctavesKnob();
	virtual ~OctavesKnob();
	static double detune(double hz, double octaves) { return hz * pow(OCTAVES_INTERVAL, octaves); }
};

class PlotBase : public EnhancedComponent
{
public:
	enum class CursorMode {NONE, VERTICAL, CROSSHAIR};
private:
	int m_xLines, m_yLines, m_cursorX, m_cursorY;
	CursorMode m_cursorMode;
protected:
	void paintBg(Graphics & g);
	void paintFg(Graphics & g);
public:
	PlotBase();
	virtual ~PlotBase();
	void setLines(int xLines, int yLines) { m_xLines = xLines; m_yLines = yLines; }
	virtual void clear();
	virtual void finish();
	void setCursorPos(double x);
	void setCursorPos(double x, double y, int padx = 0, int pady = 0);
	void setCursorPosAbsolute(int x, int y);
	void setCursorMode(CursorMode mode) { m_cursorMode = mode; }
};

class GenericPlot : public PlotBase
{
private:
	std::vector<std::pair<double, double>> m_values;
public:
	GenericPlot() { };
	virtual ~GenericPlot() { };
	virtual void paint(Graphics & g);
	void addPoint(double x, double y);
	virtual void clear();
};

class WaveformPlot : public PlotBase
{
private:
	std::vector<double> m_values;
public:
	WaveformPlot();
	virtual ~WaveformPlot();
	virtual void paint(Graphics & g);
	void addPoint(double value);
	void setDataFromAudioBuffer(AudioBuffer & buf, bool clip = true);
	virtual void clear();
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_ANIMATEDWIDGETS_H_ */
