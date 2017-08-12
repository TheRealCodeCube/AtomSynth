/*
 * basic.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#include "Basic.h"

namespace Adsp {

#ifdef USE_STRICT_CONSTEXPR
double panLeftAmplitude(double panning) {
	return cos(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

double panRightAmplitude(double panning) {
	return sin(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

double panLeft(double value, double panning) {
	return amplify(value, panLeftAmplitude(panning));
}

double panRight(double value, double panning) {
	return amplify(value, panRightAmplitude(panning));
}

double mix2(double value1, double value2, int mixMode) {
	switch(mixMode) {
	case MIX_MODE_AVERAGE:
		return (value1 + value2) / 2.0;
	case MIX_MODE_MULTIPLY:
		return (value1 * value2);
	case MIX_MODE_MINIMUM:
		return (value1 < value2) ? value1 : value2;
	case MIX_MODE_MAXIMUM:
		return (value1 > value2) ? value1 : value2;
	case MIX_MODE_ADD:
		return value1 + value2;
	default:
		return NAN;
	}
}

double mix3(double value1, double value2, double value3, int mixMode) {
	switch(mixMode) {
	case MIX_MODE_AVERAGE:
		return (value1 + value2 + value3) / 3.0;
	case MIX_MODE_MULTIPLY:
		return (value1 * value2 * value3);
	case MIX_MODE_MINIMUM:
		return (value1 < value2) ? value1 : ((value2 < value3) ? value2: value3);
	case MIX_MODE_MAXIMUM:
		return (value1 > value2) ? value1 : ((value2 > value3) ? value2: value3);
	case MIX_MODE_ADD:
		return value1 + value2 + value3;
	default:
		return NAN;
	}
}
#endif

} /* namespace Adsp */

