/*
 * remap.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_REMAP_H_
#define SOURCE_ADSP_REMAP_H_

namespace Adsp {
/**
 * @defgroup remap
 * @ingroup Adsp
 * A couple functions for remapping values from one range to another.
 * @{
 */

//Remaps value from a range going from inMin to inMax to a range going from outMin to outMax
//E.G. if value = 0.5, inMin, inMax = 0.0, 1.0, outMin, outMax = -1.0, 1.0, then it returns 0.0.
/**
 * Remaps a value from one range to another range. For example, you
 * could use this function to remap samples with a range of 0 to 1 to
 * a range of -1 to 1 by doing something like remap(v, 0, 1, -1, 1).
 * Uses this function:
 * \f$\frac{v-i_{\min }}{i_{\max }-i_{\min }}\cdot \left(o_{\max }-o_{\min }\right)+o_{\min }\f$
 * This particular function is only recommended if the range will
 * change very often. If the range stays the same, use fastRemap().
 * @param value The value to remap.
 * @param inMin The lower bound of the range of the input value.
 * @param inMax The upper bound of the range of the input value.
 * @param outMin The desired lower bound of the range of the output value.
 * @param outMax The desired upper bound of the range of the output value.
 * @return The remapped value.
 */
constexpr double remap(double value, double inMin, double inMax, double outMin, double outMax) {
	return ((value - inMin) / (inMax - inMin)) * (outMax - outMin) + outMin;
}

/**
 * Computes constA for use in fastRemap(). See fastRemap() for
 * more details about when you should use this. This function
 * calculates the constant using this formula:
 * \f$\frac{o_{max}-o_{min}}{i_{max}-i_{min}}\f$
 * @param inMin The lower bound of the range of the input value.
 * @param inMax The upper bound of the range of the input value.
 * @param outMin The desired lower bound of the range of the output value.
 * @param outMax The desired upper bound of the range of the output value.
 * @return The value of constA for use in fastRemap().
 */
constexpr double fastRemapComputeA(double inMin, double inMax, double outMin, double outMax) {
	return (outMax - outMin) / (inMax - inMin);
}

/**
 * Computes constB for use in fastRemap(). See fastRemap() for
 * more details about when you should use this. This function
 * calculates the constant using this formula:
 * \f$\frac{i_{min}*\left(o_{max}-o_{min}\right)}{i_{max}-i_{min}}+o_{max}\f$
 * @param inMin The lower bound of the range of the input value.
 * @param inMax The upper bound of the range of the input value.
 * @param outMin The desired lower bound of the range of the output value.
 * @param outMax The desired upper bound of the range of the output value.
 * @return The value of constB for use in fastRemap().
 */
constexpr double fastRemapComputeB(double inMin, double inMax, double outMin, double outMax) {
	return (inMin * (outMax - outMin)) / (inMax - inMin) + outMax;
}

//Fast remap function. Takes A and B constants created by the fastRemapCompute functions.
//This function itself is much faster than the normal remap function.
//However, calling both compute functions and the fast remap function all at once is slower than the normal remap function.
//Thus, this function is best for a constant remap that must be applied over a large number of values.
/**
 * Fast remap function. This is useful when you need to remap
 * many values from one range to another. Instead of taking in
 * the upper and lower bounds of the input and output range,
 * it takes in constA and constB, which can be computed by
 * fastRemapComputeA() and fastRemapComputeB(), respectively.
 * This calculation only needs to be done once. It is much faster
 * than calling remap() many times. However, if the range also
 * changes with the values, then use remap().
 * @param value The value to remap.
 * @param constA A constant as calculated by fastRemapComputeA()
 * @param constB A constant as calculated by fastRemapComputeB()
 * @return The remapped value.
 */
constexpr double fastRemap(double value, double constA, double constB) {
	return value * constA + constB;
}

} /* namespace Adsp */

#endif /* SOURCE_ADSP_REMAP_H_ */
