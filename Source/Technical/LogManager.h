/*
 * LogManager.h
 *
 *  Created on: Aug 15, 2017
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_LOGMANAGER_H_
#define SOURCE_TECHNICAL_LOGMANAGER_H_

#include <string>
#include <vector>

namespace AtomSynth {

class AudioBuffer;
class Synth;

class LogManager {
private:
	bool m_debugEverything = false;
	int m_tabIndent = 0;
	std::string m_content = "";
	std::string createIndentation();
	Synth * m_parent = nullptr;
	void setup();

	friend class Synth;
public:
	LogManager();
	virtual ~LogManager();

	void addLabel(std::string label);
	void addNewline();
	void endLabel();

	void writeString(std::string toWrite);
	void writeInt(int toWrite);
	void writeAudioBuffer(AudioBuffer& toWrite);

	void clear() {
		m_content.clear();
	}
	void dumpToConsole();
	void dumpToFile();

	bool shouldDebugEverything() {
		return m_debugEverything;
	}

	void setDebugEverything() {
		m_debugEverything = true;
	}

	void clearDebugEverything() {
		m_debugEverything = false;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_LOGMANAGER_H_ */
