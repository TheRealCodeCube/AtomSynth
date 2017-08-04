/*
 * AtomList.h
 *
 *  Created on: Dec 3, 2016
 *      Author: josh
 */

#ifndef SOURCE_ATOMLIST_H_
#define SOURCE_ATOMLIST_H_

#include <vector>

#include "Atom.h"
#ifdef INCLUDE_ATOMS
#include "Basic/Input.h"
#include "Basic/Output.h"
#include "Basic/Multiply.h"
#include "Generation/BasicOsc.h"
#include "Generation/Noise.h"
#include "Generation/Envelope.h"
#include "Processing/Amplifier.h"
#include "Processing/Mixer.h"
/*
 #include "Filters/Amplifier.h"
 #include "Filters/Comb.h"
 #include "Filters/Mixer.h"
 #include "Transforms/LfoTuner.h"*/
//#include "Filters/Reverb.h"
#endif

namespace AtomSynth {

std::vector<AtomController *> getAllAtoms() {
	std::vector<AtomController *> toReturn;
#ifdef INCLUDE_ATOMS
	toReturn.push_back(new InputController());
	toReturn.push_back(new OutputController());
	toReturn.push_back(new MultiplyController());
	toReturn.push_back(new BasicOscController());
	toReturn.push_back(new NoiseController());
	toReturn.push_back(new EnvelopeController());
	toReturn.push_back(new AmplifierController());
	toReturn.push_back(new MixerController());
	/*
	 toReturn.push_back(new EnvelopeController());
	 toReturn.push_back(new LfoTunerController());
	 toReturn.push_back(new BasicOscController());
	 toReturn.push_back(new NoiseController());
	 toReturn.push_back(new ReverbController());
	 toReturn.push_back(new CombController());
	 toReturn.push_back(new MixerController());
	 toReturn.push_back(new AmplifierController());*/
#endif
	return toReturn;
}

}

#endif /* SOURCE_ATOMLIST_H_ */
