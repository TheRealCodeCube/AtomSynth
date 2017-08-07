/*
 * waveform.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#include "Waveform.h"

#include <cmath>

namespace Adsp {

//Calculates a sine wave at the given phase.
double sineWave(double phase) {
	return sin(phase * M_PI);
}

//Calculates a square wave at the given phase. It is low before 0.0, and high after.
double squareWave(double phase) {
	return (phase > 0.0) ? 1.0 : -1.0;
}

//Calculates a triangle wave. Peak is at phase 0.0.
double triWave(double phase) {
	return phase * ((phase > 0.0) ? -2.0 : 2.0) + 1.0;
}

//Calculates an exponential wave.
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
double expWave(double phase, double var) {
	var = (var + 1.15) * 2.0; //Minimum value is 0.3, max value is 4.3.
	return pow(phase * 0.5 + 0.5, var) * 2.0 - 1.0; //For pow function, phase should be between 0 and 1, not -1 and 1. The result is also between 0 and 1, and is remapped to -1 to 1.
}

//Calculates a symmetrical exp wave (two exp waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a triangle wave, do -0.65.
double expSymmWave(double phase, double var) {
	var = (var + 1.15) * 2.0; //Minimum value is 0.3, max value is 4.3.
	if (phase < 0.0) {
		return pow(-phase, var) * 2.0 - 1.0;
	} else {
		return pow(phase, var) * 2.0 - 1.0;
	}
}

//Calculates a crested exponential wave. (y=\left\{-1<x<0:\ \left(x+1\right)^v,\ 1>x>0:\ 1-x^v\right\})
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
//Crest is at phase 0.0
double expCrestWave(double phase, double var) {
	var = (var + 1.15) * 2.0; //Minimum value is 0.3, max value is 4.3.
	if (phase < 0.0) {
		return pow(phase + 1.0, var) * 2.0 - 1.0; //Normal exp wave before peak.
	} else {
		return 1.0 - 2.0 * pow(phase, var); //Flipped exp wave after peak.
	}
}

//Calculates a tanh wave (tanh(x*v)/tanh(v))
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
double tanhWave(double phase, double var) {
	var += 1.5;
	return tanh(phase * var) / tanh(var);
}

//Calculates a symmetrical tanh wave (two tanh waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
double tanhSymmWave(double phase, double var) {
	var += 1.5;
	return tanh((phase * ((phase < 0.0) ? 2.0 : -2.0) + 1.0) * var) / tanh(var);
}

} /* namespace Adsp */

