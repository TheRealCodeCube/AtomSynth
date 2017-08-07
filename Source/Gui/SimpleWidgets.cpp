/*
 * SimpleWidgets.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#include "SimpleWidgets.h"

#include <iomanip>
#include <sstream>

#include "Global.h"
#include "Gui/Colours.h"
#include "Technical/SaveState.h"
#include "Technical/Synth.h"

namespace AtomSynth {

Label * EnhancedComponent::createLabel(std::string text, bool top) {
	Label * toReturn = new Label();
	toReturn->setText(text, NotificationType::dontSendNotification);
	toReturn->setFont(Font(C::DEFAULT_FONT));
	toReturn->setMinimumHorizontalScale(C::MIN_TEXT_SCALE);
	if (top) {
		toReturn->setBounds(getX() - getWidth(), getY() - C::LABEL_HEIGHT - C::SPACING, getWidth() * 3, C::LABEL_HEIGHT);
	} else {
		toReturn->setBounds(getX() - getWidth(), getY() + getHeight() + C::SPACING, getWidth() * 3, C::LABEL_HEIGHT);
	}
	toReturn->setJustificationType(Justification::centred);
	return toReturn;
}

void RightClickMenu::paint(Graphics& g) {
	int w = getWidth() - (DS_SIZE * 2), h = getHeight() - (DS_SIZE * 2);
	Colour shadow = Colour(0.0, 0.0, 0.0f, 0.2f), transparent = Colour(0.0, 0.0, 0.0f, 0.0f);

	g.setGradientFill(ColourGradient(transparent, 0, 0, shadow, 0, DS_SIZE, false));
	g.fillRect(DS_SIZE, 0, w, DS_SIZE);
	g.setGradientFill(ColourGradient(transparent, 0, h + (DS_SIZE * 2), shadow, 0, h + DS_SIZE, false));
	g.fillRect(DS_SIZE, h + DS_SIZE, w, DS_SIZE);
	g.setGradientFill(ColourGradient(transparent, 0, 0, shadow, DS_SIZE, 0, false));
	g.fillRect(0, DS_SIZE, DS_SIZE, h);
	g.setGradientFill(ColourGradient(shadow, w + DS_SIZE, 0, transparent, w + (DS_SIZE * 2), 0, false));
	g.fillRect(w + DS_SIZE, DS_SIZE, DS_SIZE, h);

	g.setGradientFill(ColourGradient(shadow, DS_SIZE, DS_SIZE, transparent, DS_CORNER, DS_CORNER, true));
	g.fillRect(0, 0, DS_SIZE, DS_SIZE);
	g.setGradientFill(ColourGradient(shadow, w + DS_SIZE, DS_SIZE, transparent, w + (DS_SIZE * 2) - DS_CORNER, DS_CORNER, true));
	g.fillRect(w + DS_SIZE, 0, DS_SIZE, DS_SIZE);
	g.setGradientFill(ColourGradient(shadow, DS_SIZE, h + DS_SIZE, transparent, DS_CORNER, h + (DS_SIZE * 2) - DS_CORNER, true));
	g.fillRect(0, h + DS_SIZE, DS_SIZE, DS_SIZE);
	g.setGradientFill(ColourGradient(shadow, w + DS_SIZE, h + DS_SIZE, transparent, w + (DS_SIZE * 2) - DS_CORNER, h + (DS_SIZE * 2) - DS_CORNER, true));
	g.fillRect(w + DS_SIZE, h + DS_SIZE, DS_SIZE, DS_SIZE);

	g.setColour(MID_LAYER);
	g.fillRect(DS_SIZE, DS_SIZE, getWidth() - (DS_SIZE * 2), getHeight() - (DS_SIZE * 2));
}

void TextButton::paint(Graphics& g) {
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), C::CORNER_SIZE);
	g.setColour(MID_LAYER);
	if (m_flash) {
		g.fillRoundedRectangle(C::SPACING, C::SPACING, getWidth() - C::SPACING * 2, getHeight() - C::SPACING * 2, C::CORNER_SIZE);
		g.setColour(FORE_LAYER);
	}
	g.setFont(C::DEFAULT_FONT);
	g.drawFittedText(m_string, 0, 0, getWidth(), getHeight(), Justification::centred, 1, C::MIN_TEXT_SCALE);
}

void TextButton::mouseDown(const MouseEvent& event) {
	for (Listener * listener : m_listeners) {
		listener->textButtonPressed(this);
	}
	m_flash = true;
	repaint();
}

void TextButton::mouseUp(const MouseEvent& event) {
	m_flash = false;
	repaint();
}

MultiButton::MultiButton() :
		EnhancedComponent(),
		m_selectedLabel(0),
		m_vertical(false) {

}

MultiButton::~MultiButton() {

}

void MultiButton::paint(Graphics & g) {
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0.0, 0.0, getWidth(), getHeight(), C::CORNER_SIZE);
	g.setFont(C::DEFAULT_FONT);
	if (m_labels.size() != 0) {
		if (m_vertical) {
			double seperation = double(getBounds().getHeight()) / double(m_labels.size());
			for (int i = 0; i < m_labels.size(); i++) {
				g.setColour(MID_LAYER);
				if (i == m_selectedLabel) {
					g.fillRect(C::SMALL_SPACING, seperation * i + C::SMALL_SPACING, getWidth() - C::SMALL_SPACING * 2, seperation - C::SMALL_SPACING * 2);
					g.setColour(FORE_LAYER);
				}
				g.drawFittedText(m_labels[i], 0, seperation * i, getWidth(), seperation, Justification::centred, 1, C::MIN_TEXT_SCALE);
			}
		} else {
			double seperation = double(getBounds().getWidth()) / double(m_labels.size());
			for (int i = 0; i < m_labels.size(); i++) {
				g.setColour(MID_LAYER);
				if (i == m_selectedLabel) {
					g.fillRect(seperation * i + C::SMALL_SPACING, C::SMALL_SPACING, seperation - C::SMALL_SPACING * 2, getHeight() - C::SMALL_SPACING * 2);
					g.setColour(FORE_LAYER);
				}
				g.drawFittedText(m_labels[i], seperation * i, 0, seperation, getHeight(), Justification::centred, 1, C::MIN_TEXT_SCALE);
			}
		}
	}
}

void MultiButton::setSelectedLabel(int index) {
	m_selectedLabel = index;
	for (Listener * listener : m_listeners) {
		listener->multiButtonPressed(this);
	}
	repaint();
}

void MultiButton::mouseDown(const MouseEvent& event) {
	if (m_labels.size() != 0) {
		double pos = 0.0;
		if (m_vertical) {
			pos = double(event.getPosition().getY()) / double(getHeight() + 1);
		} else {
			pos = double(event.getPosition().getX()) / double(getWidth() + 1);
		}
		pos *= m_labels.size();
		pos = floor(pos);
		m_selectedLabel = int(pos);
		for (Listener * listener : m_listeners) {
			listener->multiButtonPressed(this);
		}
		repaint();
	}
}

void MultiButton::loadSaveState(SaveState state) {
	setSelectedLabel(state.getValue(0));
}

SaveState MultiButton::saveSaveState() {
	SaveState toReturn = SaveState();
	toReturn.addValue(m_selectedLabel);
	return toReturn;
}

SimpleKnob::SimpleKnob() :
		EnhancedComponent(),
		m_listeners(std::vector<SimpleKnob::Listener *>()),
		m_value(0.0),
		m_min(-10.0),
		m_max(10.0),
		m_suffix(""),
		m_int(true),
		m_bounded(true) {

}

/*SimpleKnob::SimpleKnob(double min, double max, bool bounded, std::string suffix, bool isInt) :
		EnhancedComponent(),
		m_listeners(std::vector<SimpleKnob::Listener *>()),
		m_value(0.0),
		m_min(min),
		m_max(max),
		m_suffix(suffix),
		m_int(isInt),
		m_bounded(bounded) {

}*/

