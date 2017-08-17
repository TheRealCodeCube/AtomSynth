/*
 * CachedFir.h
 *
 *  Created on: Aug 17, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_CACHEDFIR_H_
#define SOURCE_ADSP_CACHEDFIR_H_

#include <array>
#include <map>

#include "Fir.h"

namespace Adsp {

namespace FilterType {
	static constexpr int LOWPASS = 0b0000,
			HIGHPASS = 0b0001,
			HAMMING = 0b0000,
			BLACKMAN = 0b1000;
}

class FirCoefficientCache {
public:
	static constexpr int FILTER_KERNEL_SIZE = 512,
			NUM_FILTERS = 1029, //The x-coordinate at which 2^{\frac{x}{100}+4} reaches 20k.
			TOTAL_SIZE = FILTER_KERNEL_SIZE * NUM_FILTERS;
private:
	std::array<double, TOTAL_SIZE> m_storage;
	static int freqToIndex(double frequency);
	static double indexToFreq(int index);
public:
	FirCoefficientCache();
	~FirCoefficientCache();

	static double roundFrequency(double frequency);

	bool hasCoefficientsFor(double frequency);
	double* getCoefficientsFor(double frequency);
	void writeCoefficientsFor(double frequency, double* coefficients);
};

class FirFilterCache {
public:
	static constexpr int FILTER_COMBINATIONS = 16, //8 possible filter types, 2 possible window types.
			WINDOW_TYPES = 2,
			FILTER_KERNEL_SIZE = FirCoefficientCache::FILTER_KERNEL_SIZE;
private:
	std::array<double, FILTER_KERNEL_SIZE * WINDOW_TYPES> m_windows;
	std::map<int, FirCoefficientCache*> m_caches;
	static FirFilterCache* s_instance;
	FirFilterCache();
public:
	static FirFilterCache* getInstance();
	~FirFilterCache();

	double* getCoefficientsFor(int filterType, double frequency, double sampleRate);
};

/**
 * Class which represents a FIR filter whose
 * coefficients are sourced from a cache.
 */
class CachedFirFilter {
private:
	double* m_coPointer = nullptr;
public:
	static constexpr int SIZE = FirFilterCache::FILTER_KERNEL_SIZE;
	CachedFirFilter() {

	}
	virtual ~CachedFirFilter() {

	}

	/**
	 * Filters the signal in input and writes the
	 * result to output. Output's size should be at
	 * least input's size plus the size of the filter,
	 * since the filter will only calculate the last
	 * [output.getSize()] samples of the input, but
	 * needs extra samples because of how FIR filters
	 * work.
	 * @param input The signal to filter.
	 * @param output Where to write the resulting signal to.
	 */
	void compute(AtomSynth::AudioBuffer& input, AtomSynth::AudioBuffer& output);

	/**
	 * Filters a section of signal from an audio
	 * buffer to produce a single sample of output.
	 * The section of audio filtered will start at
	 * startSample and end at startSample + size of
	 * this filter.
	 * @param input The signal to filter.
	 * @param channel The channel to filter from.
	 * @param startSample The sample to start filtering the signal from.
	 * @return A filtered sample, computed by filtering the selected section of signal.
	 */
	double compute(AtomSynth::AudioBuffer& input, int channel, int startSample);

	/**
	 * Draws a graphical representation of the filter
	 * coefficients to the console.
	 */
	void drawToConsole();

	/**
	 * Sets the coefficients of the filter. If the
	 * same coefficients were previously requested,
	 * they will be loaded from the cache instead of
	 * recalculated.
	 * @param filterType A bitwise-or combination of window type and filter type from FilterType.
	 * @param frequency The cutoff frequency.
	 * @param sampleRate The sample rate (used when the coefficients need to be recomputed.
	 */
	void set(int filterType, double frequency, double sampleRate);
};

} /* namespace Adsp */

#endif /* SOURCE_ADSP_CACHEDFIR_H_ */
