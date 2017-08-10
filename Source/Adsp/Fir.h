/*
 * Fir.h
 *
 *  Created on: Aug 9, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_FIR_H_
#define SOURCE_ADSP_FIR_H_

#include <array>
#include <cassert>
#include "Technical/AudioBuffer.h"

namespace Adsp {

/**
 * Class which represents a FIR filter with
 * N coefficients.
 */
template<int N>
class FirFilter {
private:
	std::array<double, N> m_coefficients;
public:
	FirFilter() {

	}
	virtual ~FirFilter() {

	}

	/**
	 * Sets a particular coefficient. No bound
	 * checking is performed.
	 * @param index The index of the coefficient to set.
	 * @param value The value to set it to.
	 */
	void setCoefficient(int index, double value) {
		m_coefficients[index] = value;
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
	void compute(AtomSynth::AudioBuffer& input, AtomSynth::AudioBuffer& output)  {
		double value;
		int ins, offset = input.getSize() - output.getSize();
		assert(offset >= N);
		for(int c = 0; c < input.getChannels(); c++) {
			std::vector<double>::iterator inputIter = input.getData().begin() + c * input.getSize(),
					outputIter = output.getData().begin() + c * output.getSize();
			inputIter += offset - N - 1;
			for(int s = 0; s < output.getSize(); s++) {
				value = 0.0;
				ins = s + offset - N - 1;
				for(int i = 0; i < N; i++) {
					value += *(inputIter + i) * m_coefficients[i];
				}
				*outputIter = value;
				inputIter++;
				outputIter++;
			}
		}
	}

	/**
	 * Returns a pointer to the first coefficient.
	 * @return A pointer to the first coefficient.
	 */
	double* getCoefficients() {
		return &m_coefficients[0];
	}
};

/**
 * Creates a lowpass filter using N coefficients using
 * a sync function.
 * @param size The number of coefficients to generate.
 * @param destination Pointer to the first element of an array to write these coefficients to.
 * @param freqFrac Frequency as a fraction of the sample rate. (e.g. 0.1 @ 44.1kHz = 4.41kHz)
 */
void createLowpassCoefficients(int size, double* destination, double freqFrac);

} /* namespace AtomSynth */

#endif /* SOURCE_ADSP_FIR_H_ */