SimpleKnob::~SimpleKnob() {

}

void SimpleKnob::mouseDown(const MouseEvent& event) {
	m_prevMousePos = event.getPosition();
}

void SimpleKnob::mouseDrag(const MouseEvent& event) {
	double delta = event.getPosition().getY() - m_prevMousePos.getY();
	delta *= (m_max - m_min) * C::MOUSE_MULT;
	m_prevMousePos = event.getPosition();
	m_value -= delta;
	if (m_bounded)
		m_value = std::max(std::min(m_value, m_max), m_min);
	if (delta != 0)
		for (SimpleKnob::Listener * listener : m_listeners)
			listener->simpleKnobChanged(this);
	repaint();
}

void SimpleKnob::paint(Graphics& g) {
	int w = getWidth(), h = getHeight();
	g.setColour(BACK_LAYER);
	g.fillEllipse(0, 0, w, h);
	double angle = getAngle();
	if (m_bounded) //Slightly shrink angle, so -1 and 1 do not look the same.
	{
		angle *= 0.96;
		angle -= M_PI * 0.04;
	}
	double x = (std::sin(angle) * w * 0.5) + (w * 0.5), y = (std::cos(angle) * h * 0.5) + (h * 0.5);
	g.setColour(FORE_LAYER);
	g.drawLine(x, y, w * 0.5, h * 0.5, C::LINE_SIZE);
	g.setColour(MID_LAYER);
	if (m_bounded) //Draw a barrier between -1 and 1.
		g.drawLine(w / 2, h / 2, w / 2, h, C::LINE_SIZE);
	g.fillEllipse(C::LINE_SIZE, C::LINE_SIZE, w - C::LINE_SIZE * 2, h - C::LINE_SIZE * 2);
	y = h / 2;
	y -= 15;
	g.setColour(BACK_LAYER);
	g.setFont(C::FONT_SIZE);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(m_int ? 0 : 2) << m_value;
	g.drawFittedText(stream.str() + m_suffix, 0, 0, w, h, Justification::centred, 1, C::MIN_TEXT_SCALE);
}

