/*
 * LogManager.cpp
 *
 *  Created on: Aug 15, 2017
 *      Author: josh
 */

#include "LogManager.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Technical/AudioBuffer.h"
#include "Technical/Log.h"
#include "Technical/Synth.h"

namespace AtomSynth {

std::string LogManager::createIndentation() {
	std::string tr = "";
	for(int i = 0; i < m_tabIndent; i++) {
		tr += "\t";
	}
	return tr;
}

void LogManager::setup() {

}

LogManager::LogManager() {

}

LogManager::~LogManager() {

}

void LogManager::addLabel(std::string label) {
	m_content += createIndentation() + label + '\n';
	m_tabIndent++;
}

void LogManager::addNewline() {
	m_content += '\n';
}

void LogManager::endLabel() {
	m_tabIndent--;
}

void LogManager::writeString(std::string toWrite) {
	m_content += createIndentation() + toWrite + '\n';
}

void LogManager::writeInt(int toWrite) {
	m_content += createIndentation() + std::to_string(toWrite) + '\n';
}

void LogManager::writeAudioBuffer(AudioBuffer& toWrite) {
	int samples = toWrite.getSize(),
			rows = (toWrite.getSize() + 63) / 64; //+63 is for round-up division.
	std::stringstream output;
	output << std::setprecision(3);
	for(int c = 0; c < toWrite.getChannels(); c++) {
		addLabel("Channel " + std::to_string(c));
		for(int r = 0; r < rows; r++) {
			output << createIndentation();
			for(int s = 0; (s < 64) && (r * 64 + s < samples); s++) {
				output << toWrite.getValue(c, r * 64 + s) << '\t';
			}
			output << '\n';
		}
		m_content += output.str();
		output.str("");
		endLabel();
	}
}

void LogManager::dumpToConsole() {
	std::cout << m_content << std::endl;
	m_content.clear();
}

void LogManager::dumpToFile() {
	File toWrite = File::getCurrentWorkingDirectory().getChildFile("debugLog.log");
	toWrite.create();
	FileOutputStream *output = toWrite.createOutputStream();
	if(output != nullptr) {
		output->setPosition(0); //Because otherwise it appends to the file.
		output->write(m_content.c_str(), m_content.size());
		output->flush();
		output->truncate();
		delete(output);
		info("Dumped synth state to log file.");
		m_parent->getGuiManager().addMessage("Saved log to " + toWrite.getFullPathName().toStdString());
		m_content.clear();
	}
}

} /* namespace AtomSynth */
