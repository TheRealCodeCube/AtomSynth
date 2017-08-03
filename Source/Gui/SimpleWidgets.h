/*
 * SimpleWidgets.h
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_SIMPLEWIDGETS_H_
#define SOURCE_GUI_SIMPLEWIDGETS_H_

#include "JuceLibraryCode/JuceHeader.h"
#include "Gui/Colours.h"
#include "Technical/GlobalNoteStates.h"

namespace AtomSynth {

//Forward declarations
class SaveState;

/**
 * Juce Component extended with some helpful methods.
 */
class EnhancedComponent: public Component, private AsyncUpdater {
private:
	void handleAsyncUpdate() {
		repaint();
	}
public:
	/**
	 * Creates a label for this component with the
	 * specified text.
	 * @param text The text the label should contain.
	 * @param top Whether the label should be positioned above or below the component.
	 * @return A new Label that can be added to this component's parent.
	 */
	Label * createLabel(std::string text, bool top = true);
	/**
	 * Triggers a repaint. This can be called from any
	 * thread without causing problems.
	 */
	void repaintAsync() {
		triggerAsyncUpdate();
	}
};

/**
 * A grey box with a drop shadow.
 */
class RightClickMenu: public Component {
private:
	bool m_firstPress;
public:
	///Used for layout.
	static constexpr int DS_SIZE = 8, ///< Size of the drop shadow.
		DS_CORNER = 3; ///< Size of the corners of the drop shadow.

	RightClickMenu() :
			Component(),
			m_firstPress(true) {
		this->removeMouseListener(this);
	}
	virtual ~RightClickMenu() {
		Desktop::getInstance().removeGlobalMouseListener(this);
	}
	/**
	 * Juce paint function.
	 * @param g Juce Graphics object.
	 */
	virtual void paint(Graphics & g);
};

/**
 * A rounded button with text inside it.
 */
class TextButton: public EnhancedComponent {
public:
	/**
	 * Extend this to be informed of mouse clicks on TextButton objects.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called when a TextButton is pressed.
		 * @param textButton The TextButton that was pressed.
		 */
		virtual void textButtonPressed(TextButton * textButton) = 0;
	};
private:
	String m_string;
	std::vector<Listener *> m_listeners;
	bool m_flash;
public:
	TextButton() :
			EnhancedComponent(),
			m_flash(false) {
	}
	virtual ~TextButton() {
	}

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent & event);
	/**
	 * Add a listener for clicks on this component.
	 * @param listener The listener to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Sets the text displayed on the button.
	 * @param text The text to display.
	 */
	void setText(std::string text) {
		m_string = String(text);
		repaint();
	}
	/**
	 * Gets the text currently displayed on the button.
	 * @return The text displayed on the button.
	 */
	std::string getText() {
		return m_string.toStdString();
	}
};

/**
 * A rounded container with several selectable options, called labels.
 */
class MultiButton: public EnhancedComponent {
public:
	/**
	 * Extend this to be informed of mouse clicks on MultiButton objects.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called when a MultiButton is clicked.
		 * @param multiButton The MultiButton that was clicked.
		 */
		virtual void multiButtonPressed(MultiButton * multiButton) = 0;
	};
private:
	int m_selectedLabel;
	std::vector<std::string> m_labels;
	std::vector<Listener *> m_listeners;
	bool m_vertical;
public:
	MultiButton();
	virtual ~MultiButton();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	/**
	 * Add a listener for clicks on this component.
	 * @param listener The listener to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Adds a label to this MultiButton.
	 * @param label Text that should show.
	 */
	void addLabel(std::string label) {
		m_labels.push_back(label);
	}
	/**
	 * Clears all labels from this MultiButton.
	 */
	void clearLabels() {
		m_labels.clear();
	}
	/**
	 * Sets which label is currently selected by
	 * index. Does not check if the index is in
	 * bounds.
	 * @param index The index of the label to select.
	 */
	void setSelectedLabel(int index);
	/**
	 * Gets the index of the currently selected label.
	 * @return The index of the currently selected label.
	 */
	int getSelectedLabel() {
		return m_selectedLabel;
	}
	/**
	 * Gets a vector of all labels in this MultiButton.
	 * @return A vector of all labels.
	 */
	std::vector<std::string> & getLabels() {
		return m_labels;
	}
	/**
	 * Stack options vertically, rather than horizontally (the default.)
	 * @param vertical Whether or not to stack options vertically.
	 */
	void setVertical(bool vertical = true) {
		m_vertical = vertical;
	}
	/**
	 * Gets if the options are stacked vertically.
	 * @return True if the options are stacked vertically.
	 */
	bool isVertical() {
		return m_vertical;
	}

	/**
	 * Loads the MultiButton's state from a SaveState.
	 * This does not include the labels, it only contains
	 * which label is selected.
	 * @param state The SaveState to load.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves the MultiButton's state to a SaveState.
	 * This does not include the labels, it only contains
	 * which label is selected.
	 * @return A SaveState representing the MultiButton's current state.
	 */
	SaveState saveSaveState();
};