void SimpleKnob::loadSaveState(SaveState state) {
	m_value = state.getValue(0);
}

SaveState SimpleKnob::saveSaveState() {
	SaveState state = SaveState();
	state.addValue(m_value);
	return state;
}

BpmMultiple::BpmMultiple() :
		EnhancedComponent(),
		m_numerator(1.0f),
		m_denominator(1.0f),
		m_editingTop(false) {

}

BpmMultiple::~BpmMultiple() {

}

void BpmMultiple::checkBounds() {
	if (m_numerator > MAX_VALUE)
		m_numerator = MAX_VALUE;
	else if (m_numerator < 0.0)
		m_numerator = 0;

	if (m_denominator > MAX_VALUE)
		m_denominator = MAX_VALUE;
	else if (m_denominator < 1.0)
		m_denominator = 1.0;
}

void BpmMultiple::mouseDown(const MouseEvent & event) {
	m_prevMousePos = event.getPosition();
	m_editingTop = (event.getMouseDownY() <= (getHeight() / 2));
}

void BpmMultiple::mouseDrag(const MouseEvent & event) {
	double delta = event.getPosition().getY() - m_prevMousePos.getY();
	delta /= -MOUSE_DIVISOR;
	if (m_editingTop) {
		delta *= (m_numerator + SLOPE_MODIFIER);
		m_numerator = m_numerator + delta;
	} else {
		delta *= (m_denominator + SLOPE_MODIFIER);
		m_denominator = m_denominator + delta;
	}
	for (Listener * listener : m_listeners)
		listener->bpmMultipleChanged(this);
	m_prevMousePos = event.getPosition();
	checkBounds();
	repaint();
}

void BpmMultiple::paint(Graphics & g) {
	double w = getWidth(), h = getHeight() / 2;
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, w, h * 2, C::CORNER_SIZE);
	g.setColour(MID_LAYER);
	g.fillRoundedRectangle(C::LINE_SIZE, C::LINE_SIZE, w - C::LINE_SIZE * 2, h - C::LINE_SIZE * 2 + (C::LINE_SIZE / 2), C::CORNER_SIZE);
	g.fillRoundedRectangle(C::LINE_SIZE, h + C::LINE_SIZE / 2, w - C::LINE_SIZE * 2, h - C::LINE_SIZE * 2 + (C::LINE_SIZE / 2), C::CORNER_SIZE);
	g.setColour(BACK_LAYER);
	g.setFont(C::LARGE_FONT);
	g.drawFittedText(std::to_string(getNumerator()), 0, 0, int(w), int(h), Justification::centred, 1, 1.0);
	g.drawFittedText(std::to_string(getDenominator()), 0, int(h), int(w), int(h), Justification::centred, 1, 1.0);
	return;
}

