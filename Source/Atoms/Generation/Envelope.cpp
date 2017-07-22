/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[12.000000:1.000000:1.000000:sEnvelope:sGeneration:stime:senv:[[[4.000000:9.000000:6.500000:2.000000:2.000000:sBpmMultiple:sreleaseTime:sRelease:]][1.000000:2.000000:]][[[4.000000:7.000000:6.500000:2.000000:2.000000:sBpmMultiple:ssustainTime:sSustain:]][0.000000:2.000000:]][[[4.000000:5.000000:6.500000:2.000000:2.000000:sBpmMultiple:sholdTime:sHold:]][1.000000:2.000000:]][[[4.000000:3.000000:6.500000:2.000000:2.000000:sBpmMultiple:sattackTime:sAttack:]][0.000000:2.000000:]][[[4.000000:1.000000:6.500000:2.000000:2.000000:sBpmMultiple:sdelayTime:sDelay:]][0.000000:2.000000:]][[[4.000000:9.000000:9.000000:2.000000:2.000000:sAtomKnob:sreleaseMult:sMultiplier:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:7.000000:14.000000:2.000000:2.000000:sAtomKnob:ssustainLevel:sLevel:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:9.000000:11.500000:2.000000:2.000000:sAtomKnob:sreleaseShape:sShape:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:3.000000:14.000000:2.000000:2.000000:sAtomKnob:sattackLevel:sLevel:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:3.000000:11.500000:2.000000:2.000000:sAtomKnob:sattackShape:sShape:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:7.000000:11.500000:2.000000:2.000000:sAtomKnob:ssustainShape:sShape:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:5.000000:14.000000:2.000000:2.000000:sAtomKnob:sholdLevel:sLevel:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:1.000000:9.000000:2.000000:2.000000:sAtomKnob:sdelayMult:sMultiplier:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:5.000000:9.000000:2.000000:2.000000:sAtomKnob:sholdMult:sMultiplier:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:7.000000:9.000000:2.000000:2.000000:sAtomKnob:ssustainMult:sMultiplier:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:3.000000:9.000000:2.000000:2.000000:sAtomKnob:sattackMult:sMultiplier:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]]]]
/* BEGIN AUTO-GENERATED INCLUDES */
#include "Envelope.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
#include "Adsp/interpolation.h"
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */
EnvelopePlot::EnvelopePlot() :
		PlotBase(),
		m_cursorPos(0.0),
		m_cursorHeight(0.0),
		m_parent(nullptr),
		m_selectedHandle(HANDLE::NONE),
		m_px(0),
		m_py(0) {
	setLines(0, 0);
	setCursorMode(CursorMode::CROSSHAIR);
}

EnvelopePlot::~EnvelopePlot() {

}

