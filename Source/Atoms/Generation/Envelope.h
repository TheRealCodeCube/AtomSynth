#ifndef ATOM_Envelope_HEADER
#define ATOM_Envelope_HEADER

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Atoms/Atom.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */

/* END USER-DEFINED INCLUDES */

namespace AtomSynth
{

class SaveState;

/* BEGIN MISC. USER-DEFINED CODE */
#include "JuceLibraryCode/JuceHeader.h"

class EnvelopeController;

class EnvelopePlot: public PlotBase
{
private:
	double m_cursorPos, m_cursorHeight;
	EnvelopeController * m_parent;
	enum class HANDLE {NONE, ATK_START, ATK_SHAPE, HOLD_START, HOLD_END, SUST_SHAPE, REL_START, REL_SHAPE, END};
	HANDLE m_selectedHandle;
	int m_px, m_py;
public:
	EnvelopePlot();
	virtual ~EnvelopePlot();
	void setParent(EnvelopeController * parent) { m_parent = parent; }
	virtual void paint(Graphics & g);
	virtual void mouseDown(const MouseEvent & e);
	virtual void mouseDrag(const MouseEvent & e);
};
/* END MISC. USER-DEFINED CODE */

class EnvelopeController : public AtomController, public AutomatedControl::Listener, public BpmMultiple::Listener
{
private:
	/* BEGIN AUTO-GENERATED MEMBERS */
	BpmMultiple m_releaseTime;
	BpmMultiple m_sustainTime;
	BpmMultiple m_holdTime;
	BpmMultiple m_attackTime;
	BpmMultiple m_delayTime;
	AtomKnob m_releaseMult;
	std::vector<double>::iterator m_releaseMultIter;
	AtomKnob m_sustainLevel;
	std::vector<double>::iterator m_sustainLevelIter;
	AtomKnob m_releaseShape;
	std::vector<double>::iterator m_releaseShapeIter;
	AtomKnob m_attackLevel;
	std::vector<double>::iterator m_attackLevelIter;
	AtomKnob m_attackShape;
	std::vector<double>::iterator m_attackShapeIter;
	AtomKnob m_sustainShape;
	std::vector<double>::iterator m_sustainShapeIter;
	AtomKnob m_holdLevel;
	std::vector<double>::iterator m_holdLevelIter;
	AtomKnob m_delayMult;
	std::vector<double>::iterator m_delayMultIter;
	AtomKnob m_holdMult;
	std::vector<double>::iterator m_holdMultIter;
	AtomKnob m_sustainMult;
	std::vector<double>::iterator m_sustainMultIter;
	AtomKnob m_attackMult;
	std::vector<double>::iterator m_attackMultIter;
	/* END AUTO-GENERATED MEMBERS */

	/* BEGIN USER-DEFINED MEMBERS */
	EnvelopePlot m_plot;
	/* END USER-DEFINED MEMBERS */
public:
	/* BEGIN AUTO-GENERATED METHODS */
	EnvelopeController();
	virtual AtomController * createNewInstance() { return new EnvelopeController(); }
	virtual ~EnvelopeController() { }

	virtual Atom * createAtom(int index);
	virtual std::string getCategory() { return "Generation"; }
	virtual std::string getName() { return "Envelope"; }
	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN AUTO-GENERATED LISTENERS */
	virtual void bpmMultipleChanged(BpmMultiple * multiple);
	virtual void automatedControlChanged(AutomatedControl * control, bool byUser);
	/* END AUTO-GENERATED LISTENERS */

	/* BEGIN USER-DEFINED METHODS */
	friend class EnvelopePlot;
	/* END USER-DEFINED METHODS */

	friend class EnvelopeAtom;
};

class EnvelopeAtom : public Atom
{
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
	EnvelopeAtom(EnvelopeController & parent, int index);
	virtual ~EnvelopeAtom() { }
	virtual void execute();
	virtual void reset();
	/* END AUTO-GENERATED METHODS */

	/* BEGIN USER-DEFINED METHODS */
	static const double POSITIVE_SHAPE[101], NEGATIVE_SHAPE[101];
	static double calculate(double pos, double shape);
	/* END USER-DEFINED METHODS */

	friend class EnvelopeController;
};

} /* namespace AtomSynth */

#endif /*  ATOM_Envelope_HEADER */