/**
 * A knob that cannot be automated.
 */
class SimpleKnob: public EnhancedComponent {
public:
	/**
	 * Extend this to be informed of changes on SimpleKnob objects.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called when a SimpleKnob is changed.
		 * @param knob The SimpleKnob that was changed.
		 */
		virtual void simpleKnobChanged(SimpleKnob * knob) = 0;
	};
private:
	Point<int> m_prevMousePos;
	std::vector<Listener *> m_listeners;
protected:
	double m_value, ///< Current value.
		m_min, ///< Lower bound.
		m_max; ///< Upper bound.
	std::string m_suffix; ///< Suffix to be rendered after the value.
	bool m_int, ///< If true, round user input to the nearest integer.
		m_bounded; ///< If true, do not allow going past m_min or m_max.
public:

	SimpleKnob();
	//SimpleKnob(double min, double max, bool bounded = false, std::string suffix = "", bool isInt = true);
	virtual ~SimpleKnob();

	virtual void paint(Graphics & g);

	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	/**
	 * Adds an object to be notified every time this
	 * knob changes.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Sets the minimum possible value.
	 * @param min The desired minimum possible value.
	 */
	void setMin(double min) {
		m_min = min;
	}
	/**
	 * Sets the maximum possible value.
	 * @param max The desired maximum possible value.
	 */
	void setMax(double max) {
		m_max = max;
	}
	/**
	 * Sets both the minimum and maximum possible
	 * values.
	 * @param min The desired minimum possible value.
	 * @param max The desired maximum possible value.
	 */
	void setRange(double min, double max) {
		m_min = min;
		m_max = max;
	}
	/**
	 * Gets the minimum possible value.
	 * @return The minimum possible value.
	 */
	double getMin() {
		return m_min;
	}
	/**
	 * Gets the maximum possible value.
	 * @return The maximum possible value.
	 */
	double getMax() {
		return m_max;
	}
	/**
	 * Sets if user input should be rounded to the nearest
	 * integer.
	 * @param isInt True if user input should be rounded to the nearest integer.
	 */
	void setInt(bool isInt) {
		m_int = isInt;
	}
	/**
	 * Gets if user input should be rounded to the nearest
	 * integer.
	 * @return True if user input should be rounded to the nearest integer.
	 */
	bool isInt() {
		return m_int;
	}
	/**
	 * Sets if the input should be bounded. If the input
	 * is not bounded, then the control will be allowed
	 * to go past getMin() and getMax().
	 * @param bounded True if input should be restricted by getMin() and getMax().
	 */
	void setBounded(bool bounded) {
		m_bounded = bounded;
	}
	/**
	 * Gets if the input should be bounded. If the input
	 * is not bounded, then the control will be allowed
	 * to go past getMin() and getMax().
	 * @return True if input is restricted by getMin() and getMax().
	 */
	bool isBounded() {
		return m_bounded;
	}

	/**
	 * Sets the suffix to render at the end of the number.
	 * @param suffix The suffix to render.
	 */
	void setSuffix(std::string suffix) {
		m_suffix = suffix;
	}

	/**
	 * Gets the current suffix. See setSuffix().
	 * @return The current suffix.
	 */
	std::string getSuffix() {
		return m_suffix;
	}

	/**
	 * Sets the value.
	 * @param value What the value should be.
	 */
	void setValue(double value) {
		m_value = value;
	}
	/**
	 * Gets the value set by the user.
	 * @return The default value.
	 */
	double getValue() {
		return (m_int) ? round(m_value) : m_value;
	}
	/**
	 * Gets the angle the knob should be pointing, taking
	 * into consideration the current value, and the
	 * minimum and maximum values.
	 * @return The angle the knob should be pointing (in radians.)
	 */
	double getAngle() {
		return -(((getValue() - m_min) / (m_max - m_min)) * M_PI * 2.0);
	}

	/**
	 * Loads the knob's state from a SaveState. See
	 * saveSaveState() for what data it contains.
	 * @param state The state to load from.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves the knob's current state to a SaveState.
	 * This only contains information about the value
	 * the user has set for it, and not its minimum,
	 * maximum, or other code-defined properties.
	 * @return A SaveState representing the knob's current state.
	 */
	SaveState saveSaveState();
};

