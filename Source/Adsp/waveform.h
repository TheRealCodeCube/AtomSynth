/*
 * waveform.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_WAVEFORM_H_
#define SOURCE_ADSP_WAVEFORM_H_

namespace Adsp {

//Calculates a sine wave at the given phase.
double sineWave(double phase);

//Calculates a square wave at the given phase. It is low before 0.0, and high after.
double squareWave(double phase);

//Calculates a triangle wave. Peak is at phase 0.0.
double triWave(double phase);

//Calculates an exponential wave.
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
double expWave(double phase, double var);

//Calculates a symmetrical exp wave (two exp waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a triangle wave, do -0.65.
double expSymmWave(double phase, double var);

//Calculates a crested exponential wave. (y=\left\{-1<x<0:\ \left(x+1\right)^v,\ 1>x>0:\ 1-x^v\right\})
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
//Crest is at phase 0.0
double expCrestWave(double phase, double var);

//Calculates a tanh wave (tanh(x*v)/tanh(v))
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
double tanhWave(double phase, double var);

//Calculates a symmetrical tanh wave (two tanh waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
double tanhSymmWave(double phase, double var);

} /* namespace Adsp */

#endif /* SOURCE_ADSP_WAVEFORM_H_ */