double BpmMultiple::getSeconds() {
	return getFraction() * (60.0f / Synth::getInstance()->getParameters().m_bpm);
}

void BpmMultiple::loadSaveState(SaveState state) {
	m_numerator = state.getValue(0);
	m_denominator = state.getValue(1);
}

SaveState BpmMultiple::saveSaveState() {
	SaveState state = SaveState();
	state.addValue(m_numerator);
	state.addValue(m_denominator);
	return state;
}

ImageButton::ImageButton() :
		Component() {
	m_icon = Image(Image::ARGB, 1, 1, true);
}

ImageButton::~ImageButton() {

}

void ImageButton::paint(Graphics& g) {
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), C::CORNER_SIZE);
	g.drawImageAt(m_icon, 0, 0, false);
}

void ImageButton::mouseDown(const MouseEvent& event) {
	for (Listener * listener : m_listeners) {
		listener->imageButtonPressed(this);
	}
}

void ImageButton::setIcon(std::string iconName) {
	int size = 0;
	m_iconName = iconName;
	iconName += "_png";
	const char * data = BinaryData::getNamedResource(iconName.c_str(), size);
	m_icon = ImageCache::getFromMemory(data, size);
	repaint();
}

void GuiImage::paint(Graphics & g) {
	g.drawImageAt(m_image, 0, 0, false);
}

void GuiImage::setImage(std::string resourceName) {
	resourceName += "_png";
	int size = 0;
	const char * data = BinaryData::getNamedResource(resourceName.c_str(), size);
	m_image = ImageCache::getFromMemory(data, size);
	setSize(m_image.getWidth(), m_image.getHeight());
	repaint();
}

void IndicatorLight::paint(Graphics & g) {
	g.setColour(BACK_LAYER.interpolatedWith(m_colour, m_level));
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), C::CORNER_SIZE);
}

void OnOffButton::paint(Graphics & g) {
	g.setColour((m_on) ? m_colour : BACK_LAYER);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), C::CORNER_SIZE);
	g.setColour((m_on) ? FORE_LAYER : MID_LAYER);
	g.drawFittedText((m_on) ? m_onLabel : m_offLabel, C::SMALL_SPACING, C::SMALL_SPACING, getWidth() - C::SMALL_SPACING * 2, getHeight() - C::SMALL_SPACING, Justification::centred, 1, C::MIN_TEXT_SCALE);
}

void OnOffButton::mouseDown(const MouseEvent&) {
	m_on = !m_on;
	for (Listener * listener : m_listeners) {
		listener->onOffButtonPressed(this);
	}
	repaint();
}

SaveState OnOffButton::saveSaveState() {
	SaveState toReturn = SaveState();
	toReturn.addValue((m_on) ? 1.0 : 0.0);
	return toReturn;
}

void OnOffButton::loadSaveState(SaveState state) {
	m_on = state.getValue(0) == 1.0;
}

const bool allowed[128] = { false, false, false, false, false, false, false, false, false, false,	//^@ - ^I
		false, false, false, false, false, false, false, false, false, false,	//^J - ^S
		false, false, false, false, false, false, false, false, false, false,	//^T - ^]
		false, false, true, true, true, true, true, true, true, true,	//^^ - ' //and a space and a double quote in there somewhere
		true, true, true, true, true, true, true, false, true, true,	// ( ) * + , - . / 0 1
		true, true, true, true, true, true, true, true, true, true,	//2 - 9 : ;
		true, true, true, true, true, true, true, true, true, true,	//< - @ A B C D E
		true, true, true, true, true, true, true, true, true, true,	//F - O
		true, true, true, true, true, true, true, true, true, true,	//P - Y
		true, true, false, true, true, true, true, true, true, true,	//Z [ \ ] ^ _ ` a b c
		true, true, true, true, true, true, true, true, true, true,	//d - m
		true, true, true, true, true, true, true, true, true, true,	//n - w
		true, true, true, true, true, true, true, false,					//x - z { | } ~ DEL
		};

bool TextEntry::canType(char c) {
	return allowed[c];
}

TextEntry::TextEntry() :
		EnhancedComponent(),
		m_text("") {
	setMouseClickGrabsKeyboardFocus(true);
	setWantsKeyboardFocus(true);
}

