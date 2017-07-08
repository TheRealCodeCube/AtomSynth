/*
 * SimpleWidgets.h
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#ifndef SOURCE_GUI_SIMPLEWIDGETS_H_
#define SOURCE_GUI_SIMPLEWIDGETS_H_

#include "JuceLibraryCode/JuceHeader.h"
#include "Gui/Colours.h"
#include "Technical/GlobalNoteStates.h"

namespace AtomSynth
{

//Forward declarations
class SaveState;

class EnhancedComponent : public Component, private AsyncUpdater
{
private:
	void handleAsyncUpdate() { repaint(); }
public:
	Label * createLabel(std::string text, bool top = true);
	void repaintAsync() { triggerAsyncUpdate(); }
};

class RightClickMenu : public Component
{
private:
	bool m_firstPress;
public:
	static constexpr int
		DS_SIZE = 8,
		DS_CORNER = 3;

	RightClickMenu()
		: Component(),
		  m_firstPress(true)
	{ this->removeMouseListener(this); }
	virtual ~RightClickMenu() { Desktop::getInstance().removeGlobalMouseListener(this); }
	virtual void paint(Graphics & g);
};

class TextButton : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void textButtonPressed(TextButton * textButton) = 0;
	};
private:
	String m_string;
	std::vector<Listener *> m_listeners;
	bool m_flash;
public:
	TextButton() : EnhancedComponent(), m_flash(false) {};
	virtual ~TextButton() {};

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseUp(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void setText(std::string text) { m_string = String(text); repaint(); }
	std::string getText() { return m_string.toStdString(); }
};

class MultiButton : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void multiButtonPressed(MultiButton * multiButton) = 0;
	};
private:
	int m_selectedLabel;
	std::vector<std::string> m_labels;
	std::vector<Listener *> m_listeners;
	bool m_vertical;
public:
	MultiButton();
	virtual ~MultiButton();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void addLabel(std::string label) { m_labels.push_back(label); }
	void clearLabels() { m_labels.clear(); }
	void setSelectedLabel(int index);
	int getSelectedLabel() { return m_selectedLabel; }
	std::vector<std::string> & getLabels() { return m_labels; }
	void setVertical(bool vertical = true) { m_vertical = vertical; }
	bool isVertical() { return m_vertical; }

	void loadSaveState(SaveState state);
	SaveState saveSaveState();
};

class SimpleKnob : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() { };
		virtual void simpleKnobChanged(SimpleKnob * knob) = 0;
	};
private:
	Point<int> m_prevMousePos;
	std::vector<Listener *> m_listeners;
protected:
	double m_value, m_min, m_max;
	std::string m_suffix;
	bool m_int, m_bounded;
public:

	SimpleKnob();
	SimpleKnob(double min, double max, bool bounded = false, std::string suffix = "", bool isInt = true);
	virtual ~SimpleKnob();

	virtual void paint(Graphics & g);

	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void setRange(double min, double max) { m_min = min; m_max = max; }
	void setMin(double min) { m_min = min; }
	void setMax(double max) { m_max = max; }
	void setValue(double value) { m_value = value; }
	void setBounded(bool bounded) { m_bounded = bounded; }
	void setInt(bool isInt) { m_int = isInt; }
	void setSuffix(std::string suffix) { m_suffix = suffix; }

	double getMin() { return m_min; }
	double getMax() { return m_max; }
	std::string getSuffix() { return m_suffix; }
	bool isInt() { return m_int; }
	bool isBounded() { return m_bounded; }
	double getValue() { return (m_int) ? round(m_value) : m_value; }
	double getAngle() { return -(((getValue() - m_min) / (m_max - m_min)) * M_PI * 2.0); }

	void loadSaveState(SaveState state);
	SaveState saveSaveState();
};

class BpmMultiple : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void bpmMultipleChanged(BpmMultiple * bpmMultiple) = 0;
	};
private:
	double m_numerator, m_denominator;
	bool m_editingTop;
	std::vector<Listener *> m_listeners;
	Point<int> m_prevMousePos;
	void checkBounds();
public:
	static constexpr double
		MOUSE_DIVISOR = 300.0,
		SLOPE_MODIFIER = 20.0,
		MAX_VALUE = 999.0;

	BpmMultiple();
	virtual ~BpmMultiple();

	virtual void paint(Graphics & g);

	virtual void mouseDown(const MouseEvent & event);
	virtual void mouseDrag(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void setNumerator(int numerator) { m_numerator = numerator; checkBounds(); repaint(); }
	void setDenominator(int denominator) { m_denominator = denominator; checkBounds(); repaint(); }
	void setFraction(int numerator, int denominator) { m_numerator = numerator, m_denominator = denominator; checkBounds(); repaint(); }

	void offsetNumerator(int nd) { m_numerator += nd; checkBounds(); repaint(); }
	void offsetDenominator(int dd) { m_denominator += dd; checkBounds(); repaint(); }
	void offsetFraction(int nd, int dd) { m_numerator += nd, m_denominator += dd; checkBounds(); repaint(); }

	int getNumerator() { return floor(m_numerator); }
	int getDenominator() { return floor(m_denominator); }
	double getFraction() { return floor(m_numerator) / floor(m_denominator); }
	double getSeconds() { return getFraction() * (60.0f / GlobalNoteStates::s_bpm); } //How long it would take for 1 cycle to elapse.
	double getHertz() { return (m_numerator == 0) ? 0.0001 : 1.0f / getSeconds(); }

	void loadSaveState(SaveState state);
	SaveState saveSaveState();
};

class ImageButton : public Component
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void imageButtonPressed(ImageButton * button) = 0;
	};

private:
	Image m_icon;
	std::string m_iconName;
	std::vector<Listener *> m_listeners;
public:
	ImageButton();
	virtual ~ImageButton();

	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & event);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }

	void setIcon(std::string iconName);
	std::string getIconName() { return m_iconName; }
};

class GuiImage : public Component
{
private:
	Image m_image;
public:
	GuiImage() : Component() { }
	virtual ~GuiImage() { }

	virtual void paint(Graphics & g);

	void setImage(std::string resourceName);
};

class IndicatorLight : public EnhancedComponent
{
private:
	double m_level;
	Colour m_colour;
public:
	IndicatorLight() : EnhancedComponent(), m_level(0.0), m_colour(FORE_LAYER) { }
	virtual ~IndicatorLight() { }

	virtual void paint(Graphics &);

	void setColour(Colour colour) { m_colour = colour; repaint(); }
	Colour getColour() { return m_colour; }
	void setLevel(double level) { m_level = level; repaint(); }
};

class OnOffButton : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() { }
		virtual void onOffButtonPressed(OnOffButton *) = 0;
	};
private:
	Colour m_colour;
	std::string m_offLabel, m_onLabel;
	std::vector<Listener *> m_listeners;
	bool m_on;
public:
	OnOffButton() : EnhancedComponent(), m_on(false) { }
	virtual ~OnOffButton() { }

	virtual void paint(Graphics &);
	virtual void mouseDown(const MouseEvent &);

	void addListener(Listener * listener) { m_listeners.push_back(listener); }
	void setColour(Colour colour) { m_colour = colour; repaint(); }
	void setLabel(std::string label) { m_offLabel = label; m_onLabel = label; }
	void setOnLabel(std::string label) { m_onLabel = label; }
	void setOffLabel(std::string label) { m_offLabel = label; }
	void setLabels(std::string off, std::string on) { m_offLabel = off; m_onLabel = on; }

	Colour getColour() { return m_colour; }
	std::string getOnLabel() { return m_onLabel; }
	std::string getOffLabel() { return m_offLabel; }

	void setOn(bool on = true) { m_on = on; repaint(); }
	bool getOn() { return m_on; }

	SaveState saveSaveState();
	void loadSaveState(SaveState);
};

class TextEntry : public EnhancedComponent
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void textEntryChanged(TextEntry * textEntry) = 0;
	};
private:
	std::vector<Listener *> m_listeners;
	std::string m_text;
protected:
	virtual bool canType(char c);
public:
	TextEntry();
	virtual ~TextEntry() { }

	virtual void paint(Graphics & g);
	virtual bool keyPressed (const KeyPress & key);
	void addListener(Listener * listener) { m_listeners.push_back(listener); }
	std::string getText() { return m_text; }
	void setText(std::string text, bool notify = true);
	virtual void mouseDown(const MouseEvent & e);
	virtual void focusLost(FocusChangeType cause) { repaint(); }
};

class NumberEntry : public TextEntry
{
public:
	class JUCE_API Listener
	{
	public:
		virtual ~Listener() {};
		virtual void numberEntryChanged(NumberEntry * textEntry) = 0;
	};
private:
	std::vector<Listener *> m_listeners;
protected:
	virtual bool canType(char c);
public:
	NumberEntry() : TextEntry() { }
	virtual ~NumberEntry() { }

	double getNumber();
	void addListener(Listener * listener) { m_listeners.push_back(listener); }
	virtual bool keyPressed(const KeyPress & key);
};

class Rectangle : public EnhancedComponent
{
private:
	bool m_rounded;
	Colour m_colour;
public:
	Rectangle() :
		EnhancedComponent(),
		m_rounded(false),
		m_colour(MID_LAYER) {}
	virtual ~Rectangle() {}

	virtual void paint(Graphics & g);

	void setRounded(bool rounded) { m_rounded = rounded; repaint(); }
	void setColour(Colour colour) { m_colour = colour; repaint(); }
};

} /* namespace AtomSynth */

#endif /* SOURCE_GUI_SIMPLEWIDGETS_H_ */
