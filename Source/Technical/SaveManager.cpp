/*
 * SaveManager.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: josh
 */

#include "SaveManager.h"

#include <chrono>
#include <iostream>

#include "Technical/Log.h"
#include "Technical/SaveState.h"
#include "Technical/Synth.h"

namespace AtomSynth {

AutosaveThread::AutosaveThread(SaveManager* parent):
		Thread("autosaveWorker") {
	m_parent = parent;
}

AutosaveThread::~AutosaveThread() {

}

void AutosaveThread::run() {
	while(!threadShouldExit()) {
		if(m_parent->getTime() >= m_parent->m_nextAutosave) {
			m_parent->saveNow();
		}
		sleep(100);
	}
}

void SaveManager::setup() {
	m_autosaveThread.startThread();
	for(auto name : getAvailablePatches()) {
		std::cout << name << std::endl;
	}
}

unsigned int SaveManager::getTime() {
	//Yuck.
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

SaveManager::SaveManager():
		m_autosaveThread(this) {
	//Have the first autosave occur soon, so that I can debug things more easily.
	scheduleAutosave(5 * 1000);
}

SaveManager::~SaveManager() {

}

void SaveManager::setPatchName(std::string newName) {
	m_name = newName;
	m_autosaveRevision = -1; //Stop using numbered autosave names.
	scheduleAutosave(1000); //Wait to save to the new file name, in case the name is changed some more.
}

void SaveManager::saveNow() {
	scheduleAutosave(30 * 1000);

	SaveState toSave, extraData;
	toSave.addState(m_parent->getAtomManager().saveSaveState());

	toSave.addState(extraData);
	std::string name;
	if(m_autosaveRevision == -1) {
		name = m_name + ".ssf";
	} else {
		m_autosaveRevision++;
		if(m_autosaveRevision == MAX_AUTOSAVE_REVISIONS) {
			m_autosaveRevision = 0;
		}
		name = "autosave-" + std::to_string(m_autosaveRevision + 1) + ".ssf";
	}
	File toWrite = File::getCurrentWorkingDirectory().getChildFile("Patches").getChildFile(StringRef(name));
	toWrite.create();
	FileOutputStream *output = toWrite.createOutputStream();
	if(output != nullptr) {
		std::string data = toSave.exportString();
		output->setPosition(0); //Because otherwise it appends to the file.
		output->write(data.c_str(), data.size());
		output->flush();
		output->truncate();
		delete(output);
		info("Saved patch to " + toWrite.getFullPathName().toStdString());
		m_parent->getGuiManager().addMessage("Saved patch to " + toWrite.getFullPathName().toStdString());
		m_lastAutosave = getTime();
	}
}

bool SaveManager::load(File loadFrom) {
	scheduleAutosave(30 * 1000);

	info("Attempting to load patch " + loadFrom.getFullPathName().toStdString());
	FileInputStream *input = loadFrom.createInputStream();
	if(input == nullptr) return false;
	std::string result(input->getTotalLength(), '.');
	input->setPosition(0);
	input->read(&result[0], input->getTotalLength());
	std::cout << result << std::endl;
	SaveState state(result),
			atomData = state.getNextState(),
			extraData = state.getNextState();
	Synth::getInstance()->getAtomManager().loadSaveState(atomData);
	info("Loaded patch from: " + loadFrom.getFullPathName().toStdString());

	m_lastAutosave = getTime();
	m_name = loadFrom.getFileNameWithoutExtension().toStdString();
	m_autosaveRevision = -1;

	m_parent->getGuiManager().repaintRootComponent();
	m_parent->getGuiManager().addMessage("Loaded patch from " + loadFrom.getFullPathName().toStdString());
	return true;
}

std::vector<std::string> SaveManager::getAvailablePatches() {
	std::vector<std::string> tr;

	File directory = File::getCurrentWorkingDirectory().getChildFile("Patches");
	DirectoryIterator iter(directory, false);
	while(iter.next()) {
		auto file = iter.getFile();
		if((!file.isDirectory()) && (file.getFileExtension().equalsIgnoreCase(".ssf"))) {
			tr.push_back(file.getFileNameWithoutExtension().toStdString());
		}
	}

	std::sort(tr.begin(), tr.end());
	return tr;
}

} /* namespace AtomSynth */
