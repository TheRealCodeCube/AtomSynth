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
/**
 * @defgroup basic
 * @ingroup Adsp
 * Very simple DSP functions.
 * @{
 */

/**
 * Compensation for sinusoidal panning. When doing sinusoidal
 * panning, values in the center will be ~0.7. Multiplying
 * the output of sinusoidal panning by this value will
 * normalize them such that centered panning does not affect
 * amplitude. This values is calculated as
 * \f${1}\div{cos(\frac{\pi}{4})}\f$
 */
constexpr double PAN_BOOST = 1.41421356237; //1.0 / cos(pi/4)

/**
 * Amplify a value. It's just multiplication, nothing special.
 * It's really only used for readability.
 * @param value The value to amplify.
 * @param amplification The factor of amplification.
 * @return The amplified value.
 */
constexpr double amplify(double value, double amplification) {
	return value * amplification;
}

/**
 * Gets the amplitude of the left channel for panning. Given a
 * panning value, this calculates how loud the left channel
 * should be. Uses a sinusoidal panning algorithm.
 * @param panning A double between -1 and 1, with -1 being left and 1 being right.
 * @return The amplitude of the left channel. This can be between 0 and PAN_BOOST, and will be 1 when the panning is centered.
 */
constexpr double panLeftAmplitude(double panning) {
	return cos(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

/**
 * Gets the amplitude of the right channel for panning. Given a
 * panning value, this calculates how loud the right channel
 * should be. Uses a sinusoidal panning algorithm.
 * @param panning A double between -1 and 1, with -1 being left and 1 being right.
 * @return The amplitude of the right channel. This can be between 0 and PAN_BOOST, and will be 1 when the panning is centered.
 */
constexpr double panRightAmplitude(double panning) {
	return sin(panning * 0.25 * M_PI + (0.25 * M_PI)) * PAN_BOOST;
}

/**
 * Pans a sample, assuming the sample is in the left channel.
 * Uses panLeftAmplitude() to amplify the sample by the
 * appropriate amount.
 * @param value The sample (from a left channel) to pan.
 * @param panning A double between -1 and 1, with -1 being left and 1 being right.
 * @return The panned sample.
 */
constexpr double panLeft(double value, double panning) {
	return amplify(value, panLeftAmplitude(panning));
}

/**
 * Pans a sample, assuming the sample is in the right channel.
 * Uses panLeftAmplitude() to amplify the sample by the
 * appropriate amount.
 * @param value The sample (from a right channel) to pan.
 * @param panning A double between -1 and 1, with -1 being left and 1 being right.
 * @return The panned sample.
 */
constexpr double panRight(double value, double panning) {
	return amplify(value, panRightAmplitude(panning));
}

/**
 * Hard clipping between -1 and 1. If the input is less than
 * -1, the output is -1. Likewise, if the output is greater
 * than 1, the output is 1.
 * @param value The sample to clip.
 * @return The clipped sample, in a range between -1 and 1.
 */
constexpr double clip(double value) {
	return (value < -1.0) ? -1.0 : ((value > 1.0) ? 1.0 : value);
}

/**
 * Hard clipping between an arbitrary range. If the input
 * is less than the minimum, the output is the minimum.
 * Likewise, if the output is greater than the maximum,
 * the output is the maximum.
 * @param value The sample to be clipped.
 * @param min The minimum value the sample can have before being clipped.
 * @param max The maximum value the sample can have before being clipped.
 * @return The clipped sample.
 */
constexpr double clip(double value, double min, double max) {
	return (value < min) ? min : ((value > max) ? max : value);
}

constexpr int MIX_MODE_AVERAGE = 0, ///< Values should be mixed by averaging them.
		MIX_MODE_MULTIPLY = 1, ///< Values should be mixed by multiplying them.
		MIX_MODE_MINIMUM = 2, ///< Only the minimum of all the values should be used.
		MIX_MODE_MAXIMUM = 3, ///< Only the maximum of all the values should be used.
		MIX_MODE_ADD = 4; ///< Values should be mixed by adding them.

/**
 * Mixes two values using the specified mix mode. This is
 * handy in applications where the user can specify their
 * own mix mode, rather than having it hard-coded in.
 * @param value1 A value to mix.
 * @param value2 A value to mix.
 * @param mixMode The method by which to mix the two values.
 * @return A mixture of the two input values.
 */
constexpr double mix2(double value1, double value2, int mixMode) {
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

/**
 * Mixes three values using the specified mix mode. This is
 * handy in applications where the user can specify their
 * own mix mode, rather than having it hard-coded in.
 * @param value1 A value to mix.
 * @param value2 A value to mix.
 * @param value3 A value to mix.
 * @param mixMode The method by which to mix the two values.
 * @return A mixture of the two input values.
 */
constexpr double mix3(double value1, double value2, double value3, int mixMode) {
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

/** @} */

} /* namespace Adsp */

#endif /* SOURCE_ADSP_BASIC_H_ */
