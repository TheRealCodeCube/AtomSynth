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

void createLowpassCoefficients(int size, double* destination, double freqFrac, double* window) {
	double sinMult = 2 * M_PI * freqFrac, sum;
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
	//Normalize
	for(int x = -offset; x < size - offset; x++) {
		*iter *= sum;
		iter++;
	}
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