/**
 * A class for rendering a user-changeable fraction.
 * The numerator can range from 0 to 999, and the
 * denominator can range from 1 to 999. It also has
 * methods for using this fraction as a multiple of
 * the global BPM. E.G. if the BPM is 120 and the
 * fraction is 1/2, it can tell you the amount of time
 * to traverse 1/2 of a beat at 120 BPM, which would be
 * 0.25 seconds.
 */
class BpmMultiple: public EnhancedComponent {
public:
	/**
	 * A class which can be informed of changes to a
	 * BpmMultiple.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called whenever a BpmMultiple is changed.
		 * @param bpmMultiple The BpmMultiple that was changed.
		 */
		virtual void bpmMultipleChanged(BpmMultiple * bpmMultiple) = 0;
	};
private:
	double m_numerator, m_denominator;
	bool m_editingTop;
	std::vector<Listener *> m_listeners;
	Point<int> m_prevMousePos;
	void checkBounds();
public:
	static constexpr double MOUSE_DIVISOR = 300.0, ///< Controls how fast the mouse changes values.
		SLOPE_MODIFIER = 20.0, ///< Controls how much faster the mouse changes values when the values are larger.
		MAX_VALUE = 999.0; ///< The maximum value that the numerator or denominator can have.

	BpmMultiple();
	virtual ~BpmMultiple();

	virtual void paint(Graphics & g);

	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	/**
	 * Adds an object to listen to any changes on this
	 * BpmMultiple.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Set the numerator of the fraction.
	 * @param numerator The desired numerator.
	 */
	void setNumerator(int numerator) {
		m_numerator = numerator;
		checkBounds();
		repaint();
	}
	/**
	 * Sets the denominator of the fraction.
	 * @param denominator The desired denominator.
	 */
	void setDenominator(int denominator) {
		m_denominator = denominator;
		checkBounds();
		repaint();
	}
	/**
	 * Sets the numerator and denominator of
	 * the fraction.
	 * @param numerator The desired numerator.
	 * @param denominator The desired denominator.
	 */
	void setFraction(int numerator, int denominator) {
		m_numerator = numerator, m_denominator = denominator;
		checkBounds();
		repaint();
	}

	/**
	 * Adds a delta to the fraction's numerator.
	 * @param nd The delta to add to the fraction's numerator.
	 */
	void offsetNumerator(int nd) {
		m_numerator += nd;
		checkBounds();
		repaint();
	}
	/**
	 * Adds a delta to the fraction's denominator.
	 * @param dd The delta to add to the fraction's denominator.
	 */
	void offsetDenominator(int dd) {
		m_denominator += dd;
		checkBounds();
		repaint();
	}
	/**
	 * Adds a delta to the the fraction.
	 * @param nd The delta to add to the fraction's numerator.
	 * @param dd The delta to add to the fraction's denominator.
	 */
	void offsetFraction(int nd, int dd) {
		m_numerator += nd, m_denominator += dd;
		checkBounds();
		repaint();
	}

	/**
	 * Gets the numerator of the fraction.
	 * @return The numerator of the fraction.
	 */
	int getNumerator() {
		return floor(m_numerator);
	}
	/**
	 * Gets the denominator of the fraction.
	 * @return The denominator of the fraction.
	 */
	int getDenominator() {
		return floor(m_denominator);
	}
	/**
	 * Gets the decimal representation of the fraction.
	 * @return The decimal representation of the fraction.
	 */
	double getFraction() {
		return floor(m_numerator) / floor(m_denominator);
	}
	/**
	 * Gets the number of seconds to traverse [getFraction()]
	 * beats at the current BPM. E.G. if the BPM is 120 and the
	 * fraction is 1/2, it will return the time to traverse
	 * half a beat at 120 BPM, or 0.25 seconds.
	 * @return
	 */
	double getSeconds() {
		return getFraction() * (60.0f / GlobalNoteStates::s_bpm);
	}
	/**
	 * Gets 1.0 / getSeconds(). If getSeconds() is 0, it
	 * returns 0.0001.
	 * @return 1.0 / getSeconds().
	 */
	double getHertz() {
		return (m_numerator == 0) ? 0.0001 : 1.0f / getSeconds();
	}

	/**
	 * Loads this component's state from a SaveState.
	 * See saveSaveState() for what data is contained
	 * in it.
	 * @param state The SaveState to load from.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves this component's current state to a
	 * SaveState. This only includes the numerator
	 * and denominator, nothing else.
	 * @return A SaveState representing this component's current state.
	 */
	SaveState saveSaveState();
};

