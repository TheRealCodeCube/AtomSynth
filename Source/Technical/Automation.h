/*
 * Automation.h
 *
 *  Created on: May 29, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_AUTOMATION_H_
#define SOURCE_TECHNICAL_AUTOMATION_H_

#include <vector>

#include "AudioBuffer.h"
#include "JuceLibraryCode/JuceHeader.h"

namespace AtomSynth {

class Atom;
class SaveState;

enum class MixMode {
	AVERAGE, MULTIPLY, MINIMUM, MAXIMUM,
};

/**
 * Describes how an input should affect the value
 * of an automated control. m_minRange and
 * m_maxRange are both values ranging from 0 to
 * 1. 0 represents the lowest value the AutomatedControl
 * can achieve, and 1 represents the highest. As
 * the specified automation input approaches -1, the
 * value of the AutomatedControl should approach
 * m_minRange. Likewise, as the specified automation
 * input approaches 1, the value of the AutomatedControl
 * should approach m_maxRange.
 */
struct AutomationInfluence {
public:
	int m_inputIndex; ///< Which automation input this AutomationInfluence is referring to.
	double m_minRange, ///< A percentage of an AutomatedControl's range, expressed in a value from 0 to 1.
		m_maxRange; ///< A percentage of an AutomatedControl's range, expressed in a value from 0 to 1.
	AutomationInfluence();
};

/**
 * Represents a value that can be changed both
 * by the user and by automation.
 */
class AutomatedControl {
public:
	/**
	 * Extend this to be informed of changes by
	 * the user and by automation of an
	 * AutomatedControl.
	 */
	class JUCE_API Listener {
	public:
		virtual ~Listener() {
		}
		/**
		 * Called whenever an AutomatedControl is changed
		 * by a user or by automation.
		 * @param control The AutomatedControl that was changed.
		 * @param byUser True if the change was triggered by a user.
		 */
		virtual void automatedControlChanged(AutomatedControl * control, bool byUser) = 0;
	};
private:
	double m_min, m_max, m_value, m_display;
	MixMode m_mixMode;
	AutomationInfluence m_influences[4];
	AudioBuffer m_result;
	std::string m_suffix;
	unsigned int m_decimalPlaces;
	bool m_bounded, m_int, m_displaying, m_silent;
	std::vector<Listener *> m_listeners;
	void checkBounds();
	void informListeners(bool byUser);
protected:
	double snap(double value, int snaps);
public:
	AutomatedControl();
	virtual ~AutomatedControl() {
	}

