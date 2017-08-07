/*
 * AudioBuffer.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#include "AudioBuffer.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace AtomSynth {

int AudioBuffer::s_size = 256, AudioBuffer::s_channels = 2;

AudioBuffer::AudioBuffer() :
		m_constant(false),
		m_size(s_size),
		m_channels(s_channels) {
	m_data.resize(m_channels * m_size);
}

AudioBuffer::AudioBuffer(int samples) :
		m_constant(false),
		m_size(samples),
		m_channels(s_channels) {
	m_data.resize(m_channels * m_size);
}

AudioBuffer::AudioBuffer(int channels, int samples) :
		m_constant(false),
		m_size(samples),
		m_channels(channels) {
	m_data.resize(m_channels * m_size);
}

AudioBuffer::~AudioBuffer() {
	// TODO Auto-generated destructor stub
}

void AudioBuffer::dumpToConsole() {
	std::cout << std::endl;
	for (int c = 0; c < m_channels; c++) {
		std::cout << "C" << c << "\t";
	}
	std::cout << std::endl;
	for (int s = 0; s < m_size; s++) {
		for (int c = 0; c < m_channels; c++) {
			std::cout << getValue(c, s) << "\t";
		}
		std::cout << std::endl;
	}
}

void AtomSynth::AudioBuffer::copyData(AudioBuffer & copyFrom) {
	int channels = std::min(copyFrom.getChannels(), m_channels);
	int samples = std::min(copyFrom.getSize(), m_size);
	for (int c = 0; c < channels; c++) {
		for (int s = 0; s < samples; s++) {
			m_data[c * m_size + s] = copyFrom.getValue(c, s);
		}
	}
}

void AudioBuffer::copyData(AudioBuffer & copyFrom, int sampleOffset, int channelOffset) {
	int channels = std::min(copyFrom.getChannels(), m_channels - channelOffset);
	int samples = std::min(copyFrom.getSize(), m_size - sampleOffset);
	for (int c = 0, cd = channelOffset; c < channels; c++, cd++) {
		for (int s = 0, sd = sampleOffset; s < samples; s++, sd++) {
			m_data[cd * m_size + sd] = copyFrom.getValue(c, s);
		}
	}
}

void AudioBuffer::offsetData(int offset) {
	if (offset < 0) {
		offset *= -1;
		for (int c = 0; c < m_channels; c++) {
			for (int s = 0; s < m_size - offset; s++) {
				m_data[c * m_size + s] = m_data[c * m_size + s + offset];
			}
			std::fill(m_data.begin() + m_size - offset + c * m_size, m_data.begin() + (c + 1) * m_size, 0.0);
		}
	} else if (offset > 0) {
		for (int c = 0; c < m_channels; c++) {
			for (int s = m_size - 1; s >= offset; s--) {
				m_data[c * m_size + s] = m_data[c * m_size + s - offset];
			}
			std::fill(m_data.begin() + c * m_size, m_data.begin() + offset - 1 + c * m_size, 0.0);
		}
	}
}

void AudioBuffer::fill(double value) {
	if (m_constant) {
		for (int c = 0; c < s_channels; c++) {
			m_data[s_size * c] = value;
		}
	} else {
		std::fill(m_data.begin(), m_data.end(), value);
	}
}

void AudioBuffer::setSize(int size) {
	m_size = size;
	m_data.resize(m_size * m_channels, 0.0);
}

void AudioBuffer::setChannels(int channels) {
	m_channels = channels;
	m_data.resize(m_size * m_channels, 0.0);
}

} /* namespace AtomSynth */
