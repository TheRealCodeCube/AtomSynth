/*
 * interpolation.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#include "interpolation.h"

namespace Adsp {

double fastEnvelopeInterp(double blend, double shape) {
	if (shape == 0.0) {
		return blend;
	} else {
		double blendp = blend * 100.0;
		int index = int(blendp);
		double lookupBlend = blendp - double(index); //This value goes between 0 and 1.
		if (shape > 0.0) {
			return linearInterp(blend, linearInterp(ENVELOPE_POSITIVE_SHAPE[index], ENVELOPE_POSITIVE_SHAPE[index + 1], lookupBlend), //Do linear interpolation on the sample data so that the envelope does not have sharp edges.
			shape); //Blend between a simple line and the calculated shape.
		} else {
			return linearInterp(blend, linearInterp(ENVELOPE_NEGATIVE_SHAPE[index], ENVELOPE_NEGATIVE_SHAPE[index + 1], lookupBlend), //Do linear interpolation so that the envelope does not have sharp edges.
			-shape); //Blend between a simple line and the calculated shape.
		}
	}
}

double envelopeInterp(double val1, double val2, double blend, double shape) {
	return fastEnvelopeInterp(blend, shape) * (val2 - val1) + val1;
}

};