/**
 * A button that shows an image on it.
 */
class ImageButton: public Component {
public:
	/**
	 * A class which can be informed of clicks on
	 * an ImageButton.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called whenever an ImageButton is pressed
		 * @param button The ImageButton that was pressed.
		 */
		virtual void imageButtonPressed(ImageButton * button) = 0;
	};

private:
	Image m_icon;
	std::string m_iconName;
	std::vector<Listener *> m_listeners;
public:
	ImageButton();
	virtual ~ImageButton();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	/**
	 * Adds an object to listen to any clicks on
	 * this ImageButton.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Sets the icon of the button given its
	 * name. It will try and get it from
	 * JuceLibraryCode/BinaryData.h. The _png
	 * extension is automatically added.
	 * @param iconName Name of the icon.
	 */
	void setIcon(std::string iconName);
	/**
	 * Gets the name of the icon currently being
	 * shown.
	 * @return The name of the icon currently being shown.
	 */
	std::string getIconName() {
		return m_iconName;
	}
};

/**
 * Just an image component.
 */
class GuiImage: public Component {
private:
	Image m_image;
public:
	GuiImage() :
			Component() {
	}
	virtual ~GuiImage() {
	}

	virtual void paint(Graphics & g);


	/**
	 * Sets the image to show given its
	 * name. It will try and get it from
	 * JuceLibraryCode/BinaryData.h. The _png
	 * extension is automatically added.
	 * @param resourceName Name of the image.
	 */
	void setImage(std::string resourceName);
};

/**
 * A rounded rectangle that can fade between
 * gray and a custom colour.
 */
class IndicatorLight: public EnhancedComponent {
private:
	double m_level;
	Colour m_colour;
public:
	IndicatorLight() :
			EnhancedComponent(),
			m_level(0.0),
			m_colour(FORE_LAYER) {
	}
	virtual ~IndicatorLight() {
	}

	virtual void paint(Graphics &);

	/**
	 * Set the custom colour to render when
	 * the level increases.
	 * @param colour The colour to render.
	 */
	void setColour(Colour colour) {
		m_colour = colour;
		repaint();
	}
	/**
	 * Get the custom colour that renders
	 * when the level increases.
	 * @return The custom colour that renders.
	 */
	Colour getColour() {
		return m_colour;
	}
	/**
	 * Sets the intensity of the custom
	 * colour. Accepts a percentage as a
	 * double between 0 and 1.
	 * @param level A percentage between 0 and 1.
	 */
	void setLevel(double level) {
		m_level = level;
		repaint();
	}
};

/**
 * A rounded rectangle that can be clicked,
 * and will toggle between two colours to
 * show its status.
 */
class OnOffButton: public EnhancedComponent {
public:
	/**
	 * A class which can be informed of clicks on
	 * an OnOffButton.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * This method is called whenever an
		 * OnOffButton is clicked.
		 * @param button The OnOffButton that was clicked.
		 */
		virtual void onOffButtonPressed(OnOffButton * button) = 0;
	};
private:
	Colour m_colour;
	std::string m_offLabel, m_onLabel;
	std::vector<Listener *> m_listeners;
	bool m_on;
