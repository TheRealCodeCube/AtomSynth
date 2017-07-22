/*
 * AnimatedWidgets.cpp
 *
 *  Created on: Mar 30, 2017
 *      Author: josh
 */

#include <iomanip>
#include <sstream>

#include "AnimatedWidgets.h"
#include "Colours.h"
#include "Technical/Automation.h"
#include "Technical/Synth.h"

namespace AtomSynth {

AutomationEditor::AutomationEditor() :
		m_editing(nullptr),
		m_px(0),
		m_py(0),
		m_influence(0),
		m_dragging(Dragging::NOTHING) {
	setSize(WIDTH, HEIGHT);
}

void AutomationEditor::paint(Graphics& g) {
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, WIDTH, HEIGHT, TAB_RADIUS);

	g.setColour(MID_LAYER);
	for (int i = 0; i < 4; i++) {
		AutomationInfluence inf = m_editing->getInfluence(i);
		if (inf.m_inputIndex == -1) {
			for (int c = 0; c < AUTOMATION_INPUTS; c++) {
				g.setColour(ATOM_COLOURS[c + 2]);
				g.fillRect(TAB_SPACING + TAB_RADIUS + (c * TAB_WIDTH), TAB_SPACING + (i * LANE_HEIGHT), TAB_WIDTH, TAB_HEIGHT);
			}

			g.setColour(ATOM_COLOURS[2]);
			g.fillEllipse(TAB_SPACING, TAB_SPACING + (i * LANE_HEIGHT), TAB_DIAMETER, TAB_DIAMETER);
			g.setColour(ATOM_COLOURS[11]);
			g.fillEllipse(WIDTH - TAB_SPACING - TAB_DIAMETER, TAB_SPACING + (i * LANE_HEIGHT), TAB_DIAMETER, TAB_DIAMETER);
		} else {
			g.setColour(MID_LAYER);
			g.fillRoundedRectangle(TAB_SPACING, TAB_SPACING + (i * LANE_HEIGHT), LANE_WIDTH, TAB_HEIGHT, TAB_RADIUS);
			g.setColour(ATOM_COLOURS[inf.m_inputIndex + 2]);
			int low = inf.m_minRange * (LANE_WIDTH - (TAB_RADIUS * 2)) + TAB_SPACING + TAB_RADIUS, high = inf.m_maxRange * (LANE_WIDTH - (TAB_RADIUS * 2)) + TAB_SPACING + TAB_RADIUS;
			if (high < low) {
				g.fillEllipse(low - TAB_RADIUS, TAB_SPACING + (i * LANE_HEIGHT), TAB_RADIUS * 2, TAB_RADIUS * 2);
				g.fillEllipse(high - TAB_RADIUS, TAB_SPACING + (i * LANE_HEIGHT), TAB_RADIUS * 2, TAB_RADIUS * 2);
				g.fillRect(high, TAB_SPACING + (i * LANE_HEIGHT) + ((TAB_HEIGHT - SKINNY_HEIGHT) / 2), low - high, SKINNY_HEIGHT);
			} else {
				g.fillRoundedRectangle(low - TAB_RADIUS, TAB_SPACING + (i * LANE_HEIGHT), high - low + TAB_DIAMETER, TAB_DIAMETER, TAB_RADIUS);
			}
			g.setColour(FORE_LAYER);
			g.fillEllipse(low - HANDLE_RADIUS, TAB_SPACING + (i * LANE_HEIGHT) + ((TAB_HEIGHT - HANDLE_DIAMETER) / 2), HANDLE_DIAMETER, HANDLE_DIAMETER);
			g.fillEllipse(high - HANDLE_RADIUS, TAB_SPACING + (i * LANE_HEIGHT) + ((TAB_HEIGHT - HANDLE_DIAMETER) / 2), HANDLE_DIAMETER, HANDLE_DIAMETER);
		}
	}
}

