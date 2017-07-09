/*
 * remap.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_REMAP_H_
#define SOURCE_ADSP_REMAP_H_

namespace Adsp {

//Remaps value from a range going from inMin to inMax to a range going from outMin to outMax
//E.G. if value = 0.5, inMin, inMax = 0.0, 1.0, outMin, outMax = -1.0, 1.0, then it returns 0.0.
constexpr double remap(double value, double inMin, double inMax, double outMin, double outMax) {
	//Implementation: \frac{v-i_{\min }}{i_{\max }-i_{\min }}\cdot \left(o_{\max }-o_{\min }\right)+o_{\min }
	return ((value - inMin) / (inMax - inMin)) * (outMax - outMin) + outMin;
}

//Computes the A constant for the fast remap function.
constexpr double fastRemapComputeA(double inMin, double inMax, double outMin, double outMax) {
	//Implementation: \frac{o_{\max }-o_{\min }}{i_{\max }-i_{\min }}
	return (outMax - outMin) / (inMax - inMin);
}

//Computes the B constant for the fast remap function.
constexpr double fastRemapComputeB(double inMin, double inMax, double outMin, double outMax) {
	//Implementation: \frac{i_{\min }\cdot \left(o_{\max }-o_{\min }\right)}{i_{\max }-i_{\min }}+o_{\max }
	return (inMin * (outMax - outMin)) / (inMax - inMin) + outMax;
}

//Fast remap function. Takes A and B constants created by the fastRemapCompute functions.
//This function itself is much faster than the normal remap function.
//However, calling both compute functions and the fast remap function all at once is slower than the normal remap function.
//Thus, this function is best for a constant remap that must be applied over a large number of values.
constexpr double fastRemap(double value, double constA, double constB) {
	//Full implementation: v\cdot \left(\frac{o_{\max }-o_{\min }}{i_{\max }-i_{\min }}\right)+\left(\frac{i_{\min }\cdot \left(o_{\max }-o_{\min }\right)}{i_{\max }-i_{\min }}+o_{\max }\right)
	//The terms in parenthesis are calculated earlier.
	return value * constA + constB;
}

} /* namespace Adsp */

#endif /* SOURCE_ADSP_REMAP_H_ */
