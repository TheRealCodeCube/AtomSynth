/*
 * SaveState.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: josh
 */

#include "Technical/SaveState.h"

#include <algorithm>
#include <iostream>

namespace AtomSynth {

SaveState::SaveState() :
		m_nextStateCounter(-1),
		m_nextValueCounter(-1),
		m_nextStringCounter(-1) {
	// TODO Auto-generated constructor stub
}

SaveState::SaveState(std::string from) :
		m_nextStateCounter(-1),
		m_nextValueCounter(-1),
		m_nextStringCounter(-1) {
	bool isNumber = true;
	double number = 0.0;
	int brackets = 0;
	std::string buffer = "";
	SaveState state;
	for (char & c : from) {
		switch (c) {
		case 's':
			if ((brackets == 1) && isNumber) {
				isNumber = false;
			} else {
				buffer += 's';
			}
			break;
		case ':':
			if (brackets == 1) {
				if (isNumber) {
					m_values.push_back(std::stod(buffer));
				} else {
					std::replace(buffer.begin(), buffer.end(), '\xb0', '[');
					std::replace(buffer.begin(), buffer.end(), '\xb1', ']');
					m_strings.push_back(buffer);
					isNumber = true;
				}
				buffer = "";
			} else {
				buffer += c;
			}
			break;
		case '[':
			if (brackets == 0) {
				buffer = "";
			} else {
				buffer += '[';
			}
			brackets++;
			break;
		case ']':
			buffer += ']';
			brackets--;
			if (brackets == 1) {
				m_states.push_back(SaveState(buffer));
				buffer = "";
			}
			break;
		default:
			buffer += c;
		}
	}
}

SaveState::~SaveState() {
	// TODO Auto-generated destructor stub
}

double SaveState::getNextValue() {
	m_nextValueCounter++;
	if (m_nextValueCounter > m_values.size()) {
		m_nextValueCounter = 0;
	}
	return m_values[m_nextValueCounter];
}

std::string SaveState::getNextString() {
	m_nextStringCounter++;
	if (m_nextStringCounter > m_strings.size()) {
		m_nextStringCounter = 0;
	}
	return m_strings[m_nextStringCounter];
}

SaveState & SaveState::getNextState() {
	m_nextStateCounter++;
	if (m_nextStateCounter > m_states.size()) {
		m_nextStateCounter = 0;
	}
	return m_states[m_nextStateCounter];
}

std::string SaveState::exportString() {
	std::string output = "[";
	for (double & value : m_values) {
		output += std::to_string(value);
		output += ":";
	}

	for (std::string & string : m_strings) {
		std::replace(string.begin(), string.end(), '[', '\xb0');
		std::replace(string.begin(), string.end(), ']', '\xb1');
		output += 's' + string;
		output += ":";
	}

	for (SaveState & state : m_states) {
		output += state.exportString();
	}
	output += ']';
	return output;
}

} /* namespace AtomSynth */