void AutomationEditor::mouseDown(const MouseEvent & e) {
	int x = e.getMouseDownPosition().getX(), y = e.getMouseDownPosition().getY();
	y -= TAB_SPACING;
	y /= LANE_HEIGHT;
	if (y > 3) {
		y = 3;
	}

	if (ModifierKeys::getCurrentModifiersRealtime().isRightButtonDown()) {
		m_editing->getInfluence(y).m_inputIndex = -1;
		repaint();
		m_editing->repaint();
	} else {
		if (m_editing->getInfluence(y).m_inputIndex == -1) {
			//Clicked on one of the rainbow colors
			x -= TAB_SPACING + TAB_RADIUS;
			x /= TAB_WIDTH;
			if (x < 0) {
				x = 0;
			} else if (x > AUTOMATION_INPUTS - 1) {
				x = AUTOMATION_INPUTS - 1;
			}
			m_editing->getInfluence(y).m_inputIndex = x;
			m_editing->getInfluence(y).m_minRange = 0.0;
			m_editing->getInfluence(y).m_maxRange = 1.0;
			repaint();
			m_editing->repaint();

			m_dragging = Dragging::NOTHING;
		} else {
			m_influence = y;
			if (abs(x - TAB_SPACING - m_editing->getInfluence(y).m_maxRange * LANE_WIDTH) <= TAB_RADIUS + GRACE_RANGE) {
				m_dragging = Dragging::MAX;
			} else if (abs(x - TAB_SPACING - m_editing->getInfluence(y).m_minRange * LANE_WIDTH) <= TAB_RADIUS + GRACE_RANGE) {
				m_dragging = Dragging::MIN;
			} else {
				m_dragging = Dragging::BOTH;
			}
		}
	}
	m_px = e.getPosition().getX();
	m_py = e.getPosition().getY();
}

void AutomationEditor::mouseDrag(const MouseEvent & e) {
	double delta = e.getPosition().getX() - m_px;
	m_px = e.getPosition().getX();
	switch (m_dragging) {
	case Dragging::MIN:
		m_editing->getInfluence(m_influence).m_minRange += (delta / (LANE_WIDTH - TAB_RADIUS * 2));
		break;
	case Dragging::BOTH:
		m_editing->getInfluence(m_influence).m_minRange += (delta / (LANE_WIDTH - TAB_RADIUS * 2));
		m_editing->getInfluence(m_influence).m_maxRange += (delta / (LANE_WIDTH - TAB_RADIUS * 2));
		break;
	case Dragging::MAX:
		m_editing->getInfluence(m_influence).m_maxRange += (delta / (LANE_WIDTH - TAB_RADIUS * 2));
		break;
	}

	if (m_editing->getInfluence(m_influence).m_minRange > 1.0) {
		m_editing->getInfluence(m_influence).m_minRange = 1.0;
	} else if (m_editing->getInfluence(m_influence).m_minRange < 0.0) {
		m_editing->getInfluence(m_influence).m_minRange = 0.0;
	}

	if (m_editing->getInfluence(m_influence).m_maxRange > 1.0) {
		m_editing->getInfluence(m_influence).m_maxRange = 1.0;
	} else if (m_editing->getInfluence(m_influence).m_maxRange < 0.0) {
		m_editing->getInfluence(m_influence).m_maxRange = 0.0;
	}

	repaint();
}

