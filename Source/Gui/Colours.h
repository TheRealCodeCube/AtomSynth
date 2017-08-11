/*
 * Colours.h
 *
 *  Created on: Aug 17, 2016
 *      Author: josh
 */

#ifndef SOURCE_COLOURS_H_
#define SOURCE_COLOURS_H_

#include "../JuceLibraryCode/JuceHeader.h"

#define ATOM_BG Colour(0.0f, 0.0f, 0.75f, 1.0f)
#define ATOM_TEXT_BG Colour(0.0f, 0.0f, 0.5f, 1.0f)
#define ATOM_CANVAS Colour(0.6f, 0.0f, 0.4f, 1.0f)
#define WHITE Colour(0.0f, 0.0f, 0.95f, 1.0f)

#define BACK_LAYER ATOM_TEXT_BG
#define MID_LAYER ATOM_BG
#define FORE_LAYER WHITE

#define CONNECTION_TYPE_WHITE Colour(0.0f, 0.0f, 0.95f, 1.0f)
#define CONNECTION_TYPE_BLACK Colour(0.0f, 0.0f, 0.3f, 1.0f)

#define RED Colour(255, 15, 15)
#define GREEN Colour(15, 240, 127)
#define DEBUG_PINK Colour(255, 0, 255)

extern Colour ATOM_COLOURS[12];

#endif /* SOURCE_COLOURS_H_ */
