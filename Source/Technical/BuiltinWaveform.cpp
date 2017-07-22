/*
 * BuiltinWaveform.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#include "BuiltinWaveform.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace AtomSynth {

namespace Waveform {

double calculate(WaveShape shape, double phase, double parm) {
	double value = 0.0f;
	switch (shape) {
	case SINE:
		return (-sin(phase * M_PI) / 2.0f + 0.5f) * 2.0 - 1.0;
	case SQUARE:
		return (phase < 0.0) ? (1.0) : (-1.0);
	case TRIANGLE:
		value = (phase < 0.0) ? (phase + 1.0f) : -(phase - 1.0f);
		return value * 2.0 - 1.0;
	case EXPONENTIAL:
		parm += 1.4;
		parm *= parm * parm * parm;
		return pow((phase / 2.0) + 0.5, parm) * 2.0 - 1.0;
	default:
		return 0.0f;
	}
}

} /* namespace Waveform */

} /* namespace AtomSynth */