AtomKnob::ThisRightClickMenu::ThisRightClickMenu(AtomKnob & editing) :
		RightClickMenu(),
		m_editing(editing),
		m_dragging(-2),
		m_influence(0) {

	m_previousPos = editing.getPosition();
	m_previousParent = editing.getParentComponent();
	m_previousParent->repaint();
	setTopLeftPosition(editing.getScreenX() - editing.getTopLevelComponent()->getScreenX() - DS_SIZE - C::SPACING, editing.getScreenY() - editing.getTopLevelComponent()->getScreenY() - DS_SIZE - C::SPACING);
	editing.setTopLeftPosition(DS_SIZE + C::SPACING, DS_SIZE + C::SPACING);
	m_previousSize = editing.getWidth();
	editing.setSize(C::KNOB_SIZE, C::KNOB_SIZE);
	addAndMakeVisible(editing);
	if (getX() < -(DS_SIZE + C::SPACING))
		setTopLeftPosition(getPosition().withX(-(DS_SIZE + C::SPACING)));
	if (getY() < -(DS_SIZE + C::SPACING))
		setTopLeftPosition(getPosition().withY(-(DS_SIZE + C::SPACING)));
	setSize(WIDTH, HEIGHT);

	addAndMakeVisible(m_mixModeSelector);
	m_mixModeSelector.setBounds(MIXMODE_X, TOP_PART_Y, CS(2), CS(2));
	m_mixModeSelector.addLabel("Average");
	m_mixModeSelector.addLabel("Multiply");
	m_mixModeSelector.addLabel("Minimum");
	m_mixModeSelector.addLabel("Maximum");
	m_mixModeSelector.setSelectedLabel(static_cast<int>(editing.getMixMode()));
	m_mixModeSelector.setVertical();
	m_mixModeSelector.addListener(this);

	addAndMakeVisible(m_automationEditor);
	m_automationEditor.setTopLeftPosition(BOX_X, BOX_Y);
	m_automationEditor.setEditing(&m_editing);
}

AtomKnob::ThisRightClickMenu::~ThisRightClickMenu() {
	m_editing.setTopLeftPosition(m_previousPos);
	m_previousParent->addAndMakeVisible(m_editing);
	m_editing.setMenuClosed();
	m_editing.setSize(m_previousSize, m_previousSize);
	m_previousParent->repaint();
}

void AtomKnob::ThisRightClickMenu::multiButtonPressed(MultiButton * b) {
	m_editing.setMixMode((MixMode) b->getSelectedLabel());
}

AtomKnob::AtomKnob() :
		AutomatedControl(),
		EnhancedComponent(),
		m_menuOpened(false),
		m_dragging(false) {

}

AtomKnob::~AtomKnob() {
	// TODO Auto-generated destructor stub
}

void AtomKnob::mouseDown(const MouseEvent& event) {
	if (ModifierKeys::getCurrentModifiersRealtime().isRightButtonDown()) {
		if (m_menuOpened) {
			Synth::s_instance->closeRightClickMenu();
			m_menuOpened = false;
		} else {
			ThisRightClickMenu * m = new ThisRightClickMenu(*this);
			//RightClickMenu * m = new RightClickMenu();
			//m->setBounds(0, 0, 100, 100);
			Synth::s_instance->openRightClickMenu(m);
			m_menuOpened = true;
		}
		m_dragging = false;
	} else {
		m_prevMousePos = event.getMouseDownPosition();
		m_dragging = true;
	}
}

void AtomKnob::mouseDrag(const MouseEvent& event) {
	if (!m_dragging)
		return;
	double offset = event.getPosition().getY() - m_prevMousePos.getY();
	m_prevMousePos = event.getPosition();
	offset *= C::MOUSE_MULT;
	offset *= -(getMax() - getMin());
	offsetValue(offset);
}

