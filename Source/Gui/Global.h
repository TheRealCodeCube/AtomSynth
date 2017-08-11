/*
 * BasicConstants.h
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_GLOBAL_H_
#define SOURCE_GUI_GLOBAL_H_

#include "JuceLibraryCode/JuceHeader.h"

namespace C //Short for Constants
{
constexpr int CORNER_SIZE = 4,      //Used for rounded rectangles.
		LINE_SIZE = 4,        //For drawing lines, like the indicator on knobs.
		SMALL_LINE_SIZE = 2,  //For drawing small lines, like on various graphs.
		SPACING = 4,          //Used for spacing objects that are next to each other.
		SMALL_SPACING = 2,    //Used for spacing objects that are inside another object. (E.G. text inside an on/off button)
		HANDLE_SIZE = LINE_SIZE * 2, //E.G. used for the handles in the envelope's gui.
		FONT_SIZE = 12,       //Used for creating a label for a widget.
		LARGE_FONT_SIZE = FONT_SIZE + 4,    //Used for more easily readable text (E.G. knob readouts)
		LABEL_HEIGHT = FONT_SIZE - 2;       //Used for text.

#define CD(INDEX) (C::CELL_SIZE * (INDEX) + C::SPACING * ((INDEX) + 1)) //Cell distance (Use for placing objects on a gui.)
#define CS(INDEX) (C::CELL_SIZE * (INDEX) + C::SPACING * ((INDEX) - 1)) //Cell size (use for setting size of objects on a gui.)
#define CB(X, Y, W, H) CD(X), CD(Y), CS(W), CS(H)                       //Cell bounding box (Use in setBounds function of a component.)

constexpr int CELL_SIZE = 30,          //Used for generic layouts.
		KNOB_SIZE = CS(2),       //Used for knobs.
		SMALL_KNOB_SIZE = CS(1), //Used for small knobs.
		HALF_CELL_SIZE = (CELL_SIZE - SPACING) / 2;  //Used for tiny details in generic layouts.

#define HD(INDEX) (C::HALF_CELL_SIZE * (INDEX) + C::SPACING * ((INDEX) + 1)) //Half cell distance. Same as CD, except with half cells.
#define HS(INDEX) (C::HALF_CELL_SIZE * (INDEX) + C::SPACING * ((INDEX) - 1)) //Half cell size. Same as CS, except with half cells.
#define HB(X, Y, W, H) HD(X), HD(Y), HS(W), HS(H)                            //Half cell bounding box. Same as CB, except with half cells.

constexpr int HEADER_WIDTH = CS(9), HEADER_HEIGHT = CS(1), VST_WIDTH = CD(30), GUI_HEIGHT = CD(20), VST_HEIGHT = GUI_HEIGHT + HEADER_HEIGHT, GUI_WIDTH = CD(12), BUTTON_TRAY_WIDTH = GUI_WIDTH - HEADER_WIDTH - SPACING;

constexpr double MIN_TEXT_SCALE = 0.8, //Used for drawing fitted text.
		MOUSE_MULT = 0.005;   //Used for mouse input.

extern Font CUSTOM_FONT, DEFAULT_FONT, LARGE_FONT;
void loadFont();
} /* namespace BasicConstants */

#endif /* SOURCE_GUI_GLOBAL_H_ */
