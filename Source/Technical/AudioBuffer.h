/*
 * AudioBuffer.h
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_AUDIOBUFFER_H_
#define SOURCE_TECHNICAL_AUDIOBUFFER_H_

#include <vector>

namespace AtomSynth {

class AudioBuffer {
private:
	std::vector<double> m_data;
	bool m_constant;
	int m_size, m_channels;
	static int s_size, s_channels;
public:
	AudioBuffer();
	AudioBuffer(int samples);
	AudioBuffer(int channels, int samples);
	virtual ~AudioBuffer();

	void dumpToConsole();

	void copyData(AudioBuffer & copyFrom);
	void copyData(AudioBuffer & copyFrom, int sampleOffset, int channelOffset = 0);
	void offsetData(int offset);
	void fill(double value);

	std::vector<double> & getData() {
		return m_data;
	}
	double getValue(int channel, int sample) {
		return m_data[channel * m_size + sample];
	}
	void setValue(int channel, int sample, double value) {
		m_data[channel * m_size + sample] = value;
	}

	void setConstant(bool constant) {
		m_constant = constant;
	}
	bool isConstant() {
		return m_constant;
	}

	//These functions have no guarantee of preserving the data that used to be in the buffer.
	void setSize(int size);
	void setChannels(int channels);
	int getSize() {
		return m_size;
	}
	int getChannels() {
		return m_channels;
	}

	static void setDefaultSize(int size) {
		s_size = size;
	}
	static void setDefaultChannels(int channels) {
		s_channels = channels;
	}
	static int getDefaultSize() {
		return s_size;
	}
	static int getDefaultSamples() {
		return s_size;
	}
	static int getDefaultChannels() {
		return s_channels;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_AUDIOBUFFER_H_ */