void AtomKnob::paint(Graphics & g) {
	int w = getWidth(), h = getHeight();
	g.setColour(BACK_LAYER);
	g.fillEllipse(0, 0, w, h);
	double angle = getAngle();
	if (isBounded()) //Slightly shrink angle, so -1 and 1 do not look the same.
	{
		angle *= ANGLE_REDUCTION;
		angle -= M_PI * (1.0 - ANGLE_REDUCTION);
	}
	double x = (std::sin(angle) * w / 2) + (w / 2), y = (std::cos(angle) * h / 2) + (h / 2);
	g.setColour(MID_LAYER);
	if (isBounded()) //Draw a barrier between -1 and 1.
		g.drawLine(w / 2, h / 2, w / 2, h, C::LINE_SIZE);
	g.setColour(FORE_LAYER);
	g.drawLine(x, y, w / 2, h / 2, C::LINE_SIZE);
	g.setColour(MID_LAYER);
	g.fillEllipse(C::LINE_SIZE, C::LINE_SIZE, w - C::LINE_SIZE * 2, h - C::LINE_SIZE * 2);

	x = w / 2;
	y = h - Y_OFFSET;

	if (getWidth() >= C::KNOB_SIZE) {
		if (getInfluence(0).m_inputIndex == -1)
			g.setColour(BACK_LAYER);
		else
			g.setColour(ATOM_COLOURS[getInfluence(0).m_inputIndex + 2]);
		g.fillEllipse(x - X_SPACING * 1.5 - AI_SIZE / 2, y - Y_SPACING, AI_SIZE, AI_SIZE);

		if (getInfluence(1).m_inputIndex == -1)
			g.setColour(BACK_LAYER);
		else
			g.setColour(ATOM_COLOURS[getInfluence(1).m_inputIndex + 2]);
		g.fillEllipse(x - X_SPACING * 0.5 - AI_SIZE / 2, y, AI_SIZE, AI_SIZE);

		if (getInfluence(2).m_inputIndex == -1)
			g.setColour(BACK_LAYER);
		else
			g.setColour(ATOM_COLOURS[getInfluence(2).m_inputIndex + 2]);
		g.fillEllipse(x + X_SPACING * 0.5 - AI_SIZE / 2, y, AI_SIZE, AI_SIZE);

		if (getInfluence(3).m_inputIndex == -1)
			g.setColour(BACK_LAYER);
		else
			g.setColour(ATOM_COLOURS[getInfluence(3).m_inputIndex + 2]);
		g.fillEllipse(x + X_SPACING * 1.5 - AI_SIZE / 2, y - Y_SPACING, AI_SIZE, AI_SIZE);

		y = h / 2;
		y -= C::LARGE_FONT_SIZE;
		g.setColour(BACK_LAYER);
		g.setFont(C::LARGE_FONT);
		g.drawFittedText(getFormattedValue(), 0, 0, w, h, Justification::centred, 1, C::MIN_TEXT_SCALE);
	}
}

double AtomKnob::getAngle() {
	return -(((getDisplayValue() - getMin()) / (getMax() - getMin())) * M_PI * 2);
}

AtomSlider::ThisRightClickMenu::ThisRightClickMenu(AtomSlider & editing) :
		RightClickMenu(),
		m_editing(editing),
		m_dragging(-2),
		m_influence(0),
		m_previousParent(nullptr) {

	/*m_previousPos = editing.getPosition();
	 m_previousParent = editing.getParentComponent();
	 m_previousParent->repaint();
	 setTopLeftPosition(editing.getScreenX() - editing.getTopLevelComponent()->getScreenX() - DS_SIZE - C::SPACING, editing.getScreenY() - editing.getTopLevelComponent()->getScreenY() - DS_SIZE - C::SPACING);
	 editing.setTopLeftPosition(DS_SIZE + C::SPACING, DS_SIZE + C::SPACING);
	 addAndMakeVisible(editing);
	 if(getX() < -(DS_SIZE + C::SPACING)) setTopLeftPosition(getPosition().withX(-(DS_SIZE + C::SPACING)));
	 if(getY() < -(DS_SIZE + C::SPACING)) setTopLeftPosition(getPosition().withY(-(DS_SIZE + C::SPACING)));

	 addAndMakeVisible(& m_mixModeSelector);
	 //m_mixModeSelector.setBounds(MIXMODE_X, TOP_PART_Y, CS(2), CS(2));
	 m_mixModeSelector.addLabel("AVG");
	 m_mixModeSelector.addLabel("MULT");
	 m_mixModeSelector.addLabel("MIN");
	 m_mixModeSelector.addLabel("MAX");
	 m_mixModeSelector.setVertical();
	 m_mixModeSelector.addListener(this);*/
}

AtomSlider::ThisRightClickMenu::~ThisRightClickMenu() {
	/*m_editing.setTopLeftPosition(m_previousPos);
	 m_previousParent->addAndMakeVisible(m_editing);
	 m_editing.setMenuClosed();
	 m_previousParent->repaint();*/
}

