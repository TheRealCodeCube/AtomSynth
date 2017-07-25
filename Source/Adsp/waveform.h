/*
 * waveform.h
 *
 *  Created on: Jul 8, 2017
 *      Author: josh
 */

#ifndef SOURCE_ADSP_WAVEFORM_H_
#define SOURCE_ADSP_WAVEFORM_H_

namespace Adsp {
/**
 * @defgroup waveform
 * @ingroup Adsp
 * Functions for generating different waveforms.
 * @{
 */


/**
 * Computes a sine wave. (function:
 * \f$ sin(\pi*phase)\f$)
 * @param phase Phase value, from -1 to 1.
 * @return Computed sine wave, from -1 to 1.
 */
double sineWave(double phase);

/**
 * Computes a square wave. (function:
 * \f$ \left\{\begin{aligned}&-1&&:phase\leq0\\&1&&:phase>0\end{aligned}\right.\f$)
 * @param phase Phase value, from -1 to 1.
 * @return Computed square wave, from -1 to 1.
 */
double squareWave(double phase);

/**
 * Computes a triangle wave, with its peak
 * at phase 0. (function:
 * \f$ 1-2|x|\f$)
 * @param phase Phase value, from -1 to 1.
 * @return Computed square wave, from -1 to 1.
 */
double triWave(double phase);

//Calculates an exponential wave.
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
/**
 * Computes an exponential wave. Var is
 * remapped from -1 - 1 to .3 - 4.3. So to
 * get a straight line, var must be -.65.
 * (function, assuming var is already
 * remapped:
 * \f$ 2(\frac{phase+1}{2})^{var}-1\f$)
 * @param phase Phase value, from -1 to 1.
 * @param var A value from -1 to 1 which changes the shape of the wave.
 * @return Computed square wave, from -1 to 1.
 */
double expWave(double phase, double var);

//Calculates a symmetrical exp wave (two exp waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a triangle wave, do -0.65.
/**
 * Computes a symmetrical exp wave (two
 * exp waves mirrored and stuck end-to-end.)
 * Var is remapped from -1 - 1 to .3 - 4.3.
 * So to get a straight line, var must be -.65.
 * See expWave() for function.
 * @param phase Phase value, from -1 to 1.
 * @param var A value from -1 to 1 which changes the shape of the wave.
 * @return Computed square wave, from -1 to 1.
 */
double expSymmWave(double phase, double var);

//Calculates a crested exponential wave. (y=\left\{-1<x<0:\ \left(x+1\right)^v,\ 1>x>0:\ 1-x^v\right\})
//Var is remapped from -1.0 - 1.0 to 0.3 - 4.3. Therefore, for a straight line, do -0.65.
//Crest is at phase 0.0
/**
 * Computes a crested exp wave (from -1 to 0,
 * a normal exp wave, and from 0 to 1, an
 * inverted exp wave, creating a crested
 * shape.)
 * \f$ \f$
 * @param phase Phase value, from -1 to 1.
 * @param var A value from -1 to 1 which changes the shape of the wave.
 * @return Computed square wave, from -1 to 1.
 */
double expCrestWave(double phase, double var);

//Calculates a tanh wave (tanh(x*v)/tanh(v))
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
/**
 * Computes a tanh wave. Var is remapped from
 * -1 - 1 to .5 - 2.5. (function, assuming var
 * is already remapped:
 * \f$ \frac{tanh(phase*var)}{tanh(var)}\f$)
 * @param phase Phase value, from -1 to 1.
 * @param var A value from -1 to 1 which changes the shape of the wave.
 * @return Computed square wave, from -1 to 1.
 */
double tanhWave(double phase, double var);

//Calculates a symmetrical tanh wave (two tanh waves, mirrored, stuck end to end)
//Var is remapped from -1.0 - 1.0 to 0.5 - 2.5.
/**
 * Computes a symmetrical tanh wave (two
 * tanh waves, mirrored and stuck end-to-end.)
 * See tanhWave() for more details.
 * @param phase Phase value, from -1 to 1.
 * @param var A value from -1 to 1 which changes the shape of the wave.
 * @return Computed square wave, from -1 to 1.
 */
double tanhSymmWave(double phase, double var);

} /* namespace Adsp */

#endif /* SOURCE_ADSP_WAVEFORM_H_ */
