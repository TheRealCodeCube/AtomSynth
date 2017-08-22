/*
 * SaveState.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: josh
 */

#include "SaveState.h"

#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "Log.h"

namespace AtomSynth {

std::string SaveState::doubleToBytes(double input) {
	//Note: this won't work for values with pows outside the range 127 to -127.
	//Uses one sign bit and 7 or 23 bits of number, less efficient than two's compliment, but less of a headache as well.
	if (input == 0.0) return std::string(4, '\x00'); //Because "\x00\x00..." creates an empty string.
	uint32_t output = 0;
	int power;
	double base = std::frexp(double(input), &power);
	//Set bits
	if (base < 0.0) {
		output |= uint32_t(1) << 31; //Sets bit 0.
		base = -base;
	}

	output |= (uint32_t(base * double(0x7FFFFF)) & 0x7FFFFF) << 8; //Set bits 1 - 23

	if(power < 0) {
	    output |= uint32_t(1) << 7; //Sets bit 24.
	    power = -power;
	}
	output |= power & 0x7F; //Set the last 7 bits.

	std::string tr(4, '\0');
	tr[0] = (output >> 24) & 0xFF;
	tr[1] = (output >> 16) & 0xFF;
	tr[2] = (output >> 8) & 0xFF;
	tr[3] = output & 0xFF;
	return tr;
}

double SaveState::bytesToDouble(std::string input) {
	uint32_t oinput = 0;
	oinput |= (unsigned char) ((unsigned int) input[0]) << 24;
	oinput |= (unsigned char) ((unsigned int) input[1]) << 16;
	oinput |= (unsigned char) ((unsigned int) input[2]) << 8;
	oinput |= (unsigned char) ((unsigned int) input[3]);

	double value = int((oinput >> 8) & 0x7FFFFF);
	value /= double(0x7FFFFF);
	int power = oinput & 0x7F;
	if(oinput & 0x80) power *= -1; //Sign for power.
	value = std::ldexp(value, power);
	if(oinput & 0x80000000) value *= -1.0; //Sign for value.
	return value;
}

void SaveState::importString(std::string input) {
	bool isNumber = true;
	double number = 0.0;
	int brackets = 0;
	std::string buffer = "";
	SaveState state;
	for (char & c : input) {
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

//Format:
//1 byte for num of doubles
//1 byte for num of strings
//1 byte for num of savestates
//for each double:
	//4 bytes for each double
//for each string:
	//1 byte for num of chars
		//1 byte for each char
//for each savestate:
	//2 bytes for size (in bytes) of savestate
	//that many bytes of content to be importBytes()'sed
std::string SaveState::exportBytesBackend() {
	std::string tr = "";
	tr += (unsigned char) m_values.size();
	tr += (unsigned char) m_strings.size();
	tr += (unsigned char) m_states.size();
	for(auto& value : m_values) {
		tr += doubleToBytes(value);
	}
	for(auto& string: m_strings) {
	    tr += (unsigned char) string.length();
	    tr += string;
	}
	for(auto& state: m_states) {
	    std::string bytes = state.exportBytesBackend();
	    tr += (unsigned char) (bytes.length() >> 8) & 0xFF;
	    tr += (unsigned char) bytes.length() & 0xFF;
	    tr += bytes;
	}
	return tr;
}

void SaveState::importBytes(std::string input) {
	int numValues = (unsigned char) input[0];
	int numStrings = (unsigned char) input[1];
	int numStates = (unsigned char) input[2];
	int pos = 3;
	for(int i = 0; i < numValues; i++) {
		m_values.push_back(bytesToDouble(input.substr(pos, 4)));
		pos += 4;
	}
	for(int i = 0; i < numStrings; i++) {
	    int stringSize = (unsigned char) input[pos];
	    pos++;
	    m_strings.push_back(input.substr(pos, stringSize));
	    pos += stringSize;
	}
	for(int i = 0; i < numStates; i++) {
	    unsigned int stateSize = 0;
	    stateSize |= ((unsigned int) input[pos]) << 8;
	    stateSize |= (unsigned int) input[pos + 1];
	    pos += 2;
	    m_states.push_back(SaveState());
	    //Because the constructor would check for the magic bytes to ensure it's in bytes format,
	    //but the magic bytes are only written to the top-level SaveState.
	    m_states[m_states.size() - 1].importBytes(input.substr(pos, stateSize));
	    pos += stateSize;
	}
}

SaveState::SaveState() :
		m_nextStateCounter(-1),
		m_nextValueCounter(-1),
		m_nextStringCounter(-1) {

}

SaveState::SaveState(std::string from) :
		m_nextStateCounter(-1),
		m_nextValueCounter(-1),
		m_nextStringCounter(-1) {
	if (from[0] == '[') { //It's the old, yucky string format.
		importString(from);
	} else if (from.substr(0, 4) == "\xC0\xD3\xC1\xB3") { //It's
		importBytes(from.substr(4));
	}
}

SaveState::~SaveState() {

}

double SaveState::getNextValue() {
	m_nextValueCounter++;
	if (m_nextValueCounter == m_values.size()) {
		m_nextValueCounter = 0;
	}
	return m_values[m_nextValueCounter];
}

std::string SaveState::getNextString() {
	m_nextStringCounter++;
	if (m_nextStringCounter == m_strings.size()) {
		m_nextStringCounter = 0;
	}
	return m_strings[m_nextStringCounter];
}

SaveState & SaveState::getNextState() {
	m_nextStateCounter++;
	if (m_nextStateCounter == m_states.size()) {
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

std::string SaveState::exportBytes() {
	//Magic bytes for identifying that exportBytes() was used.
	return "\xC0\xD3\xC1\xB3" + exportBytesBackend();
}

void SaveState::showOffNewFormat() {
	info("Saving this state with the old format would take " +
			std::to_string(exportString().length()) +
			" bytes, but with the new format, it only takes " +
			std::to_string(exportBytes().length()) + "!");
}

} /* namespace AtomSynth */
