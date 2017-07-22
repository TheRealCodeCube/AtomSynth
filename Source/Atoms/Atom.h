/*
 * Atom.h
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#ifndef SOURCE_ATOM_H_
#define SOURCE_ATOM_H_

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include "Gui/AtomGui.h"
#include "Technical/AudioBuffer.h"
#include "Technical/GlobalNoteStates.h"
#include "Technical/Log.h"

#define ATOM_TIME ((double(sample) / m_sampleRate_f) + m_time)
//These are helper macros to create various controls and a corresponding iterator automatically.
#define CONTROL(NAME, INAME, TYPE) TYPE m_##NAME; std::vector<double>::iterator m_##INAME
#define KNOB(NAME) CONTROL(NAME##Knob, NAME##Iter, AtomKnob)
#define CKNOB(NAME) CONTROL(NAME##Knob, NAME##Iter, CentsKnob)
#define SKNOB(NAME) CONTROL(NAME##Knob, NAME##Iter, SemitonesKnob)
#define OKNOB(NAME) CONTROL(NAME##Knob, NAME##Iter, OctavesKnob)
#define SLIDER(NAME) CONTROL(NAME##Slider, NAME##Iter, AtomSlider)
//These are helper macros that are wrappers for the addAutomatedControl method.
#define ADD_KNOB(NAME) addAutomatedControl(m_##NAME##Knob, m_##NAME##Iter)
#define ADD_SLIDER(NAME) addAutomatedControl(m_##NAME##Slider, m_##NAME##Iter)

namespace AtomSynth
{

#define AUTOMATION_INPUTS 10

class Atom;
class AtomController;
class AtomKnob;
class SaveState;

typedef std::vector<double>::iterator DVecIter;

//Basic parameters that are shared by atoms and atom controllers.
struct AtomParameters
{
	int m_numPrimaryInputs, m_numOutputs, m_id;
	bool m_automationEnabled;

	AtomParameters(int id, int numPrimaryInputs, bool automationEnabled, int numOutputs);
	AtomParameters withId(int newId);
};

struct AtomControllerPreview
{
	std::function<AtomController * ()> m_createFunction;
	std::string m_name;
	AtomParameters m_parameters {0, 0, false, 0};
};

//Atom controller class
//An atom controller is a single instance of some part of the synth.
//E.G. a single envelope is an atom controller.
//An atom controller contains several child 'atoms', each responsible for a different polyphony.
//The controller does no synthesizing. It only contains the GUI and manages child atoms.
class AtomController
{
private:
	std::vector<std::pair<AtomController *, int>> m_primaryInputs;
	std::vector<std::pair<AtomController *, int>> m_automationInputs;
	std::vector<Atom *> m_atoms;
	AtomParameters m_parameters;
	int m_x, m_y;
	bool m_stopped, m_shouldBeDeleted;
protected:
	AtomGui m_gui;
	AutomationSet m_automation;
	void init();
	std::vector<Atom *> & getAtoms() { return m_atoms; }
	void addAutomatedControl(AutomatedControl & knob, DVecIter & iterator) { m_automation.add(knob, iterator); }
public:
	//Construct an atom from basic parameters.
	AtomController(AtomParameters parameters);
	virtual AtomController * createNewInstance() { return nullptr; }
	virtual ~AtomController();

	virtual Atom * createAtom(int index);
	virtual Atom * getAtom(int index);

	virtual void loadSaveState(SaveState state);
	virtual SaveState saveSaveState();

	virtual std::string getCategory() { return "ungrouped"; }
	virtual std::string getName() { return ""; }

	void linkPrimaryInput(int index, AtomController * controller, int outputIndex);
	void linkAutomationInput(int index, AtomController * controller, int outputIndex);
	void linkInput(int index, AtomController * controller, int outputIndex);
	void unlinkPrimaryInput(int index);
	void unlinkAutomationInput(int index);
	void unlinkInput(int index);

	//Returns the number of primary inputs to this atom
	//(E.G. two for a sidechain compressor)
	int getNumPrimaryInputs() { return m_parameters.m_numPrimaryInputs; }
	//Returns the number of inputs used for automation
	//(If automation is disabled, this returns 0)
	int getNumAutomationInputs() { return (getAutomationEnabled()) ? AUTOMATION_INPUTS : 0; }
	//Returns the total number of inputs
	//(primary inputs + automation inputs)
	int getNumInputs() { return getNumPrimaryInputs() + getNumAutomationInputs(); }
	//Gets the number of primary outputs of this atom
	//(E.G. oscillator has 1)
	int getNumOutputs() { return m_parameters.m_numOutputs; }
	//Gets the ID for this type of atom
	//(E.G. envelope atoms might have an id of 4)
	int getId() { return m_parameters.m_id; }
	//Gets whether or not automation is enabled.
	//(If the atom has knobs which can be animated, this should return true)
	bool getAutomationEnabled() { return m_parameters.m_automationEnabled; }
	//Returns the complete set of parameters for this atom controller
	AtomParameters getParameters() { return m_parameters; }
	//Gets where a particular primary input is pointing.
	//Returns a pair. The first element is the atom controller the input links to.
	//The second element is the index of the output it is connected to.
	std::pair<AtomController *, int> getPrimaryInput(int index) { return m_primaryInputs[index]; }
	//Gets where a particular automation input is pointing.
	//Returns a pair. The first element is the atom controller the input links to.
	//The second element is the index of the output it is connected to.
	std::pair<AtomController *, int> getAutomationInput(int index) { return m_automationInputs[index]; }
	//Gets where a particular input is pointing.
	//Returns a pair. The first element is the atom controller the input links to.
	//The second element is the index of the output it is connected to.
	std::pair<AtomController *, int> getInput(int index);
	//Returns a vector containing both the primary and automation inputs.
	//Has to construct a vector during execution, so do not call this often.
	std::vector<std::pair<AtomController *, int> > getAllInputs();

	//Should trigger the execution of child atoms.
	virtual void execute();
	virtual void stopControlAnimation();

	void setPosition(int x, int y) { m_x = x; m_y = y; }
	AtomGui & getGui() { return m_gui; }
	int getX() { return m_x; }
	int getY() { return m_y; }

	void markForDeletion() { m_shouldBeDeleted = true; }
	bool getIsMarkedForDeletion() { return m_shouldBeDeleted; }
	void cleanupInputsFromAtom(AtomController * source);

	friend class Atom;
};

//Contains iterators for inputs and outputs of an atom, used in execute().
//Usage is similar to AutomationSet.
class IOSet
{
private:
	std::vector<AudioBuffer*> m_constInputSources, m_incInputSources, m_outputSources;
	std::vector<DVecIter*> m_constInputs, m_incInputs, m_outputs;
	//typedef std::vector<double>::iterator DVecIter;
public:
	IOSet() { }
	virtual ~IOSet() { }

	void clear();
	DVecIter* addInput(AudioBuffer * input);
	DVecIter& addOutput(AudioBuffer& output);

	void resetPosition();
	void incrementPosition();
	void incrementChannel();
};

//Atom class
//Represents a particular polyphony of an atom controller.
//E.G. with 8x polyphony, there will be 8 envelope atoms for 1 envelope controller
//This has no gui, only audio processing.
class Atom
{
private:
	AtomController & m_p;
	int m_updateTimer;
	AtomParameters m_parameters;
	std::vector<AudioBuffer *> m_automationInputs;
protected:
	int m_sampleRate;
	double m_sampleRate_f;
	bool m_shouldUpdateParent;
	std::vector<AudioBuffer *> m_primaryInputs;
	std::vector<AudioBuffer> m_outputs;

	void linkInput(int index, AudioBuffer * buffer);
	void linkPrimaryInput(int index, AudioBuffer * buffer) { m_primaryInputs[index] = buffer; }
	void linkAutomationInput(int index, AudioBuffer * buffer) { m_automationInputs[index] = buffer; }
	void unlinkInput(int index);
	void unlinkPrimaryInput(int index) { m_primaryInputs[index] = nullptr; }
	void unlinkAutomationInput(int index) { m_automationInputs[index] = nullptr; }
public:
	Atom(AtomController & parent, int index);
	virtual ~Atom();

	AudioBuffer * getOutput(int index) { return & m_outputs[index]; }
	int getIndex() { return m_parameters.m_id; }
	int getId() { return m_parameters.m_id; }

	virtual void execute();
	virtual void reset() { m_shouldUpdateParent = getId() == 0; };

	friend class AutomatedControl;
	friend class AtomController;
};

} /* namespace AtomSynth */

#endif /* SOURCE_ATOM_H_ */