public:
	OnOffButton() :
			EnhancedComponent(),
			m_on(false) {
	}
	virtual ~OnOffButton() {
	}

	virtual void paint(Graphics &);
	virtual void mouseDown(const MouseEvent &);

	/**
	 * Adds an object to listen to any clicks on
	 * this ImageButton.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}
	/**
	 * Sets the colour to render when the button is
	 * in its on state.
	 * @param colour The colour to render when on.
	 */
	void setColour(Colour colour) {
		m_colour = colour;
		repaint();
	}
	/**
	 * Sets the text to render on top of the button.
	 * @param label The text to render on top.
	 */
	void setLabel(std::string label) {
		m_offLabel = label;
		m_onLabel = label;
	}
	/**
	 * Sets the text to render on top of the button,
	 * but only when the button is in the on state.
	 * @param label The text to render when on.
	 */
	void setOnLabel(std::string label) {
		m_onLabel = label;
	}
	/**
	 * Sets the text to render on top of the button,
	 * but only when the button is in the off state.
	 * @param label The text to render when off.
	 */
	void setOffLabel(std::string label) {
		m_offLabel = label;
	}
	/**
	 * Sets what text should be rendered on top of
	 * the button when it is on and off.
	 * @param off The text to render when off.
	 * @param on The text to render when on.
	 */
	void setLabels(std::string off, std::string on) {
		m_offLabel = off;
		m_onLabel = on;
	}

	/**
	 * Gets the colour that is rendered when the
	 * button is on.
	 * @return The colour rendered when on.
	 */
	Colour getColour() {
		return m_colour;
	}
	/**
	 * Gets the text that is rendered when the
	 * button is on.
	 * @return The text rendered when on.
	 */
	std::string getOnLabel() {
		return m_onLabel;
	}
	/**
	 * Gets the text that is rendered when the
	 * button is off.
	 * @return The text rendered when off.
	 */
	std::string getOffLabel() {
		return m_offLabel;
	}

	/**
	 * Sets if the button is currently in the
	 * on state.
	 * @param on True if the button should be in the on state.
	 */
	void setOn(bool on = true) {
		m_on = on;
		repaint();
	}
	/**
	 * Gets which state the button is in.
	 * @return True if the button is on.
	 */
	bool getOn() {
		return m_on;
	}

	/**
	 * Saves the component's state to a SaveState.
	 * This state only contains whether or not
	 * the button is on, and does not contain
	 * information about labels or colours.
	 * @return A SaveState representing the component's current state.
	 */
	SaveState saveSaveState();
	/**
	 * Loads the component's state from a
	 * SaveState. See saveSaveState() for details
	 * about what data the SaveState contains.
	 * @param state The SaveState to load from.
	 */
	void loadSaveState(SaveState state);
};

/**
 * A text box that can be edited by the user.
 */
class TextEntry: public EnhancedComponent {
public:
	/**
	 * A class that can be informed of changes
	 * inside of the text box.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * This is called whenever a TextEntry is
		 * changed.
		 * @param textEntry The TextEntry that changed.
		 */
		virtual void textEntryChanged(TextEntry * textEntry) = 0;
	};
private:
	std::vector<Listener *> m_listeners;
	std::string m_text;
protected:
	/**
	 * Returns true if a character should be allowed
	 * to be typed.
	 * @param c The character to check.
	 * @return True if the character should be allowed.
	 */
	virtual bool canType(char c);
public:
	TextEntry();
	virtual ~TextEntry() {
	}

	virtual void paint(Graphics & g);
	virtual bool keyPressed(const KeyPress & key);
	/**
	 * Adds an object to listen to any changes on
	 * this TextEntry.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}
	/**
	 * Gets the text currently being displayed.
	 * @return The current text.
	 */
	std::string getText() {
		return m_text;
	}
	/**
	 * Sets the current text.
	 * @param text What the current text should be.
	 * @param notify True if listeners should be notified.
	 */
	void setText(std::string text, bool notify = true);
	virtual void mouseDown(const MouseEvent & e);
	virtual void focusLost(FocusChangeType cause) {
		repaint();
	}
};

/**
 * A specialized text entry for only
 * entering numbers.
 */
class NumberEntry: public TextEntry {
public:
	/**
	 * A class that can be informed of changes
	 * inside of the number entry.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * This is called whenever a NumberEntry is
		 * changed.
		 * @param textEntry The NumberEntry that changed.
		 */
		virtual void numberEntryChanged(NumberEntry * textEntry) = 0;
	};
private:
	std::vector<Listener *> m_listeners;
protected:
	virtual bool canType(char c);
public:
	NumberEntry() :
			TextEntry() {
	}
	virtual ~NumberEntry() {
	}

	/**
	 * Gets the current text as a
	 * number.
	 * @return
	 */
	double getNumber();
	/**
	 * Adds an object to listen to any changes on
	 * this NumberEntry.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}
	virtual bool keyPressed(const KeyPress & key);
};

/**
 * Just a rectangle.
 */
class Rectangle: public EnhancedComponent {
private:
	bool m_rounded;
	Colour m_colour;
public:
	Rectangle() :
			EnhancedComponent(),
			m_rounded(false),
			m_colour(MID_LAYER) {
	}
	virtual ~Rectangle() {
	}

	virtual void paint(Graphics & g);

	/**
	 * Sets whether or not the rectangle should be
	 * rendered with rounded corners.
	 * @param rounded True if rendering should use rounded corners.
	 */
	void setRounded(bool rounded) {
		m_rounded = rounded;
		repaint();
	}
	/**
	 * Sets what colour the rectangle should be.
	 * @param colour What colour the rectangle should be.
	 */
	void setColour(Colour colour) {
		m_colour = colour;
		repaint();
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_SIMPLEWIDGETS_H_ */
