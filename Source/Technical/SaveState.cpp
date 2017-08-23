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

uint32_t uintDifference(uint32_t a, uint32_t b) {
	if(a > b) {
		return a - b;
	} else {
		return b - a;
	}
}

constexpr double CONSTANTS[] = {
		0.0, 1.0, 0.5, 0.25, 0.75, 0.125, 0.325, 0.625,
		0.875, 2.0, 1.5, 1.25, 1.75, 4.0, 3.0, 3.5,
		4.5, 5.0, 1.125, 1.375, 1.625, 1.875, 3.25, 3.75,
		0.333333333333, 0.666666666666, 1.333333333333, 1.666666666666,
		0.166666666666, 0.833333333333, 1.166666666666, 1.833333333333,
};

std::string SaveState::doubleToBytes(double input) {
	//Designed to be compact, there are lots of places where precision is
	//sacrificed for output size. Note: this won't work for values with
	//pows outside the range 127 to -127. Uses one sign bit and some bits
	//for number, less efficient than two's compliment, but less of a
	//headache as well. There are actually 4 seperate  formats, the first
	//2 bits determine which format is used:
	//00: 4-byte float
	//01: 2-byte float
	//10: 2-byte int
	//11: 1-byte constant
	//The next bit is set if the number is negative. After that, the layout
	//is dependend on the format being used.
	uint32_t output = 0;
	//Set sign bit.
	if (input < 0.0) {
		output |= uint32_t(1) << 29; //Sets bit 2.
		input = -input;
	}

	//Check if the input matches a constant
	//If so, export 1-byte constant format (0b11)
	//bits 3-7 contain an index of a number in the CONSTANTS array. To
	//get the original number, get the value at that index from the
	//CONSTANTS array, and multiply it by -1 if the sign bit is set.
	for(uint32_t i = 0; i < 32; i++) {
		if(std::abs(input - CONSTANTS[i]) < 1.0e-6) { //1.0e-6 is arbitrary, it's just a cutoff for where the input should be considered close enough to the constant.
			output |= 0b11 << 30; //Sets bits 0 and 1 for the type.
			output |= (i & 0x1F) << 24; //Sets bits 3-7.
			std::string tr(1, '\0');
			//Returns the first byte in a string.
			tr[0] = (output >> 24) & 0xFF;
			return tr;
		}
	}

	//Checks if the input is roughly equal to the integer representation
	//of itself, and if that integer representation is less than 8192 (2^13).
	//If so, exports 2-byte int format, where bits 3-15 represent an integer
	//value from 0 to 8191. To get the original number, parse this section as
	//an int, and multiply it by -1 if the sign bit is set.
	if((int(input + 0.5) < 8192) && (std::abs(input - int(input + 0.5)) < 1.0e-6)) { //1.0e-6 is arbitrary, it's just a cutoff for where the input should be considered close enough to its integer counterpart.
		output |= 0b10 << 30; //Sets bits 0 and 1 for the type.
		output |= uint32_t(input + 0.5) << 16; //Sets bits 3-15 with the integer value.
		std::string tr(2, '\0');
		//Returns the first and second bytes in a string.
		tr[0] = (output >> 24) & 0xFF;
		tr[1] = (output >> 16) & 0xFF;
		return tr;
	}

	//Both floating point formats follow a similar pattern: X bits for
	//the decimal value, 1 bit for the sign of the power, and Y bits for
	//the power. Both formats use different numbers for X and Y.
	int power;
	double base = std::frexp(double(input), &power);
	uint32_t base21 = (uint32_t(base * double(0x200000)) & 0x1FFFFF); //21 bit representation of the base value.
	uint32_t base8  = (uint32_t(base * double(0x100)) & 0xFFFFFFFF); //8 bit representation of the base value.

	//Checks if the power can be stored in 4 bits. Also checks if the
	//8-bit base value is close enough to the 21-bit base value. If both
	//of these things are true, it will export in the 2-byte float format.
	//Otherwise, it will export in the 4-byte float format.
	if ((power < 16) && (power > -16) &&
			(uintDifference(base21, base8 << 13) < 0x3F)) { //0x3F is arbitrary. It's a cutoff for when the value should be considered close enough to its low-precision counterpart.
		//2 bits for type, 1 bit for sign, 8 bits for value, 5 bits for power.
		output |= 0b01 << 30; //Sets bits 0 and 1 for the type.
		//Sign is done previously.
		output |= base8 << 21; //Sets bits 3-10 with the base value.
		if(power < 0) {
			output |= 0b1 << 20; //Sets the sign bit (11) for the power.
			power = -power;
		}
		output |= (power & 0xF) << 16; //Sets bits 12-15 with the power.
		//Returns the first two bytes.
		std::string tr(2, '\0');
		tr[0] = (output >> 24) & 0xFF;
		tr[1] = (output >> 16) & 0xFF;
		return tr;
	} else {
		//2 bits for type, 1 bit for sign, 21 bits for value, 8 bits for power.
		//output |= 0b00 << 30;
		//Sign is done previously.
		output |= base21 << 8; //Sets bits 3-23 with the base value.
		if(power < 0) {
			output |= 0b1 << 7; //Sets the sign bit (24) for the power.
			power = -power;
		}
		output |= power & 0xF; //Sets bits 25-31 with the power.
		//Packs all four bytes into the return string.
		std::string tr(4, '\0');
		tr[0] = (output >> 24) & 0xFF;
		tr[1] = (output >> 16) & 0xFF;
		tr[2] = (output >> 8) & 0xFF;
		tr[3] = output & 0xFF;
		return tr;
	}
}

