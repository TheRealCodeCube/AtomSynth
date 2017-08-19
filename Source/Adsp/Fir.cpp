/*
 * Fir.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: josh
 */

#include "Fir.h"

#include <cmath>

namespace Adsp {

void createHammingWindow(int size, double* destination) {
	double offset = (2 * M_PI) / (size - 1);
	for(double x = 0; x < M_PI * 2; x += offset) {
		*destination = 0.54 - 0.46 * std::cos(x);
		destination++;
	}
}

void createBlackmanWindow(int size, double* destination) {
	double offset = (2 * M_PI) / (size - 1);
	for(double x = 0; x < M_PI * 2; x += offset) {
		*destination = 0.42 - 0.5 * std::cos(x) + 0.08 * std::cos(2 * x);
		destination++;
	}
}

/**
 * Renders a normalized, windowed sync function to an
 * array of doubles.
 * @param size The number of samples to render. The center of the sync function will be at size / 2.
 * @param destination A pointer to the first element of the destination array.
 * @param freqFrac A fraction of the sample rate. (E.G. 0.1 @ 44khz = 4.4khz.)
 * @param window A pointer to the first element of an array containing the window to use.
 */
void windowedSinc(int size, double* destination, double freqFrac, double* window) {
	double sinMult = 2 * M_PI * freqFrac, sum = 0.0;
	int offset = size / 2;
	double *iter = destination;
	//E.G. if size is 256, loops from -128 to 127. +0.01 is to avoid div0 errors.
	for(double x = -offset + 0.01; x < size - offset + 0.01; x++) {
		*iter = (sin(sinMult * x) / x) * *window;
		sum += *iter;
		iter++;
		window++;
	}
	iter = destination;
	sum = 1.0 / sum; //One division and many multiplications is better than many divisions.
	for(int i = 0; i < size; i++) {
		(*iter) *= sum;
		iter++;
	}
}

/**
 * Applies spectral inversion (*-1 to every other
 * coefficient) to a filter kernel.
 * @param size The number of samples in the kernel.
 * @param coefficients A pointer to the first coefficient in the kernel.
 */
void spectralInversion(int size, double* coefficients) {
	coefficients++;
	for(int i = 0; i < int(size / 2); i++) {
		(*coefficients) = -(*coefficients);
		coefficients += 2;
	}
}

void createLowpassCoefficients(int size, double* destination, double freqFrac, double* window) {
	windowedSinc(size, destination, freqFrac, window);
}

void createHighpassCoefficients(int size, double* destination, double freqFrac, double* window) {
	//Invert frequency, make lowpass filter, invert that.
	freqFrac = 0.5 - freqFrac;
	windowedSinc(size, destination, freqFrac, window);
	spectralInversion(size, destination);
}

void createDebugCoefficients(int size, double* destination, double* window) {
	int offset = size / 2;
	double sum = 0.0;
	double *iter = destination;
	//E.G. if size is 256, loops from -128 to 127. +0.01 is to avoid div0 errors.
	for(double x = -offset + 0.01; x < size - offset + 0.01; x++) {
		*iter = *window;
		sum += *iter;
		iter++;
		window++;
	}
	iter = destination;
	sum = 1.0 / sum; //One division and many multiplications is better than many divisions.
	//Normalize
	for(int x = -offset; x < size - offset; x++) {
		*iter *= sum;
		iter++;
	}
}

} /* namespace AtomSynth */