	/**
	 * Called whenever the value has changed.
	 */
	virtual void valueChanged() {
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
		checkBounds();
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
	 * integer. This does not affect values generated
	 * through automation.
	 * @param isInt True if user input should be rounded to the nearest integer.
	 */
	void setInt(bool isInt) {
		m_int = isInt;
	}
	/**
	 * Gets if user input should be rounded to the nearest
	 * integer. This does not affect values generated
	 * through automation.
	 * @return True if user input should be rounded to the nearest integer.
	 */
	bool isInt() {
		return m_int;
	}
	/**
	 * Sets if the input should be bounded. If the input
	 * is not bounded, then the control will be allowed
	 * to go past getMin() and getMax(). However, these
	 * values should still be used when rendering a
	 * visual representation. Also, if the input is not
	 * bounded, then the range of automation will be
	 * determined by the value the user has set the control
	 * to. (I.E. the automation range will be +-getValue().)
	 * @param bounded True if input should be restricted by getMin() and getMax().
	 */
	void setBounded(bool bounded) {
		m_bounded = bounded;
	}
	/**
	 * Gets if the input should be bounded. If the input
	 * is not bounded, then the control will be allowed
	 * to go past getMin() and getMax(). However, these
	 * values should still be used when rendering a
	 * visual representation. Also, if the input is not
	 * bounded, then the range of automation will be
	 * determined by the value the user has set the control
	 * to. (I.E. the automation range will be +-getValue().)
	 * @return True if input is restricted by getMin() and getMax().
	 */
	bool isBounded() {
		return m_bounded;
	}

	/**
	 * Sets the method by which automation influences are
	 * mixed. This controls how the final value of the
	 * control is calculated if there is more than one
	 * influence active.
	 * @param mode A MixMode specifying which method of mixing should be used.
	 */
	void setMixMode(MixMode mode) {
		m_mixMode = mode;
	}
	/**
	 * Gets the method by which automation influences are
	 * mixed. This controls how the final value of the
	 * control is calculated if there is more than one
	 * influence active.
	 * @return A MixMode specifying which method of mixing is used.
	 */
	MixMode getMixMode() {
		return m_mixMode;
	}
	/**
	 * Sets the default value. This is the same value
	 * that the user can specify. This value is used
	 * when there is no automation active. It is also
	 * used as the automation range when isBounded() is
	 * false.
	 * @param value A double specifying what to set the value to.
	 */
	void setValue(double value);
	/**
	 * Offsets the default value. See setValue() for
	 * more details about the default value.
	 * @param value An offset to add to the default value.
	 */
	void offsetValue(double value);
	/**
	 * Gets the default value. See setValue() for
	 * more details about the default value.
	 * @return The default value.
	 */
	double getValue() {
		return (m_int) ? int(m_value) : m_value;
	}
	/**
	 * Sets the display value. This is only used to
	 * show what automation has been calculated. It
	 * cannot be set by the user. Whenever a display
	 * value is present, it should be rendered
	 * instead of the default value. See setValue()
	 * for more details about the default value.
	 * @param value The value to display instead of the default value.
	 */
	void setDisplayValue(double value);
	/**
	 * Stops displaying the display value. This will
	 * clear the display value, and any visual
	 * representation of the control should revert
	 * to showing the default value. See setDisplayValue()
	 * for more details about the display value, and
	 * setValue() for more details about the default
	 * value.
	 */
	void clearDisplayValue();
	/**
	 * Gets the value that should be displayed in
	 * any visual representation of this control.
	 * If a display value has been set (see
	 * setDisplayValue() for more details), then
	 * this method will return the display value.
	 * Otherwise, this method is equivalent to
	 * getValue().
	 * @return The value to be displayed.
	 */
	double getDisplayValue();
	/**
	 * Returns a formatted versoin of getDisplayValue().
	 * This is recommended for any graphical
	 * representation of the control. It handles
	 * appending the suffix (see setSuffix() for
	 * more details) as well as displaying the
	 * proper number of decimal points.
	 * @return A formatted string of getDisplayValue().
	 */
	std::string getFormattedValue();

	/**
	 * Sets if the AutomatedControl should silence
	 * all events. If it is true, no events will
	 * be generated.
	 * @param silent True if no events should be generated.
	 */
	void setSilent(bool silent = true) {
		m_silent = silent;
	}
	/**
	 * Sets the suffix to be added after the number.
	 * In any graphical representation of this control,
	 * the suffix should appear after any textual
	 * presentation of the number. This can be used
	 * to show units (E.G. Hz.)
	 * @param suffix The suffix to show after the number.
	 */
	void setSuffix(std::string suffix) {
		m_suffix = suffix;
	}
	/**
	 * Gets the suffix, see setSuffix() for more details.
	 * @return The suffix, see setSuffix() for more details.
	 */
	std::string getSuffix() {
		return m_suffix;
	}
	/**
	 * Set the number of decimal places that should be
	 * rendered. This is used by getFormattedValue()
	 * to stringify the control's value with the
	 * appropriate number of decimal places.
	 * @param places Number of decimal places to render.
	 */
	void setDecimalPlaces(unsigned int places) {
		m_decimalPlaces = places;
	}
	/**
	 * Returns true if there are any active automation
	 * influences.
	 * @return True if there are any active automation influences.
	 */
	bool isAutomated();

	/**
	 * Gets a particular automation influence.
	 * @param index The index of the influence (0 to 3.)
	 * @return A reference to the AutomationInfluence at that index.
	 */
	AutomationInfluence & getInfluence(int index) {
		return m_influences[index];
	}
	/**
	 * Gets the result audio buffer. Whenever this knob is
	 * calculated via calculateAutomation(), this buffer is
	 * filled with the proper values for the knob. This could
	 * be all the same value, probably due to there being no
	 * automation, or it could change rapidly from the automation.
	 * Note that only a few samples may be populated, see the
	 * description of constant audio buffers in the AudioBuffer
	 * documentation for why.
	 * @return
	 */
	AudioBuffer & getResult() {
		return m_result;
	}
	/**
	 * Loads the control's state from a SaveState. See
	 * saveSaveState() for what information the SaveState
	 * contains.
	 * @param state The SaveState to load from.
	 */
	void loadSaveState(SaveState state);
	/**
	 * Saves the control's current state to a SaveState.
	 * The returned SaveState contains information about
	 * the default value and all four automation influence
	 * channels. It also contains information about the
	 * minimum and maximum possible values, as well as the
	 * suffix, so that knobs that are changed by code can
	 * be restored without having to execute that code
	 * again.
	 * @return A SaveState representing the control's current state.
	 */
	SaveState saveSaveState();
	/**
	 * Adds an object which will be informed of changes to
	 * the control. It will be informed of changes to the
	 * default value and the display value. It will not be
	 * informed if isSilent() is false.
	 * @param listener The Listener object to add.
	 */
	void addListener(Listener * listener) {
		m_listeners.push_back(listener);
	}

	/**
	 * Computes automation for a series of AutomatedControl
	 * objects, taking into consideration an Atom to source
	 * the automation inputs from. Also see getResult()
	 * @param controls A vector of AutomatedControl objects to calculate.
	 * @param source The Atom to source the automation inputs from.
	 */
	static void calculateAutomation(std::vector<AutomatedControl *> & controls, Atom & source);

	friend class Atom;
};

/**
 * Keeps track of iterators of getResult() buffers
 * from AutomatedControl objects. See AudioBuffer
 * for why this is necessary.
 */
class AutomationSet {
private:
	std::vector<AutomatedControl *> m_controls;
	std::vector<std::vector<double>::iterator *> m_iterators;
	std::vector<int> m_incrementIndices, m_constantIndices;
public:
	AutomationSet() {
	}
	virtual ~AutomationSet() {
	}

