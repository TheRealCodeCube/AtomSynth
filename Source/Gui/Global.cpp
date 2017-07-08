/*
 * Global.cpp
 *
 *  Created on: Apr 29, 2017
 *      Author: josh
 */

#include "Global.h"

namespace C
{
	Font CUSTOM_FONT, DEFAULT_FONT, LARGE_FONT;

	void loadFont()
	{
		int dataSize;
		const char * data = BinaryData::getNamedResource("VeraMono_ttf", dataSize);
		DEFAULT_FONT = Font(Typeface::createSystemTypefaceFor(data, dataSize));
		DEFAULT_FONT.setHeight(FONT_SIZE);
		CUSTOM_FONT = DEFAULT_FONT;
		LARGE_FONT = DEFAULT_FONT;
		LARGE_FONT.setHeight(LARGE_FONT_SIZE);
	}
}