void EnvelopePlot::paint(Graphics & g) {
	paintBg(g);
	double delayPercent = m_parent->m_delayTime.getFraction() * m_parent->m_delayMult.getDisplayValue(),
			attackPercent = m_parent->m_attackTime.getFraction() * m_parent->m_attackMult.getDisplayValue(),
			holdPercent = m_parent->m_holdTime.getFraction() * m_parent->m_holdMult.getDisplayValue(),
			sustainPercent = m_parent->m_sustainTime.getFraction() * m_parent->m_sustainMult.getDisplayValue(),
			releasePercent = m_parent->m_releaseTime.getFraction() * m_parent->m_releaseMult.getDisplayValue(),
			total = delayPercent + attackPercent + holdPercent + sustainPercent + releasePercent,
			o = (C::LINE_SIZE + C::HANDLE_SIZE) / 2.0, w = getWidth() - o * 2.0, h = getHeight() - o * 2.0;
	delayPercent /= total / w;
	attackPercent /= total / w;
	holdPercent /= total / w;
	sustainPercent /= total / w;
	releasePercent /= total / w;
	double attackStart = delayPercent, holdStart = attackStart + attackPercent, sustainStart = holdStart + holdPercent, releaseStart = sustainStart + sustainPercent;
	double holdLevel = (1.0 - m_parent->m_holdLevel.getDisplayValue()) * h + o, sustainLevel = (1.0 - m_parent->m_sustainLevel.getDisplayValue()) * h + o;
	g.setColour(FORE_LAYER);
	double y = 0.0, py = h + o, px = attackStart + o;
	if (attackPercent >= 2.0) {
		for (double dx = 0; dx <= 1.0; dx += 1.0 / attackPercent) {
			y = Adsp::fastEnvelopeInterp(dx, m_parent->m_attackShape.getDisplayValue()) * m_parent->m_holdLevel.getDisplayValue();
			y = (1.0 - y) * (h) + o;
			g.drawLine(px - 1.0, py, px, y, C::LINE_SIZE);
			py = y;
			px++;
		}
		g.drawLine(px - 1.0, py, px, holdLevel, C::LINE_SIZE);
	} else {
		g.drawLine(attackStart + o, h + o, attackStart + attackPercent + o, holdLevel, C::LINE_SIZE);
	}
	g.drawLine(holdStart + o, holdLevel, holdStart + holdPercent + o, holdLevel, C::LINE_SIZE);
	py = holdLevel;
	px = sustainStart + o;
	if (sustainPercent >= 2.0) {
		for (double dx = 0; dx <= 1.0; dx += 1.0 / sustainPercent) {
			y = (1.0 - Adsp::fastEnvelopeInterp(1.0 - dx, m_parent->m_sustainShape.getDisplayValue())) * (sustainLevel - holdLevel) + holdLevel;
			g.drawLine(px - 1.0, py, px, y, C::LINE_SIZE);
			py = y;
			px++;
		}
		g.drawLine(px, py, px, sustainLevel, C::LINE_SIZE);
	} else {
		g.drawLine(sustainStart + o, holdLevel, sustainStart + sustainPercent + o, sustainLevel, C::LINE_SIZE);
	}
	py = sustainLevel;
	px = releaseStart + o;
	if (releasePercent >= 2.0) {
		for (double dx = 0; dx <= 1.0; dx += 1.0 / releasePercent) {
			y = (1.0 - Adsp::fastEnvelopeInterp(1.0 - dx, m_parent->m_releaseShape.getDisplayValue())) * (h - sustainLevel + o) + sustainLevel;
			g.drawLine(px - 1.0, py, px, y, C::LINE_SIZE);
			py = y;
			px++;
		}
		g.drawLine(px, py, px, h + o, C::LINE_SIZE);
	} else {
		g.drawLine(releaseStart + o, sustainLevel, releaseStart + releasePercent + o, h + o, C::LINE_SIZE);
	}
	double ho = C::LINE_SIZE / 2.0;
	g.fillEllipse(attackStart + ho, h + ho, C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(holdStart + ho, holdLevel - C::HANDLE_SIZE / 2.0, C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(sustainStart + ho, holdLevel - C::HANDLE_SIZE / 2.0, C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(releaseStart + ho, sustainLevel - C::HANDLE_SIZE / 2.0, C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(w + ho, h + ho, C::HANDLE_SIZE, C::HANDLE_SIZE);

	g.fillEllipse(attackStart + (attackPercent / 2) + ho, h + ho - (Adsp::fastEnvelopeInterp(0.5, m_parent->m_attackShape.getDisplayValue()) * (h - holdLevel + o)), C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(sustainStart + (sustainPercent / 2) + ho, ho + (Adsp::fastEnvelopeInterp(0.5, -m_parent->m_sustainShape.getDisplayValue()) * (sustainLevel - holdLevel) + holdLevel - o), C::HANDLE_SIZE, C::HANDLE_SIZE);
	g.fillEllipse(releaseStart + (releasePercent / 2) + ho, ho + (Adsp::fastEnvelopeInterp(0.5, -m_parent->m_releaseShape.getDisplayValue()) * (h - sustainLevel + o) + sustainLevel - o), C::HANDLE_SIZE, C::HANDLE_SIZE);
	paintFg(g);
}

void EnvelopePlot::mouseDown(const MouseEvent& e) {
	double delayPercent = m_parent->m_delayTime.getFraction() * m_parent->m_delayMult.getDisplayValue(),
			attackPercent = m_parent->m_attackTime.getFraction() * m_parent->m_attackMult.getDisplayValue(),
			holdPercent = m_parent->m_holdTime.getFraction() * m_parent->m_holdMult.getDisplayValue(),
			sustainPercent = m_parent->m_sustainTime.getFraction() * m_parent->m_sustainMult.getDisplayValue(),
			releasePercent = m_parent->m_releaseTime.getFraction() * m_parent->m_releaseMult.getDisplayValue(),
			total = delayPercent + attackPercent + holdPercent + sustainPercent + releasePercent,
			o = (C::LINE_SIZE + C::HANDLE_SIZE) / 2.0, w = getWidth() - o * 2.0, h = getHeight() - o * 2.0;
	delayPercent /= total / w;
	attackPercent /= total / w;
	holdPercent /= total / w;
	sustainPercent /= total / w;
	releasePercent /= total / w;
	double attackStart = delayPercent, holdStart = attackStart + attackPercent, sustainStart = holdStart + holdPercent, releaseStart = sustainStart + sustainPercent;
	double holdLevel = (1.0 - m_parent->m_holdLevel.getDisplayValue()) * h + o, sustainLevel = (1.0 - m_parent->m_sustainLevel.getDisplayValue()) * h + o;

	m_px = e.getPosition().getX();
	m_py = e.getPosition().getY();

	m_selectedHandle = HANDLE::NONE;
	if ((fabs(m_px - attackStart) <= C::HANDLE_SIZE) && (fabs(m_py - o - h) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::ATK_START;
	else if ((fabs(m_px - holdStart) <= C::HANDLE_SIZE) && (fabs(m_py - holdLevel) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::HOLD_START;
	else if ((fabs(m_px - sustainStart) <= C::HANDLE_SIZE) && (fabs(m_py - holdLevel) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::HOLD_END;
	else if ((fabs(m_px - releaseStart) <= C::HANDLE_SIZE) && (fabs(m_py - sustainLevel) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::REL_START;
	else if ((fabs(m_px - o - w) <= C::HANDLE_SIZE) && (fabs(m_py - o - h) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::END;
	else if (((fabs(m_px - attackStart - attackPercent / 2)) <= C::HANDLE_SIZE) &&
			(fabs(m_py - (h - (Adsp::fastEnvelopeInterp(0.5, m_parent->m_attackShape.getDisplayValue()) * (h - holdLevel + o)))) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::ATK_SHAPE;
	else if (((fabs(m_px - sustainStart - sustainPercent / 2)) <= C::HANDLE_SIZE) &&
			(fabs(m_py - (Adsp::fastEnvelopeInterp(0.5, -m_parent->m_sustainShape.getDisplayValue()) * (sustainLevel - holdLevel) + holdLevel - o)) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::SUST_SHAPE;
	else if (((fabs(m_px - releaseStart - releasePercent / 2)) <= C::HANDLE_SIZE) &&
			(fabs(m_py - ((Adsp::fastEnvelopeInterp(0.5, -m_parent->m_releaseShape.getDisplayValue()) * (h - sustainLevel + o) + sustainLevel - o))) <= C::HANDLE_SIZE))
		m_selectedHandle = HANDLE::REL_SHAPE;
}

void EnvelopePlot::mouseDrag(const MouseEvent& e) {
	int x = e.getPosition().getX(), y = e.getPosition().getY();
	int threshold = 30, xdelta = 0, ydelta = y - m_py;
	if (x > m_px + threshold) {
		xdelta = 1;
		m_px += threshold;
	} else if (x < m_px - threshold) {
		xdelta = -1;
		m_px -= threshold;
	}

	switch (m_selectedHandle) {
	case HANDLE::ATK_START: {
		m_parent->m_delayTime.offsetNumerator(xdelta);
		break;
	}

	case HANDLE::ATK_SHAPE: {
		m_parent->m_attackShape.offsetValue(double(ydelta) / getHeight() * 3.0);
		break;
	}

	case HANDLE::HOLD_START: {
		m_parent->m_attackTime.offsetNumerator(xdelta);
		m_parent->m_holdLevel.offsetValue(-double(ydelta) / getHeight());
		break;
	}

	case HANDLE::HOLD_END: {
		m_parent->m_holdTime.offsetNumerator(xdelta);
		m_parent->m_holdLevel.offsetValue(-double(ydelta) / getHeight());
		break;
	}

	case HANDLE::SUST_SHAPE: {
		m_parent->m_sustainShape.offsetValue(double(ydelta) / getHeight() * 3.0);
		break;
	}

	case HANDLE::REL_START: {
		m_parent->m_sustainTime.offsetNumerator(xdelta);
		m_parent->m_sustainLevel.offsetValue(-double(ydelta) / getHeight());
		break;
	}

	case HANDLE::REL_SHAPE: {
		m_parent->m_releaseShape.offsetValue(double(ydelta) / getHeight() * 3.0);
		break;
	}

	case HANDLE::END: {
		m_parent->m_releaseTime.offsetNumerator(xdelta);
		break;
	}
	}
	m_py = y;
	repaint();
}
/* END MISC. USER-DEFINED CODE */

EnvelopeController::EnvelopeController() :
		AtomController(AtomParameters(12, 1, true, 1)) {
	init();

	m_gui.addComponent(&m_releaseTime);
	m_releaseTime.setBounds(CB(9.000000, 6.500000, 2.000000, 2.000000));
	m_releaseTime.addListener(this);
	m_releaseTime.setFraction(1, 2);
	m_gui.addComponent(m_releaseTime.createLabel("Release", true));

	m_gui.addComponent(&m_sustainTime);
	m_sustainTime.setBounds(CB(7.000000, 6.500000, 2.000000, 2.000000));
	m_sustainTime.addListener(this);
	m_sustainTime.setFraction(0, 2);
	m_gui.addComponent(m_sustainTime.createLabel("Sustain", true));

	m_gui.addComponent(&m_holdTime);
	m_holdTime.setBounds(CB(5.000000, 6.500000, 2.000000, 2.000000));
	m_holdTime.addListener(this);
	m_holdTime.setFraction(1, 2);
	m_gui.addComponent(m_holdTime.createLabel("Hold", true));

	m_gui.addComponent(&m_attackTime);
	m_attackTime.setBounds(CB(3.000000, 6.500000, 2.000000, 2.000000));
	m_attackTime.addListener(this);
	m_attackTime.setFraction(0, 2);
	m_gui.addComponent(m_attackTime.createLabel("Attack", true));

	m_gui.addComponent(&m_delayTime);
	m_delayTime.setBounds(CB(1.000000, 6.500000, 2.000000, 2.000000));
	m_delayTime.addListener(this);
	m_delayTime.setFraction(0, 2);
	m_gui.addComponent(m_delayTime.createLabel("Delay", true));

	m_gui.addComponent(&m_releaseMult);
	m_releaseMult.setBounds(CB(9.000000, 9.000000, 2.000000, 2.000000));
	m_releaseMult.addListener(this);
	m_releaseMult.setRange(0.000000, 1.000000);
	m_releaseMult.setValue(1.000000);
	m_releaseMult.setBounded(true);
	addAutomatedControl(m_releaseMult, m_releaseMultIter);
	m_gui.addComponent(m_releaseMult.createLabel("Multiplier", true));

	m_gui.addComponent(&m_sustainLevel);
	m_sustainLevel.setBounds(CB(7.000000, 14.000000, 2.000000, 2.000000));
	m_sustainLevel.addListener(this);
	m_sustainLevel.setRange(0.000000, 1.000000);
	m_sustainLevel.setValue(1.000000);
	m_sustainLevel.setBounded(true);
	addAutomatedControl(m_sustainLevel, m_sustainLevelIter);
	m_gui.addComponent(m_sustainLevel.createLabel("Level", true));

	m_gui.addComponent(&m_releaseShape);
	m_releaseShape.setBounds(CB(9.000000, 11.500000, 2.000000, 2.000000));
	m_releaseShape.addListener(this);
	m_releaseShape.setBounded(true);
	addAutomatedControl(m_releaseShape, m_releaseShapeIter);
	m_gui.addComponent(m_releaseShape.createLabel("Shape", true));

	m_gui.addComponent(&m_attackLevel);
	m_attackLevel.setBounds(CB(3.000000, 14.000000, 2.000000, 2.000000));
	m_attackLevel.addListener(this);
	m_attackLevel.setRange(0.000000, 1.000000);
	m_attackLevel.setValue(1.000000);
	m_attackLevel.setBounded(true);
	addAutomatedControl(m_attackLevel, m_attackLevelIter);
	m_gui.addComponent(m_attackLevel.createLabel("Level", true));

	m_gui.addComponent(&m_attackShape);
	m_attackShape.setBounds(CB(3.000000, 11.500000, 2.000000, 2.000000));
	m_attackShape.addListener(this);
	m_attackShape.setBounded(true);
	addAutomatedControl(m_attackShape, m_attackShapeIter);
	m_gui.addComponent(m_attackShape.createLabel("Shape", true));

	m_gui.addComponent(&m_sustainShape);
	m_sustainShape.setBounds(CB(7.000000, 11.500000, 2.000000, 2.000000));
	m_sustainShape.addListener(this);
	m_sustainShape.setBounded(true);
	addAutomatedControl(m_sustainShape, m_sustainShapeIter);
	m_gui.addComponent(m_sustainShape.createLabel("Shape", true));

	m_gui.addComponent(&m_holdLevel);
	m_holdLevel.setBounds(CB(5.000000, 14.000000, 2.000000, 2.000000));
	m_holdLevel.addListener(this);
	m_holdLevel.setRange(0.000000, 1.000000);
	m_holdLevel.setValue(1.000000);
	m_holdLevel.setBounded(true);
	addAutomatedControl(m_holdLevel, m_holdLevelIter);
	m_gui.addComponent(m_holdLevel.createLabel("Level", true));

	m_gui.addComponent(&m_delayMult);
	m_delayMult.setBounds(CB(1.000000, 9.000000, 2.000000, 2.000000));
	m_delayMult.addListener(this);
	m_delayMult.setRange(0.000000, 1.000000);
	m_delayMult.setValue(1.000000);
	m_delayMult.setBounded(true);
	addAutomatedControl(m_delayMult, m_delayMultIter);
	m_gui.addComponent(m_delayMult.createLabel("Multiplier", true));

	m_gui.addComponent(&m_holdMult);
	m_holdMult.setBounds(CB(5.000000, 9.000000, 2.000000, 2.000000));
	m_holdMult.addListener(this);
	m_holdMult.setRange(0.000000, 1.000000);
	m_holdMult.setValue(1.000000);
	m_holdMult.setBounded(true);
	addAutomatedControl(m_holdMult, m_holdMultIter);
	m_gui.addComponent(m_holdMult.createLabel("Multiplier", true));

	m_gui.addComponent(&m_sustainMult);
	m_sustainMult.setBounds(CB(7.000000, 9.000000, 2.000000, 2.000000));
	m_sustainMult.addListener(this);
	m_sustainMult.setRange(0.000000, 1.000000);
	m_sustainMult.setValue(1.000000);
	m_sustainMult.setBounded(true);
	addAutomatedControl(m_sustainMult, m_sustainMultIter);
	m_gui.addComponent(m_sustainMult.createLabel("Multiplier", true));

	m_gui.addComponent(&m_attackMult);
	m_attackMult.setBounds(CB(3.000000, 9.000000, 2.000000, 2.000000));
	m_attackMult.addListener(this);
	m_attackMult.setRange(0.000000, 1.000000);
	m_attackMult.setValue(1.000000);
	m_attackMult.setBounded(true);
	addAutomatedControl(m_attackMult, m_attackMultIter);
	m_gui.addComponent(m_attackMult.createLabel("Multiplier", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_gui.addComponent(&m_plot); //This needs to be initialized last.
	m_plot.setParent(this);
	m_plot.setBounds(CB(0.000000, 0.000000, 12.000000, 6.000000));
	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * EnvelopeController::createAtom(int index) {
	return new EnvelopeAtom(*this, index);
}

SaveState EnvelopeController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_releaseTime.saveSaveState());
	extraData.addState(m_sustainTime.saveSaveState());
	extraData.addState(m_holdTime.saveSaveState());
	extraData.addState(m_attackTime.saveSaveState());
	extraData.addState(m_delayTime.saveSaveState());
	extraData.addState(m_releaseMult.saveSaveState());
	extraData.addState(m_sustainLevel.saveSaveState());
	extraData.addState(m_releaseShape.saveSaveState());
	extraData.addState(m_attackLevel.saveSaveState());
	extraData.addState(m_attackShape.saveSaveState());
	extraData.addState(m_sustainShape.saveSaveState());
	extraData.addState(m_holdLevel.saveSaveState());
	extraData.addState(m_delayMult.saveSaveState());
	extraData.addState(m_holdMult.saveSaveState());
	extraData.addState(m_sustainMult.saveSaveState());
	extraData.addState(m_attackMult.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void EnvelopeController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if (version == 5) {
		m_releaseTime.loadSaveState(extraData.getNextState());
		m_sustainTime.loadSaveState(extraData.getNextState());
		m_holdTime.loadSaveState(extraData.getNextState());
		m_attackTime.loadSaveState(extraData.getNextState());
		m_delayTime.loadSaveState(extraData.getNextState());
		m_releaseMult.loadSaveState(extraData.getNextState());
		m_sustainLevel.loadSaveState(extraData.getNextState());
		m_releaseShape.loadSaveState(extraData.getNextState());
		m_attackLevel.loadSaveState(extraData.getNextState());
		m_attackShape.loadSaveState(extraData.getNextState());
		m_sustainShape.loadSaveState(extraData.getNextState());
		m_holdLevel.loadSaveState(extraData.getNextState());
		m_delayMult.loadSaveState(extraData.getNextState());
		m_holdMult.loadSaveState(extraData.getNextState());
		m_sustainMult.loadSaveState(extraData.getNextState());
		m_attackMult.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	} else if (version == 4) {
		m_releaseMult.loadSaveState(extraData.getNextState());
		m_sustainLevel.loadSaveState(extraData.getNextState());
		m_releaseShape.loadSaveState(extraData.getNextState());
		m_attackLevel.loadSaveState(extraData.getNextState());
		m_attackShape.loadSaveState(extraData.getNextState());
		m_sustainShape.loadSaveState(extraData.getNextState());
		m_holdLevel.loadSaveState(extraData.getNextState());
		m_delayMult.loadSaveState(extraData.getNextState());
		m_attackMult.loadSaveState(extraData.getNextState());
		m_holdMult.loadSaveState(extraData.getNextState());
		m_sustainMult.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void EnvelopeController::bpmMultipleChanged(BpmMultiple * multiple) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	m_plot.repaintAsync();
	/* END USER-DEFINED LISTENER CODE */
}

void EnvelopeController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	m_plot.repaintAsync();
	/* END USER-DEFINED LISTENER CODE */
}

EnvelopeAtom::EnvelopeAtom(EnvelopeController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_releaseStart = 0.0;
	m_releaseLevel.resize(AudioBuffer::getDefaultChannels(), 0.0);
	m_releasing.resize(AudioBuffer::getDefaultChannels(), false);
	/* END USER-DEFINED CONSTRUCTION CODE */
}

void EnvelopeAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & releaseMultIter = m_parent.m_releaseMultIter;
	DVecIter & sustainLevelIter = m_parent.m_sustainLevelIter;
	DVecIter & releaseShapeIter = m_parent.m_releaseShapeIter;
	DVecIter & attackLevelIter = m_parent.m_attackLevelIter;
	DVecIter & attackShapeIter = m_parent.m_attackShapeIter;
	DVecIter & sustainShapeIter = m_parent.m_sustainShapeIter;
	DVecIter & holdLevelIter = m_parent.m_holdLevelIter;
	DVecIter & delayMultIter = m_parent.m_delayMultIter;
	DVecIter & holdMultIter = m_parent.m_holdMultIter;
	DVecIter & sustainMultIter = m_parent.m_sustainMultIter;
	DVecIter & attackMultIter = m_parent.m_attackMultIter;

	IOSet io = IOSet();
	DVecIter * timeInput = io.addInput(m_primaryInputs[0]);
	DVecIter & envOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if (timeInput != nullptr) {
		double delayTime = m_parent.m_delayTime.getSeconds() * m_parent.m_delayMult.getDisplayValue();
		double attackTime = m_parent.m_attackTime.getSeconds() * m_parent.m_attackMult.getDisplayValue() + 0.001;
		double holdTime = m_parent.m_holdTime.getSeconds() * m_parent.m_holdMult.getDisplayValue();
		double sustainTime = m_parent.m_sustainTime.getSeconds() * m_parent.m_sustainMult.getDisplayValue();
		double releaseTime = m_parent.m_releaseTime.getSeconds() * m_parent.m_releaseMult.getDisplayValue();

		double time = 0.0, ltime = 0.0;
		bool triggerRelease = false;
		for (int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
			for (int s = 0; s < AudioBuffer::getDefaultSize(); s++) {
				time = (**timeInput);
				triggerRelease = (time < 0.0); //Negative time = key not being pressed, waiting for note to finish.
				time = fabs(time);
				if ((!triggerRelease) && m_releasing[c])
					m_releasing[c] = false;
				if (m_releasing[c]) {
					if ((time - m_releaseStart) < releaseTime) {
						ltime = time - m_releaseStart;
						ltime /= releaseTime;
						ltime = 1.0 - ltime; //go firom 0 - 1 to 1 - 0
						(*envOutput) = Adsp::fastEnvelopeInterp(ltime, *releaseShapeIter) * m_releaseLevel[c] - 1.0;
					} else {
						(*envOutput) = -1.0;
					}
				} else {
					if (time < delayTime) {
						(*envOutput) = -1.0;
					} else if (time < delayTime + attackTime) {
						ltime = time - delayTime; //Time window between 0s and [attack length] seconds
						ltime /= attackTime; //Between 0 and 1
						(*envOutput) = Adsp::fastEnvelopeInterp(ltime, *attackShapeIter) * (*holdLevelIter) * 2.0 - 1.0;
					} else if (time < delayTime + attackTime + holdTime) {
						(*envOutput) = (*holdLevelIter) * 2.0 - 1.0;
					} else if (time < delayTime + attackTime + holdTime + sustainTime) {
						ltime = time - (delayTime + attackTime + holdTime); //Time window between 0s and [sustain length] seconds
						ltime /= sustainTime; //Between 0 and 1
						ltime = 1.0 - ltime;
						(*envOutput) = Adsp::fastEnvelopeInterp(ltime, *sustainShapeIter) * 2.0 * (*holdLevelIter - *sustainLevelIter) - 1.0 + (*sustainLevelIter * 2);
					} else {
						(*envOutput) = (*sustainLevelIter) * 2.0 - 1.0;
					}

					if (triggerRelease) {
						m_releasing[c] = true;
						m_releaseStart = time;
						m_releaseLevel[c] = (*envOutput) + 1.0;
					}
				}
				automation.incrementPosition();
				io.incrementPosition();
			}
			automation.incrementChannel();
			io.incrementChannel();
		}

		if (m_shouldUpdateParent) {
			double cursorPos = 0.0;
			io.resetPosition(); //Get values from the start.
			cursorPos = (**timeInput);
			if (cursorPos >= 0.0) {
				cursorPos /= (delayTime + attackTime + holdTime + sustainTime);
				cursorPos = std::min(cursorPos, 1.0);
				cursorPos *= ((delayTime + attackTime + holdTime + sustainTime) / (delayTime + attackTime + holdTime + sustainTime + releaseTime));
			} else {
				cursorPos += m_releaseStart;
				cursorPos /= -(releaseTime);
				cursorPos = std::min(cursorPos, 1.0);
				cursorPos *= ((releaseTime) / (delayTime + attackTime + holdTime + sustainTime + releaseTime));
				cursorPos += ((delayTime + attackTime + holdTime + sustainTime) / (delayTime + attackTime + holdTime + sustainTime + releaseTime));
			}
			m_parent.m_plot.setCursorPos(cursorPos, ((*envOutput) - 1.0) / -2.0, C::CORNER_SIZE, C::CORNER_SIZE);
		}
	} else {
		m_outputs[0].fill(-1.0);
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void EnvelopeAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */

	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

