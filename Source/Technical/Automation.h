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

namespace AtomSynth
{

class Atom;
class SaveState;

enum class MixMode
{
	AVERAGE, MULTIPLY,
	MINIMUM, MAXIMUM,
};

struct AutomationInfluence
{
public:
	int m_inputIndex;
	double m_minRange, m_maxRange;
	AutomationInfluence();
};

class AutomatedControl
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() { };
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
	virtual void valueChanged() { }
public:
	AutomatedControl();
	virtual ~AutomatedControl() { };

	void setMin(double min) { m_min = min; }
	void setMax(double max) { m_max = max; }
	void setRange(double min, double max) { m_min = min; m_max = max; checkBounds(); }
	double getMin() { return m_min; }
	double getMax() { return m_max; }
	void setInt(bool isInt) { m_int = isInt; }
	bool isInt() { return m_int; }
	void setBounded(bool bounded) { m_bounded = bounded; }
	bool isBounded() { return m_bounded; }

	void setMixMode(MixMode mode) { m_mixMode = mode; }
	MixMode getMixMode() { return m_mixMode; }
	void setValue(double value);
	void offsetValue(double value);
	double getValue() { return (m_int) ? int(m_value) : m_value; }
	void setDisplayValue(double value);
	void clearDisplayValue();
	double getDisplayValue();
	std::string getFormattedValue();

	void setSilent(bool silent = true) { m_silent = silent; }
	void setSuffix(std::string suffix) { m_suffix = suffix; }
	std::string getSuffix() { return m_suffix; }
	void setDecimalPlaces(unsigned int places) { m_decimalPlaces = places; }
	bool isAnimated();

	AutomationInfluence & getInfluence(int index) { return m_influences[index]; }
	AudioBuffer & getResult() { return m_result; }
	void loadSaveState(SaveState state);
	SaveState saveSaveState();
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	static void calculateAutomation(std::vector<AutomatedControl *> & controls, Atom & source);

	friend class Atom;
};

class AutomationSet
{
private:
	std::vector<AutomatedControl *> m_controls;
	std::vector<std::vector<double>::iterator *> m_iterators;
	std::vector<int> m_incrementIndices, m_constantIndices;
public:
	AutomationSet() { }
	virtual ~AutomationSet() { }

	void clear() { m_controls.clear(); m_iterators.clear(); }
	void add(AutomatedControl & control, std::vector<double>::iterator & iterator);

	void resetPosition();
	void incrementPosition();
	void incrementChannel();

	void stopAutomationAnimation() { for(AutomatedControl * k : m_controls) k->clearDisplayValue(); }
	void calculateAutomation(Atom & source) { AutomatedControl::calculateAutomation(m_controls, source); }
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_AUTOMATION_H_ */
