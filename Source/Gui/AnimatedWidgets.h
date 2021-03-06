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
#include "Technical/SaveState.h"

namespace AtomSynth {

class Atom;
class AtomController;
class AtomKnob;

#define AUTOMATION_INPUTS 10
#define RCM_WIDTH (260 + 16 + 8)
#define RCM_HEIGHT (64 + 74 + 12 + 16)
#define RCM_INPUT_SPACING

/**
 * Used for editing automation influences of
 * AutomatedControl objects.
 */
class AutomationEditor: public EnhancedComponent {
public:
	/**
	 * Class for listening to changes in AutomationEditor.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called whenever there is a change in an AutomationEditor.
		 * @param editor The AutomationEditor which changed.
		 */
		virtual void automationChanged(AutomationEditor * editor) = 0;
	};
private:
	enum class Dragging {
		NOTHING, MIN, BOTH, MAX
	};
	AutomatedControl * m_editing;
	int m_px, m_py, m_influence, m_snapping = 0;
	double m_fakeMin = 0.0, m_fakeMax = 0.0;
	bool m_vertical = false;
	Dragging m_dragging;
	std::vector<Listener *> m_listeners;

	static constexpr double snap(double value, double snaps) {
		return double(int(value * snaps + 0.5)) / snaps;
	}

	static constexpr int TAB_WIDTH = HS(1) + 4,
			TAB_HEIGHT = HS(1),
			TAB_SPACING = C::SMALL_SPACING,
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
	static constexpr int WIDTH = TAB_SPACING * 2 + TAB_RADIUS * 2 + TAB_WIDTH * AUTOMATION_INPUTS, ///< Width of the box (when placed horizontally)
		HEIGHT = TAB_SPACING * 5 + TAB_HEIGHT * 4; ///< Height of the box (when placed horizontally)

	AutomationEditor();
	virtual ~AutomationEditor() {
	}

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & e);
	virtual void mouseDrag(const MouseEvent & e);
	virtual void mouseUp(const MouseEvent & e);

	/**
	 * Add a listener to this object. Any changes made
	 * via the GUI will cause all listeners added by
	 * this function to be informed.
	 * @param listener The listener to add.
	 */
	virtual void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Sets the AutomatedControl that this editor should edit.
	 * @param editing The AutomatedControl to edit.
	 */
	void setEditing(AutomatedControl * editing) {
		m_editing = editing;
	}
	/**
	 * Gets the AutomatedControl that this editor is editing.
	 * @return The AutomatedControl the editor is editing.
	 */
	AutomatedControl * getEditing() {
		return m_editing;
	}
	/**
	 * Sets whether this AutomationEditor should be rendered
	 * as vertical rather than horizontal.
	 * @param vertical True if it should be rendered vertical.
	 */
	void setVertical(bool vertical = true);
};

/**
 * An automatable knob. See AutomatedControl.
 */
class AtomKnob: public AutomatedControl, public EnhancedComponent {
private:
	class ThisRightClickMenu: public RightClickMenu, public MultiButton::Listener {
	private:
		AtomKnob & m_editing;
		AutomationEditor m_automationEditor;
		MultiButton m_mixModeSelector;
		Point<int> m_previousPos;
		Component * m_previousParent;
		int m_previousSize;
		static constexpr int TOP_Y = DS_SIZE + C::SPACING,
				AUTOMATION_X = DS_SIZE + C::SPACING + C::KNOB_SIZE + C::SPACING,
				MIXMODE_X = AUTOMATION_X + AutomationEditor::WIDTH + C::SPACING,
				WIDTH = MIXMODE_X + CS(2) + C::SPACING + DS_SIZE,
				HEIGHT = TOP_Y + CS(2) + C::SPACING + DS_SIZE;
	public:
		ThisRightClickMenu(AtomKnob & editing);
		virtual ~ThisRightClickMenu();
		virtual void multiButtonPressed(MultiButton *);
	};
	int m_snapping = 0;
	double m_fakeValue = 0.0;
	bool m_menuOpened, m_dragging;
	Point<int> m_prevMousePos;
protected:
	/**
	 * Call this whenever the value to be displayed changes.
	 */
	virtual void valueChanged() {
		repaintAsync();
	}
public:
	static constexpr double ANGLE_REDUCTION = 0.96; ///<Used for layout.
	static constexpr int X_SPACING = 8, ///<X-axis spacing of automation indicators
			Y_SPACING = 4, ///<Y-axis spacing of automation indicators
			Y_OFFSET = 16, ///<How far up from the bottom the automation indicators are placed
			AI_SIZE = 6, ///<Diameter of the automation indicators
			CAP = 0; ///<Used for layout.

