/*
 * Fir.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: josh
 */

#include "Fir.h"

#include <cmath>

namespace Adsp {

void createLowpassCoefficients(int size, double* destination, double freqFrac) {
	double sinMult = 2 * M_PI * freqFrac, sum;
	int offset = size / 2;
	double *iter = destination;
	//E.G. if size is 256, loops from -128 to 127. +0.01 is to avoid div0 errors.
	for(double x = -offset + 0.01; x < size - offset + 0.01; x++) {
		*iter = sin(sinMult * x) / x;
		sum += *iter;
		iter++;
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