double SaveState::bytesToDouble(std::string input, int& bytesUsed) {
	uint32_t oinput = 0;
	oinput |= (unsigned char) ((unsigned int) input[0]) << 24;
	if(input.size() > 1) {
		oinput |= (unsigned char) ((unsigned int) input[1]) << 16;
	}
	if(input.size() > 3) {
		oinput |= (unsigned char) ((unsigned int) input[2]) << 8;
		oinput |= (unsigned char) ((unsigned int) input[3]);
	}

	uint32_t format = (oinput >> 30) & 0b11;
	double sign = (oinput & (0b1 << 29)) ? -1.0 : 1.0; //If the sign bit is set, the result should be negative.
	if(format == 0b00) { //4-byte float.
		bytesUsed = 4;
		double value = int((oinput >> 8) & 0x1FFFFF);
		value /= double(0x200000);
		int power = oinput & 0x7F;
		if(oinput & 0x80) power *= -1; //Sign for power.
		return sign * std::ldexp(value, power);
	} else if (format == 0b01) { //2-byte float.
		bytesUsed = 2;
		double value = (oinput >> 21) & 0xFF;
		value /= double(0x100);
		int power = (oinput >> 16) & 0xF;
		if(oinput & (0x10 << 16)) power *= -1; //Sign for power.
		return sign * std::ldexp(value, power);
	} else if (format == 0b10) { //2-byte int.
		bytesUsed = 2;
		return double((oinput >> 16) & 0x1FFF) * sign;
	} else if (format == 0b11) { //1-byte constant.
		bytesUsed = 1;
		uint32_t index = (oinput >> 24) & 0x1F;
		return CONSTANTS[index] * sign;
	}
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
	//1-4 bytes for each double
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
	int pos = 3, size;
	for(int i = 0; i < numValues; i++) {
		m_values.push_back(bytesToDouble(input.substr(pos, 4), size));
		pos += size;
	}
	for(int i = 0; i < numStrings; i++) {
	    int stringSize = (unsigned char) input[pos];
	    pos++;
	    m_strings.push_back(input.substr(pos, stringSize));
	    pos += stringSize;
	}
	for(int i = 0; i < numStates; i++) {
	    uint32_t stateSize = 0;
	    //Manual endianness.
	    stateSize += uint32_t((unsigned char) input[pos]) << 8;
	    stateSize += uint32_t((unsigned char) input[pos + 1]);
	    pos += 2;
	    m_states.push_back(SaveState());
	    //Because the constructor would check for the magic bytes to ensure it's in bytes format,
	    //but the magic bytes are only written to the top-level SaveState.
	    m_states[m_states.size() - 1].importBytes(input.substr(pos, stateSize));
	    pos += stateSize;
	}
}