void AtomSlider::ThisRightClickMenu::multiButtonPressed(MultiButton * b) {
	/*m_editing.m_mixMode = (MixMode) b->getSelectedLabel();*/
}

AtomSlider::AtomSlider() :
		AutomatedControl(),
		m_menuOpened(false),
		m_dragging(false),
		m_leftLevel(0.0),
		m_rightLevel(0.0),
		m_px(0),
		m_py(0) {

}

void AtomSlider::paint(Graphics& g) {
	if (getHeight() > getWidth()) {
		double w = getWidth(), h = getHeight();
		g.setColour(BACK_LAYER);
		g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), C::CORNER_SIZE);
		g.setFont(C::LARGE_FONT);
		if (m_leftLevel + m_rightLevel != 0.0) {
			g.setColour(GREEN);
			constexpr int CS2 = C::SPACING * 2;
			double height = floor(m_leftLevel * (h - CS2));
			g.fillRoundedRectangle(0, h - height - CS2, w / 2, height + CS2, C::CORNER_SIZE);
			g.fillRect(w / 2 - C::CORNER_SIZE, h - height - CS2, C::CORNER_SIZE, height + CS2);
			height = floor(m_rightLevel * (h - CS2));
			g.fillRoundedRectangle(w / 2, h - height - CS2, w / 2, height + CS2, C::CORNER_SIZE);
			g.fillRect(w / 2, h - height - CS2, C::CORNER_SIZE, height + CS2);
		}
		g.saveState();
		//I'm not sure *why* this is the correct transform / bounding box, but it works.
		g.addTransform(AffineTransform::identity.rotated(M_PI * -0.5, w, w));
		g.setColour(MID_LAYER);
		g.drawFittedText(getFormattedValue(), 0, 0, w * 2 - C::SPACING, w, Justification::centredRight, 1, C::MIN_TEXT_SCALE);
		//Because there's no g.clearTransform()?!
		g.restoreState();
		g.setColour(FORE_LAYER);
		double y = getDisplayValue() - getMin();
		y = 1 - y / (getMax() - getMin()); //0 (min) - 1 (max)
		y = floor(y * (h - UNUSABLE_SPACE)) + C::SPACING;
		g.fillRoundedRectangle(C::SPACING, y, HANDLE_WIDTH, HANDLE_HEIGHT, C::CORNER_SIZE);
	}
}

void AtomSlider::mouseDown(const MouseEvent& event) {
	m_px = event.getPosition().getX();
	m_py = event.getPosition().getY();
	m_dragging = !ModifierKeys::getCurrentModifiersRealtime().isRightButtonDown();
}

void AtomSlider::mouseDrag(const MouseEvent& event) {
	if (!m_dragging)
		return;
	double offset = event.getPosition().getY() - m_py;
	m_px = event.getPosition().getX();
	m_py = event.getPosition().getY();
	offset /= getHeight() - HANDLE_HEIGHT - UNUSABLE_SPACE;
	offset *= -(getMax() - getMin());
	offsetValue(offset);
}

DetuneKnob::DetuneKnob(int range, std::string suffix) :
		AtomKnob() {
	setSuffix(suffix);
	setInt(true);
	setBounded(false);
	setRange(-range, range);
}

DetuneKnob::~DetuneKnob() {

}

CentsKnob::CentsKnob() :
		DetuneKnob(100.0, "c") {

}

CentsKnob::~CentsKnob() {

}

SemitonesKnob::SemitonesKnob() :
		DetuneKnob(12.0, "s") {

}

SemitonesKnob::~SemitonesKnob() {

}

OctavesKnob::OctavesKnob() :
		DetuneKnob(4.0, "o") {

}

OctavesKnob::~OctavesKnob() {

}

