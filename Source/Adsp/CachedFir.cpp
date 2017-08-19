/*
 * CachedFir.cpp
 *
 *  Created on: Aug 17, 2017
 *      Author: josh
 */

#include "CachedFir.h"

#include <cmath>

#include "Technical/Log.h"

namespace Adsp {

FirCoefficientCache::FirCoefficientCache() {
	for(int i = 0; i < TOTAL_SIZE; i += FILTER_KERNEL_SIZE) {
		m_storage[i] = -10.0;
	}
}

FirCoefficientCache::~FirCoefficientCache() {
}

int FirCoefficientCache::freqToIndex(double frequency) {
	return int((std::log2(frequency) - 4) * 100.0 + 0.5); //+0.5 is for rounding.
}

double FirCoefficientCache::indexToFreq(int index) {
	return std::pow(2.0, (index / 100.0) + 4.0);
}

double FirCoefficientCache::roundFrequency(double frequency) {
	return indexToFreq(freqToIndex(frequency));
}

bool FirCoefficientCache::hasCoefficientsFor(double frequency) {
	return m_storage[freqToIndex(frequency) * FILTER_KERNEL_SIZE] != -10.0;
}

double* FirCoefficientCache::getCoefficientsFor(double frequency) {
	return &m_storage[freqToIndex(frequency) * FILTER_KERNEL_SIZE];
}

void FirCoefficientCache::writeCoefficientsFor(double frequency, double* coefficients) {
	double* writeTo = &m_storage[freqToIndex(frequency) * FILTER_KERNEL_SIZE];
	for(int i = 0; i < FILTER_KERNEL_SIZE; i++) {
		*writeTo = *coefficients;
		writeTo++;
		coefficients++;
	}
}

FirFilterCache* FirFilterCache::s_instance = nullptr;

FirFilterCache::FirFilterCache() {
	createHammingWindow(FILTER_KERNEL_SIZE, &m_windows[0]);
	createBlackmanWindow(FILTER_KERNEL_SIZE, &m_windows[FILTER_KERNEL_SIZE]);
}

FirFilterCache* FirFilterCache::getInstance() {
	if(s_instance == nullptr) {
		s_instance = new FirFilterCache();
	}
	return s_instance;
}

FirFilterCache::~FirFilterCache() {

}

double* FirFilterCache::getCoefficientsFor(int filterType, double frequency, double sampleRate) {
	if(m_caches.count(filterType) == 0) {
		m_caches[filterType] = new FirCoefficientCache();
		info("Created a new FIR coefficient cache");
	}
	FirCoefficientCache* cache = m_caches[filterType];
	double* coArray = cache->getCoefficientsFor(frequency);
	if(!cache->hasCoefficientsFor(frequency)) {
		frequency = FirCoefficientCache::roundFrequency(frequency);
		double* window = &m_windows[(filterType >> 3) * FILTER_KERNEL_SIZE]; //The first three bits tell the filter type, the next bit (or more in the future) say the window type.
		int type = filterType & 0b111;
		double freqFrac = frequency / sampleRate;
		switch(type) {
		case FilterType::LOWPASS:
			createLowpassCoefficients(FILTER_KERNEL_SIZE, coArray, freqFrac, window);
			break;
		case FilterType::HIGHPASS:
			createHighpassCoefficients(FILTER_KERNEL_SIZE, coArray, freqFrac, window);
			break;
		}
	}
	return coArray;
}

void CachedFirFilter::compute(AtomSynth::AudioBuffer& input, AtomSynth::AudioBuffer& output) {
	double value;
	int ins, offset = input.getSize() - output.getSize();
	constexpr int N = FirFilterCache::FILTER_KERNEL_SIZE;
	assert(offset >= N);
	for(int c = 0; c < input.getChannels(); c++) {
		std::vector<double>::iterator inputIter = input.getData().begin() + c * input.getSize(),
				outputIter = output.getData().begin() + c * output.getSize();
		inputIter += offset - N - 1;
		for(int s = 0; s < output.getSize(); s++) {
			value = 0.0;
			ins = s + offset - N - 1;
			for(int i = 0; i < N; i++) {
				value += *(inputIter + i) * *(m_coPointer + i);
			}
			*outputIter = value;
			inputIter++;
			outputIter++;
		}
	}
}

double CachedFirFilter::compute(AtomSynth::AudioBuffer& input, int channel, int startSample) {
	double tr = 0.0;
	std::vector<double>::iterator inputIter = input.getData().begin() + channel * input.getSize() + startSample;
	constexpr int N = FirFilterCache::FILTER_KERNEL_SIZE;
	for(int i = 0; i < N; i++) {
		tr += *inputIter * *(m_coPointer + i);
		inputIter++;
	}
	return tr;
}

void CachedFirFilter::drawToConsole() {
	constexpr int HEIGHT = 64;
	double min = 0.0, max = 0.0;
	constexpr int N = FirFilterCache::FILTER_KERNEL_SIZE;
	for(int i = 0; i < N; i++) {
		double c = *(m_coPointer + i);
		if(c < min) {
			min = c;
		} else if(c > max) {
			max = c;
		}
	}
	double stepSize = (max - min) / HEIGHT;
	int minSteps = int(-min / stepSize + 1);
	min = minSteps * stepSize;
	int maxSteps = int(max / stepSize + 1);
	max = maxSteps * stepSize;
	int steps = minSteps + maxSteps + 1;
	std::vector<std::string> strings(steps, "");
	for(int i = 0; i < N; i++) {
		double c = *(m_coPointer + i);
		int step = int(c / stepSize + 0.5) + minSteps;
		for(int s = 0; s < steps; s++) {
			if(s == step) {
				if(step == minSteps) {
					if(c > 0.0001) {
						strings[s] += '^';
					} else if(c < 0.0001) {
						strings[s] += 'v';
					}
				} else {
					strings[s] += '*';
				}
			} else if(s == minSteps) {
				strings[s] += '-';
			} else {
				strings[s] += ' ';
			}
		}
	}
	for(int s = steps - 1; s >= 0; s--) {
		std::cout << strings[s] << std::endl;
	}
	std::cout << "Min: " << min << " Max: " << max << std::endl;
}

void CachedFirFilter::set(int filterType, double frequency, double sampleRate) {
	m_coPointer = FirFilterCache::getInstance()->getCoefficientsFor(filterType, frequency, sampleRate);
}

} /* namespace Adsp */