	/**
	 * Clears all AutomatedControl objects added to this.
	 */
	void clear() {
		m_controls.clear();
		m_iterators.clear();
	}
	/**
	 * Adds an AutomatedControl to the automation set. The
	 * provided iterator is linked to the output of the
	 * provided AutomatedControl. This only needs to be
	 * called once. After that, the iterator will always
	 * point to the output of the AutomatedControl.
	 * @param control The AutomatedControl to add.
	 * @param iterator The iterator to link to the AutomatedControl.
	 */
	void add(AutomatedControl & control, std::vector<double>::iterator & iterator);

	/**
	 * Resets all iterators to their starting positions.
	 */
	void resetPosition();
	/**
	 * Increments all iterators to move to the next sample
	 * of data in the outputs of their respective controls.
	 * See AudioBuffer for why you should use this instead
	 * of ++.
	 */
	void incrementPosition();
	/**
	 * Increments all iterators to move to the next channel
	 * of data. Call this in addition to incrementPosition()
	 * to successfully move all iterators to the next
	 * channel. See AudioBuffer for why you should use this
	 * instead of ++.
	 */
	void incrementChannel();

	/**
	 * Clears the display values of all controls added to
	 * this set.
	 */
	void stopAutomationAnimation() {
		for (AutomatedControl * k : m_controls)
			k->clearDisplayValue();
	}
	/**
	 * Calculates automation for all controls in this set,
	 * given an Atom to source automation inputs from.
	 * @param source The Atom to source automation inputs from.
	 */
	void calculateAutomation(Atom & source) {
		AutomatedControl::calculateAutomation(m_controls, source);
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_AUTOMATION_H_ */