PlotBase::PlotBase() :
		EnhancedComponent(),
		m_xLines(9),
		m_yLines(9),
		m_cursorX(0),
		m_cursorY(0),
		m_cursorMode(CursorMode::NONE) {

}

PlotBase::~PlotBase() {
	// TODO Auto-generated destructor stub
}

void PlotBase::paintBg(Graphics & g) {
	double w = getWidth(), h = getHeight();
	g.setColour(BACK_LAYER);
	g.fillRoundedRectangle(0, 0, w, h, C::CORNER_SIZE);
	g.setColour(MID_LAYER);
	double xo = (w + 1.0f) / double(m_xLines + 1), yo = (h + 1.0f) / double(m_yLines + 1);
	for (int x = 0; x < m_xLines; x++)
		g.drawLine((x + 1) * xo, 0, (x + 1) * xo, h);
	for (int y = 0; y < m_yLines; y++)
		g.drawLine(0, (y + 1) * yo, w, (y + 1) * yo);
}

void PlotBase::paintFg(Graphics & g) {
	g.setColour(FORE_LAYER);
	switch (m_cursorMode) {
	case CursorMode::CROSSHAIR:
		g.fillRect(0, m_cursorY - (C::SMALL_LINE_SIZE / 2), getWidth(), C::SMALL_LINE_SIZE);
	case CursorMode::VERTICAL:
		g.fillRect(m_cursorX - (C::SMALL_LINE_SIZE / 2), 0, C::SMALL_LINE_SIZE, getHeight());
	}
}

void PlotBase::clear() {
	repaintAsync();
}

void PlotBase::finish() {
	repaintAsync();
}

void PlotBase::setCursorPos(double x) {
	m_cursorX = x * getWidth();
	repaintAsync();
}

void PlotBase::setCursorPos(double x, double y, int padx, int pady) {
	m_cursorX = x * (getWidth() - padx * 2) + padx;
	m_cursorY = y * (getHeight() - pady * 2) + pady;
	repaintAsync();
}

void PlotBase::setCursorPosAbsolute(int x, int y) {
	m_cursorX = x;
	m_cursorY = y;
	repaintAsync();
}

void GenericPlot::paint(Graphics & g) {
	paintBg(g);
	g.setColour(FORE_LAYER);
	if (m_values.size() > 1) {
		for (int i = 1; i < m_values.size(); i++) {
			g.drawLine(m_values[i - 1].first, m_values[i - 1].second, m_values[i].first, m_values[i].second, C::SMALL_LINE_SIZE);
		}
	}
	paintFg(g);
}

void GenericPlot::addPoint(double x, double y) {
	m_values.push_back(std::pair<double, double>(x, y));
}

void GenericPlot::clear() {
	m_values.clear();
	PlotBase::clear();
}

WaveformPlot::WaveformPlot() :
		PlotBase() {

}

WaveformPlot::~WaveformPlot() {
	// TODO Auto-generated destructor stub
}

void WaveformPlot::paint(Graphics& g) {
	paintBg(g);
	double w = getWidth(), h = getHeight();
	if (m_values.size() != 0) {
		double xo = w / m_values.size();
		g.setColour(FORE_LAYER);
		for (int index = 1; index < m_values.size(); index++)
			g.drawLine((index - 1) * xo, (-m_values[index - 1] + 1.0) / 2.0 * h, index * xo, (-m_values[index] + 1.0) / 2.0 * h, 2.0);
	}
	paintFg(g);
}

void WaveformPlot::addPoint(double value) {
	m_values.push_back(value);
}

void WaveformPlot::setDataFromAudioBuffer(AudioBuffer& source, bool clip) {
	m_values.clear();
	int values;
	if (clip) //Clip = clip to range of display
	{
		values = std::min(getWidth() + 1, source.getSize());
	} else {
		values = source.getSize();
	}

	for (int i = 0; i < values; i++) {
		m_values.push_back(source.getValue(0, i));
	}
	repaintAsync();
}

void WaveformPlot::clear() {
	m_values.clear();
	PlotBase::clear();
}

} /* namespace AtomSynth */
