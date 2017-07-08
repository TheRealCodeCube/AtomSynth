/*
 * SaveState.h
 *
 *  Created on: Oct 17, 2016
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_SAVESTATE_H_
#define SOURCE_TECHNICAL_SAVESTATE_H_

#include <string>
#include <vector>

namespace AtomSynth
{

class SaveState
{
private:
	std::vector<double> m_values;
	std::vector<std::string> m_strings;
	std::vector<SaveState> m_states;
	int m_nextStateCounter, m_nextValueCounter, m_nextStringCounter;
public:
	SaveState();
	SaveState(std::string from);
	virtual ~SaveState();
	void addValue(double value) { m_values.push_back(value); }
	void addString(std::string string) { m_strings.push_back(string); }
	void addState(SaveState state) { m_states.push_back(state); }
	double getValue(int index) { return (index > m_values.size()) ? 0.0 : m_values[index]; }
	std::string getString(int index) { return m_strings[index]; }
	SaveState & getState(int index) { return m_states[index]; }
	double getNextValue();
	std::string getNextString();
	SaveState & getNextState();
	std::vector<double> & getValues() { return m_values; }
	std::vector<std::string> & getStrings() { return m_strings; }
	std::vector<SaveState> & getStates() { return m_states; }
	std::string getString();
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_SAVESTATE_H_ */