//The index is a character code, the value is the value that digit holds. (E.G. 68 for D is 4.)
constexpr uint32_t REVERSE_DIGITS[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0-15
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //16-31
		0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 63, 0, 0, 63, //32-47
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 63, 0, 0, 0, 0, 0, //48-63
		0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, //64-79
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 62, //80-95
		0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //96-111
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 62, 0, //112-127
};
void SaveState::importBase64(std::string input) {
	std::string bytes = "";
	int groups = (input.length() + 3) / 4; //The number of 4-digit groups that will be turned into bytes.
	if(input.length() < groups * 4) { //Someone forgot to add padding (or copy-paste it.)
		input += "=="; //Just add some padding, it won't change the size of the output.
	}
	for(int i = 0, index = 0; i < groups; i++, index += 4) {
		uint32_t data = 0;
		data |= REVERSE_DIGITS[(unsigned char) input[index]] << 18;
		data |= REVERSE_DIGITS[(unsigned char) input[index + 1]] << 12;
		data |= REVERSE_DIGITS[(unsigned char) input[index + 2]] << 6;
		data |= REVERSE_DIGITS[(unsigned char) input[index + 3]];
		bytes += (unsigned char) (data >> 16) & 0xFF;
		bytes += (unsigned char) (data >> 8) & 0xFF;
		bytes += (unsigned char) data & 0xFF;
	}
	importBytes(bytes.substr(4)); //substr(4) is to strip the magic bytes (C0D3C1B3), since those get encoded into the base64 as well.
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
	} else if (from.substr(0, 4) == "\xC0\xD3\xC1\xB3") { //It's the new, fancy bytes format.
		importBytes(from.substr(4));
	} else if (from.substr(0, 4) == "wNPB") { //It's a base64 encoding of the new, fancy bytes format.
		importBase64(from);
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

constexpr char DIGITS[] =
		"ABCDEFGHIJKLMNOP"
		"QRSTUVWXYZabcdef"
		"ghijklmnopqrstuv"
		"wxyz0123456789_:"; //_ and : are used as the symbols for ease of copy-pasting.
//(double-clicking on word1-word2 results in one word being selected, whereas double-clicking
//on word1_word2 or word1:word2 (: only works in some cases) results in the whole thing being
//selected.)
std::string SaveState::exportBase64() {
	//Parses groups of 3 bytes into groups of 4 digits.
	std::string bytes = exportBytes(), tr = "";
	int groups = (bytes.length() + 2) / 3, //The number of 3-byte groups that will be processed.
	    index = 0;
	uint32_t data = 0;
	for(int i = 0; i < groups - 1; i++) {
		data = 0;
		data |= uint32_t((unsigned char) bytes[index]) << 16;
		data |= uint32_t((unsigned char) bytes[index + 1]) << 8;
		data |= uint32_t((unsigned char) bytes[index + 2]);
		tr += DIGITS[(data >> 18) & 0x3F];
		tr += DIGITS[(data >> 12) & 0x3F];
		tr += DIGITS[(data >> 6) & 0x3F];
		tr += DIGITS[data & 0x3F];
		index += 3;
	}
	//Check how many bytes are left to encode.
	int remaining = bytes.length() - index;
	//Set the final bytes of data, with bounds checking.
	data = 0;
	data |= uint32_t(bytes[index]) << 16;
	if(remaining > 1) data |= uint32_t(bytes[index + 1]) << 8;
	if(remaining > 2) data |= uint32_t(bytes[index + 2]);
	tr += DIGITS[(data >> 18) & 0x3F];
	tr += DIGITS[(data >> 12) & 0x3F];
	tr += DIGITS[(data >> 6) & 0x3F];
	tr += DIGITS[data & 0x3F];
	//Add padding = depending on how many bytes were encoded.
	if(remaining < 3) tr[tr.size() - 1] = '=';
	if(remaining < 2) tr[tr.size() - 2] = '=';
	return tr;
}

void SaveState::dumpToConsole(int extraIndentation) {
	std::string indentation(extraIndentation, '\t');
	std::cout << indentation << "========BEGIN SAVE STATE DUMP=========" << std::endl;
	std::cout << indentation << "Values:" << std::endl;
	for(double value : m_values) {
		std::cout << indentation << '\t' << value << std::endl;
	}
	std::cout << indentation << "Strings:" << std::endl;
	for(std::string string : m_strings) {
		std::cout << indentation << '\t' << string << std::endl;
	}
	std::cout << indentation << "States:" << std::endl;
	for(auto& state : m_states) {
		state.dumpToConsole(extraIndentation + 1);
	}
}

void SaveState::showOffNewFormat() {
	info("Old format: " +
			std::to_string(exportString().length()) +
			" bytes.\nNew raw format: " +
			std::to_string(exportBytes().length()) +
			" bytes.\nBase-64 encoded: " +
			std::to_string(exportBase64().length()) +
			" bytes.");
}

} /* namespace AtomSynth */
