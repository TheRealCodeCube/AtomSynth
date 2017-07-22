/*
 * basic.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_BASIC_H_
#define SOURCE_ADSP_BASIC_H_

#include <cmath>

namespace Adsp {

constexpr double PAN_BOOST = 1.0 / 0.707106781187; //1.0 / cos(pi/4)

//Just amplifies the first value. Nothing special, it's just multiplication. Mostly used for readability.
constexpr double amplify(double value, double amplification) {
	return value * amplification;
}

//Gets the amplitude of the left channel for the specified panning.
//Panning goes from -1.0 to 1.0, with the former being left and the latter being right.
//Uses a sinusoidal algorithm for panning, because I think it sounds the smoothest and most realistic.
constexpr double panLeftAmplitude(double panning) {
	return cos(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

//Gets the amplitude of the right channel for the specified panning.
//Panning goes from -1.0 to 1.0, with the former being left and the latter being right.
//Uses a sinusoidal algorithm for panning, because I think it sounds the smoothest and most realistic.
constexpr double panRightAmplitude(double panning) {
	return sin(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

//Pans the sample, assuming the sample is in the left channel.
//Panning goes from -1.0 to 1.0, with the former being left and the latter being right.
//Uses a sinusoidal algorithm for panning, because I think it sounds the smoothest and most realistic.
constexpr double panLeft(double value, double panning) {
	return amplify(value, panLeftAmplitude(panning));
}

//Pans the sample, assuming the sample is in the right channel.
//Panning goes from -1.0 to 1.0, with the former being left and the latter being right.
//Uses a sinusoidal algorithm for panning, because I think it sounds the smoothest and most realistic.
constexpr double panRight(double value, double panning) {
	return amplify(value, panRightAmplitude(panning));
}

//Clips the sample if it is above 1.0 or below -1.0.
constexpr double clip(double value) {
	return (value < -1.0) ? -1.0 : ((value > 1.0) ? 1.0 : value);
}

//Clips the sample to a given range.
constexpr double clip(double value, double min, double max) {
	return (value < min) ? min : ((value > max) ? max : value);
}

} /* namespace Adsp */

#endif /* SOURCE_ADSP_BASIC_H_ */