	AtomKnob();
	virtual ~AtomKnob();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent& event);
	/**
	 * Called when the right-click menu is closed.
	 */
	void setMenuClosed() {
		m_menuOpened = false;
	}

	/**
	 * Calculates the angle the knob is pointing. Takes
	 * into consideration the minimum and maximum values
	 * to compute where the knob should be pointing.
	 * @return Angle in radians.
	 */
	double getAngle();
};

/**
 * An automatable slider. See AutomatedControl.
 */
class AtomSlider: public AutomatedControl, public EnhancedComponent {
private:
	class ThisRightClickMenu: public RightClickMenu, public MultiButton::Listener {
	private:
		AtomSlider & m_editing;
		AutomationEditor m_automationEditor;
		MultiButton m_mixModeSelector;
		Point<int> m_previousPos;
		Component * m_previousParent;
		int m_previousSize;
		static constexpr int TOP_Y = DS_SIZE + C::SPACING,
				OTHER_X = DS_SIZE + C::SPACING + CS(1) + C::SPACING,
				MIXMODE_Y = TOP_Y + AutomationEditor::WIDTH + C::SPACING,
				WIDTH = OTHER_X + CS(2) + C::SPACING + DS_SIZE,
				HEIGHT = MIXMODE_Y + CS(2) + C::SPACING + DS_SIZE,
				SLIDER_HEIGHT = HEIGHT - DS_SIZE - C::SPACING - C::SPACING - DS_SIZE;
	public:
		ThisRightClickMenu(AtomSlider & editing);
		virtual ~ThisRightClickMenu();
		virtual void multiButtonPressed(MultiButton *);
	};
	int m_snapping = 0;
	bool m_menuOpened, m_dragging;
	double m_leftLevel, m_rightLevel, m_fakeValue = 0.0;
	Point<int> m_prevMousePos;
protected:
	static constexpr int AI_SIZE = 6;
	/**
	 * Call this whenever the value to be displayed changes.
	 */
	virtual void valueChanged() {
		repaintAsync();
	}
public:
	static constexpr int HANDLE_WIDTH = C::CELL_SIZE - C::SPACING * 2, ///< Used for layout.
		HANDLE_HEIGHT = C::HALF_CELL_SIZE, ///< Used for layout.
		UNUSABLE_SPACE = HANDLE_HEIGHT + C::SPACING * 2; ///< Used for layout.
	AtomSlider();
	virtual ~AtomSlider() {
	}

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent & event);

	/**
	 * Sets audio levels to show to the user. These
	 * are rendered as green bars. Values should be
	 * in the range 0 - 1.
	 * @param left Intensity of the left bar.
	 * @param right Intensity of the right bar.
	 */
	void setAudioLevels(double left, double right) {
		m_leftLevel = left;
		m_rightLevel = right;
		repaintAsync();
	}
	/**
	 * Called when the right-click menu is closed.
	 */
	void setMenuClosed() {
		m_menuOpened = false;
	}
};

/**
 * A specialized knob for detuning frequencies. This
 * class is not used, see its child classes for useful
 * knobs. By default, it is unbounded, only shows
 * integer values, and has a range and suffix hard-coded
 * by its child classes.
 */
class DetuneKnob: public AtomKnob {
public:
	/**
	 * Constructor. Only used by child classes.
	 * @param range Range of the knob.
	 * @param suffix Suffix of the knob.
	 */
	DetuneKnob(int range, std::string suffix);
	virtual ~DetuneKnob();
};

#define CENTS_INTERVAL 1.00057778951 //1200th root of 2
/**
 * Subclass of DetuneKnob which detunes values in
 * cents. (The range is 100, ~1 semitone, and the
 * suffix is c for cents.)
 */