void TextEntry::mouseDown(const MouseEvent & e) {
	grabKeyboardFocus();
	repaint();
}

void TextEntry::paint(Graphics& g) {
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.0);
	g.setColour(FORE_LAYER);
	g.drawFittedText(m_text + ((hasKeyboardFocus(false)) ? "|" : ""), 0, 0, getWidth(), getHeight(), Justification::centred, 1, 1.0);
}

bool TextEntry::keyPressed(const KeyPress & key) {
	char c = (char) key.getTextCharacter();
	if ((c == 8) || (c == 127)) //bksp / del
			{
		m_text = m_text.substr(0, m_text.size() - 1);
	} else if (canType(c)) {
		m_text += ((char) key.getTextCharacter());
	}
	for (Listener * listener : m_listeners) {
		listener->textEntryChanged(this);
	}
	repaint();
	return true;
}

void TextEntry::setText(std::string text, bool notify) {
	m_text = text;
	if (notify) {
		for (Listener * listener : m_listeners) {
			listener->textEntryChanged(this);
		}
	}
	repaint();
}

bool NumberEntry::canType(char c) {
	//Numbers, a decimal point, or a negative sign, in their respective places.
	return ((c >= '0') && (c <= '9')) || ((c == '.') && (getText().find('.') == std::string::npos)) || ((c == '-') && (getText().size() == 0));
}

double NumberEntry::getNumber() {
	return atof(getText().c_str());
}

void Rectangle::paint(Graphics & g) {
	g.setColour(m_colour);
	g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), (m_rounded) ? C::CORNER_SIZE : 0);
}

bool NumberEntry::keyPressed(const KeyPress& key) {
	bool tr = TextEntry::keyPressed(key);
	for (Listener * listener : m_listeners) {
		listener->numberEntryChanged(this);
	}
	return tr;
}

Arrow::Arrow() {
}

void Arrow::setCaps(int cap1, int cap2) {
	m_cap1 = cap1;
	m_cap2 = cap2;
}

void Arrow::paint(Graphics& g) {
	g.setColour(FORE_LAYER);
	constexpr int C2 = C::CELL_SIZE / 2, LS = C::LINE_SIZE;
	if(getWidth() >= getHeight()) {
		g.drawLine(C2, C2, getWidth() - C2, C2, LS);
		switch(m_cap1) {
		case LINE_CAP:
			g.drawLine(0, C2, C2, C2, LS);
			break;
		case ARROW_CAP:
			g.drawArrow(Line<float>(C2, C2, 0, C2), LS, LS * 3, LS * 3);
			break;
		case POINT_CAP:
			g.fillEllipse(C2 - LS, C2 - LS, LS * 2, LS * 2);
			break;
		}

		switch(m_cap2) {
		case LINE_CAP:
			g.drawLine(getWidth() - C2, C2, getWidth(), C2, LS);
			break;
		case ARROW_CAP:
			g.drawArrow(Line<float>(getWidth() - C2, C2, getWidth(), C2), LS, LS * 3, LS * 3);
			break;
		case POINT_CAP:
			g.fillEllipse(getWidth() - C2 - LS, C2 - LS, LS * 2, LS * 2);
			break;
		}
	} else {
		g.drawLine(C2, C2, C2, getHeight() - C2, LS);
		switch(m_cap1) {
		case LINE_CAP:
			g.drawLine(C2, 0, C2, C2, LS);
			break;
		case ARROW_CAP:
			g.drawArrow(Line<float>(C2, C2, C2, 0), LS, LS * 3, LS * 3);
			break;
		case POINT_CAP:
			g.fillEllipse(C2 - LS, C2 - LS, LS * 2, LS * 2);
			break;
		}

		switch(m_cap2) {
		case LINE_CAP:
			g.drawLine(C2, getHeight() - C2, C2, getHeight(), LS);
			break;
		case ARROW_CAP:
			g.drawArrow(Line<float>(C2, getHeight() - C2, C2, getHeight()), LS, LS * 3, LS * 3);
			break;
		case POINT_CAP:
			g.fillEllipse(C2 - LS, getHeight() - C2 - LS, LS * 2, LS * 2);
			break;
		}
	}
}

} /* namespace AtomSynth */
