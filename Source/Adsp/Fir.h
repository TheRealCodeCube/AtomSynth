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
#include <iostream>
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
	double compute(AtomSynth::AudioBuffer& input, int channel, int startSample) {
		double tr = 0.0;
		std::vector<double>::iterator inputIter = input.getData().begin() + channel * input.getSize() + startSample;
		for(int i = 0; i < N; i++) {
			tr += *inputIter * m_coefficients[i];
			inputIter++;
		}
		return tr;
	}

	/**
	 * Draws a graphical representation of the filter
	 * coefficients to the console.
	 */
	void drawToConsole() {
		constexpr int HEIGHT = 64;
		double min = 0.0, max = 0.0;
		for(double c : m_coefficients) {
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
		for(double c : m_coefficients) {
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

	/**
	 * Returns a pointer to the first coefficient.
	 * @return A pointer to the first coefficient.
	 */
	double* getCoefficients() {
		return &m_coefficients[0];
	}
};

/**
 * Creates a hamming window for use with the filter
 * coefficient creator functions.
 * @param size The size of the window in samples.
 * @param destination A pointer to the first element of the destination array.
 */
void createHammingWindow(int size, double* destination);
/**
 * Creates a blackman window for use with the filter
 * coefficient creator functions.
 * @param size The size of the window in samples.
 * @param destination A pointer to the first element of the destination array.
 */
void createBlackmanWindow(int size, double* destination);
/**
 * Creates a lowpass filter using N coefficients using
 * a sync function.
 * @param size The number of coefficients to generate.
 * @param destination Pointer to the first element of an array to write these coefficients to.
 * @param freqFrac Frequency as a fraction of the sample rate. (e.g. 0.1 @ 44.1kHz = 4.41kHz)
 * @param window A pointer to the first element of a window array created with a create[window]Window() function.
 */
void createLowpassCoefficients(int size, double* destination, double freqFrac, double* window);
/**
 * Creates a highpass filter using N coefficients using
 * a spectrally inverted sync function.
 * @param size The number of coefficients to generate.
 * @param destination Pointer to the first element of an array to write these coefficients to.
 * @param freqFrac Frequency as a fraction of the sample rate. (e.g. 0.1 @ 44.1kHz = 4.41kHz)
 * @param window A pointer to the first element of a window array created with a create[window]Window() function.
 */
void createHighpassCoefficients(int size, double* destination, double freqFrac, double* window);
/**
 * Fills a filter with all 1's, multiplied by a window.
 * @param size The number of 1's to generate.
 * @param destination Pointer to the first element of an array to write the 1's to.
 * @param window A pointer to the first element of a window array created with a create[window]Window() function.
 */
void createDebugCoefficients(int size, double* destination, double* window);

} /* namespace AtomSynth */

#endif /* SOURCE_ADSP_FIR_H_ */
