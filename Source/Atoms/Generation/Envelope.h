#ifndef ATOM_Envelope_HEADER
#define ATOM_Envelope_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */
#include "JuceLibraryCode/JuceHeader.h"

class EnvelopeController;

/**
 * A component for rendering the shape of an
 * envelope, as generated by EnvelopeController.
 * Also used for editing, by dragging points of
 * the rendered envelope.
 */
class EnvelopePlot: public PlotBase {
private:
	double m_cursorPos, m_cursorHeight;
	EnvelopeController * m_parent;
	enum class HANDLE {
		NONE, ATK_START, ATK_SHAPE, HOLD_START, HOLD_END, SUST_SHAPE, REL_START, REL_SHAPE, END
	};
	HANDLE m_selectedHandle;
	int m_px, m_py;
public:
	EnvelopePlot();
	virtual ~EnvelopePlot();
	/**
	 * Set the EnvelopeController from which this EnvelopePlot
	 * should get the parameters of the envelope shape.
	 * @param parent An EnvelopeController to get parameters from.
	 */
	void setParent(EnvelopeController * parent) {
		m_parent = parent;
	}
	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & e);
	virtual void mouseDrag(const MouseEvent & e);
};
/* END MISC. USER-DEFINED CODE */

/**
 * Takes a time (in seconds) as an input, and outputs an
 * envelope as defined by parameters in the GUI.
 */
class EnvelopeController: public AtomController, public AutomatedControl::Listener, public BpmMultiple::Listener {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	BpmMultiple m_releaseTime;
	BpmMultiple m_sustainTime;
	BpmMultiple m_holdTime;
	BpmMultiple m_attackTime;
	BpmMultiple m_delayTime;
	AtomKnob m_releaseMult;
	DVecIter m_releaseMultIter;
	AtomKnob m_sustainLevel;
	DVecIter m_sustainLevelIter;
	AtomKnob m_releaseShape;
	DVecIter m_releaseShapeIter;
	AtomKnob m_attackLevel;
	DVecIter m_attackLevelIter;
	AtomKnob m_attackShape;
	DVecIter m_attackShapeIter;
	AtomKnob m_sustainShape;
	DVecIter m_sustainShapeIter;
	AtomKnob m_holdLevel;
	DVecIter m_holdLevelIter;
	AtomKnob m_delayMult;
	DVecIter m_delayMultIter;
	AtomKnob m_holdMult;
	DVecIter m_holdMultIter;
	AtomKnob m_sustainMult;
	DVecIter m_sustainMultIter;
	AtomKnob m_attackMult;
	DVecIter m_attackMultIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	EnvelopePlot m_plot;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	EnvelopeController();
	virtual AtomController * createNewInstance() {
		return new EnvelopeController();
	}
	virtual ~EnvelopeController() {
	}

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() {
		return "Generation";
	}
	virtual std::string getName() {
		return "Envelope";
	}
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	/** Listener function. */
	virtual void bpmMultipleChanged(BpmMultiple * multiple);
	/** Listener function. */
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */
	friend class EnvelopePlot;
	/* END USER-DEFINED METHODS */

	friend class EnvelopeAtom;
};

/**
 * See EnvelopeController
 */
class EnvelopeAtom: public Atom {
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	EnvelopeController & m_parent;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	std::vector<double> m_releaseLevel;
	std::vector<bool> m_releasing;
	double m_releaseStart;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	/** Constructor which stores a more specific reference to the parent */
	EnvelopeAtom(EnvelopeController & parent, int index);
	virtual ~EnvelopeAtom() {
	}
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */

	/* END USER-DEFINED METHODS */

	friend class EnvelopeController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Envelope_HEADER */