class CentsKnob: public DetuneKnob {
public:
	CentsKnob();
	virtual ~CentsKnob();
	/**
	 * Detunes a frequency by a certain number of cents.
	 * @param hz Base frequency (must be greater than 0.)
	 * @param cents Number of cents to detune up or down (can be any number.)
	 * @return The detuned frequency.
	 */
	static double detune(double hz, double cents) {
		return hz * pow(CENTS_INTERVAL, cents);
	}
};

#define SEMITONES_INTERVAL 1.05946309436 //12th root of 2
/**
 * Subclass of DetuneKnob which detunes values in
 * semitones. (The range is 12, ~1 octave, and the
 * suffix is s for semitones.)
 */
class SemitonesKnob: public DetuneKnob {
public:
	SemitonesKnob();
	virtual ~SemitonesKnob();
	/**
	 * Detunes a frequency by a certain number of semitones.
	 * @param hz Base frequency (must be greater than 0.)
	 * @param semitones Number of semitones to detune up or down (can be any number.)
	 * @return The detuned frequency.
	 */
	static double detune(double hz, double semitones) {
		return hz * pow(SEMITONES_INTERVAL, semitones);
	}
};

#define OCTAVES_INTERVAL 2.0 //1th root of 2
/**
 * Subclass of DetuneKnob which detunes values in
 * octaves. (The range is 4, and the suffix is o
 * for cents.)
 */
class OctavesKnob: public DetuneKnob {
public:
	OctavesKnob();
	virtual ~OctavesKnob();
	/**
	 * Detunes a frequency by a certain number of octaves.
	 * @param hz Base frequency (must be greater than 0.)
	 * @param octaves Number of octaves to detune up or down (can be any number.)
	 * @return The detuned frequency.
	 */
	static double detune(double hz, double octaves) {
		return hz * pow(OCTAVES_INTERVAL, octaves);
	}
};

/**
 * A generic class for creating plots and graphs. This
 * class has no actual plotting capabilities, but
 * serves as a base class for other classes which do.
 */
class PlotBase: public EnhancedComponent {
public:
	/**
	 * Types of 'cursors' that PlotBase can render to indicate
	 * points of time inside the plot.
	 */
	enum class CursorMode {
		NONE, ///< No cursor should be rendered.
		VERTICAL, ///< Only a vertical cursor (one straight bar) should be rendered.
		CROSSHAIR ///< Render both a horizontal and vertical bar.
	};
private:
	int m_xLines, m_yLines, m_cursorX, m_cursorY;
	double m_xSkew = 1.0, m_ySkew = 1.0;
	CursorMode m_cursorMode;
	void checkX();
	void checkY();
protected:
	/**
	 * Paint the background of the plot. Call this
	 * before painting content onto the graph.
	 * @param g A Juce Graphics object, as passed into paint().
	 */
	void paintBg(Graphics & g);
	/**
	 * Paint the foreground of the plot. Call this
	 * after painting the content onto the graph.
	 * @param g A Juce Graphics object, as passed into paint().
	 */
	void paintFg(Graphics & g);
public:
	PlotBase();
	virtual ~PlotBase();
	/**
	 * Set the number of lines that should be rendered
	 * in the background.
	 * @param xLines Number of vertical lines to render.
	 * @param yLines Number of horizontal lines to render.
	 */
	void setLines(int xLines, int yLines) {
		m_xLines = xLines + 1;
		m_yLines = yLines + 1;
	}
	void setXLines(int xLines) {
		m_xLines = xLines + 1;
	}
	void setYLines(int yLines) {
		m_yLines = yLines + 1;
	}
	void setSkews(double xSkew, double ySkew) {
		m_xSkew = xSkew;
		m_ySkew = ySkew;
	}
	void setXSkew(double xSkew) {
		m_xSkew = xSkew;
	}
	void setYSkew(double ySkew) {
		m_ySkew = ySkew;
	}
	/**
	 * Clear all data added to the plot.
	 */
	virtual void clear();
	/**
	 * Finish writing data to the plot. This will trigger
	 * a repaint. Make sure to call this once and only once
	 * after writing all data to the plot for optimal
	 * performance.
	 */
	virtual void finish();
	/**
	 * Set the x coordinate of the cursor. Values are given
	 * in percentages of the width of the plot.
	 * @param x A double from 0 - 1, indicating the x position of the cursor.
	 */
	void setCursorPos(double x);
	/**
	 * Set both the x and y coordinates of the cursor. Values
	 * are given in percentages of the width and height of the
	 * plot, respectively. Additionally, padding, measured in
	 * pixels, can be provided.
	 * @param x A double from 0 - 1, representing the x coordinate.
	 * @param y A double from 0 - 1, representing the y coordinate.
	 * @param padx Number of pixels to 'trim off of' the width when calculating the real cursor position.
	 * @param pady Number of pixels to 'trim off of' the height when calculating the real cursor position.
	 */
	void setCursorPos(double x, double y, int padx = 0, int pady = 0);
	/**
	 * Set the cursor position to absolute pixel coordinates.
	 * @param x X coordinate of cursor, in pixels.
	 * @param y Y coordinate of cursor, in pixels.
	 */
	void setCursorPosAbsolute(int x, int y);
	/**
	 * Set the render mode of the cursor.
	 * @param mode How the cursor should be rendered.
	 */
	void setCursorMode(CursorMode mode) {
		m_cursorMode = mode;
	}
};

