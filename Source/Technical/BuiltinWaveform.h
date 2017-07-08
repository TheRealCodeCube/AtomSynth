/*
 * BuiltinWaveform.h
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#ifndef SOURCE_BUILTINWAVEFORM_H_
#define SOURCE_BUILTINWAVEFORM_H_

namespace AtomSynth
{

namespace Waveform
{

enum WaveShape { SINE, SQUARE, TRIANGLE, EXPONENTIAL };

double calculate(WaveShape shape, double phase, double parm); //Everything except shape goes from -1.0f to 1.0f

} /* namespace Waveform */

} /* namespace AtomSynth */

#endif /* SOURCE_BUILTINWAVEFORM_H_ */