/**
 * Plots arbitrary x-y pairs with a connected line.
 */
class GenericPlot: public PlotBase {
private:
	std::vector<std::pair<double, double>> m_values;
public:
	GenericPlot() {
	}
	virtual ~GenericPlot() {
	}
	virtual void paint(Graphics & g);
	/**
	 * Adds a point to the plot. The coordinates
	 * are given in percentages of the width and
	 * height of the plot.
	 * @param x A double from 0 - 1, representing the x coordinate.
	 * @param y A double from 0 - 1, representing the y coordinate.
	 */
	void addPoint(double x, double y);
	virtual void clear();
};

/**
 * Plots a series of samples in the range of
 * -1 to 1.
 */
class WaveformPlot: public PlotBase {
protected:
	std::vector<double> m_values;
public:
	WaveformPlot();
	virtual ~WaveformPlot();
	virtual void paint(Graphics & g);
	/**
	 * Add a sample to the plot.
	 * @param value A double from -1 to 1 to add to the plot.
	 */
	void addPoint(double value);
	/**
	 * Adds samples from the left channel of an
	 * AudioBuffer. If clip is set to true, then
	 * this will only add as many samples as there
	 * are pixels. This can look better, but will
	 * appear to cut off the end of the audio buffer.
	 * @param buf The buffer to source samples from.
	 * @param clip Whether or not to limit the number of samples retrieved from the AudioBuffer.
	 */
	void setDataFromAudioBuffer(AudioBuffer & buf, bool clip = true);
	/**
	 * 'Draws' samples from the left channel of an
	 * AudioBuffer. This is different
	 * from setDataFromAudioBuffer() in that it will
	 * use decimal indexes to retrieve one sample for
	 * each pixel. If your AudioBuffer has antialiasing
	 * on the AudioBuffer::get() function, it will show
	 * up on the WaveformPlot.
	 * @param buf The buffer to draw data from.
	 * @param start The index to start drawing from.
	 * @param end The index to stop drawing at.
	 */
	void drawDataFromAudioBuffer(AudioBuffer & buf, double start = 0.0, double end = -1.0);
	virtual void clear();
};

/**
 * A plot that can be drawn on by the user. (Max
 * 256 samples.)
 */
class DrawablePlot: public WaveformPlot {
private:
	void draw(double x, double y);
	int m_px = 0, m_py = 0;
public:
	DrawablePlot();
	virtual ~DrawablePlot();

	virtual void mouseDown(const MouseEvent& event);
	virtual void mouseDrag(const MouseEvent& event);
	virtual void mouseUp(const MouseEvent& event);

	void setLength(int length) {
		m_values.resize(std::min(length, 256), 0.0);
	}
	std::vector<double>& getValues() {
		return m_values;
	}
	/**
	 * Gets a linear interpolated value at
	 * the specified x coordinate (in the
	 * range of 0 to 1.
	 * @param point The x coordinate to get the value from (in the range 0 to 1.)
	 * @return The linear interpolated value.
	 */
	double getValueAt(double point);

	void createDiagonalLine();

	SaveState saveSaveState();
	void loadSaveState(SaveState state);
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_ANIMATEDWIDGETS_H_ */
