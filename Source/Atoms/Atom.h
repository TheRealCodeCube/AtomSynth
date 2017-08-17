/*
 * Atom.h
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#ifndef SOURCE_ATOMMM_H_
#define SOURCE_ATOMMM_H_

#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include "Gui/AtomGui.h"
#include "Technical/AudioBuffer.h"
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

namespace AtomSynth {

#define AUTOMATION_INPUTS 10

class Atom;
class AtomController;
class AtomKnob;
class SaveState;

typedef std::vector<double>::iterator DVecIter;

/**
 * Stores parameters of an atom.
 *
 * Stores how many inputs and outputs there are, as well as the ID
 * and whether or not automation inputs should be enabled. This is
 * used so that these do not have to be individual parameters in
 * the Atom and AtomController classes.
 */
struct AtomParameters {
	int m_numPrimaryInputs, ///< Number of primary inputs. These are the inputs that are directly accessible from Atom::execute()
		m_numOutputs, ///< Number of outputs. These are written to in Atom::execute()
		m_id; ///< A unique ID number. Each pair of AtomController and Atom classes needs its own unique ID.
	bool m_automationEnabled; ///< Whether or not automation inputs are enabled. (Set this to false if there is nothing in the atom to automate.)

	/**
	 * Initializes all values of the struct.
	 *
	 * @param id The unique ID number.
	 * @param numPrimaryInputs How many primary inputs the atom should have.
	 * @param automationEnabled Whether or not automation inputs should be enabled.
	 * @param numOutputs The number of outputs the atom should have.
	 */
	AtomParameters(int id, int numPrimaryInputs, bool automationEnabled, int numOutputs);

	/**
	 * Returns a copy of this object, except with the specified ID.
	 *
	 * @param newId The ID that the new object.
	 * @return The new object.
	 */
	AtomParameters withId(int newId);
};

//Atom controller class
//An atom controller is a single instance of some part of the synth.
//E.G. a single envelope is an atom controller.
//An atom controller contains several child 'atoms', each responsible for a different polyphony.
//The controller does no synthesizing. It only contains the GUI and manages child atoms.
/**
 * A single instance of some component in the synth.
 * (For example, an 'envelope' is an AtomController). Contains several child Atom objects,
 * each are responsible for rendering a single voice. (The synth uses around ~16 voices for
 * polyphony, so when multiple notes are played, individual voices handle each key.) The
 * AtomController handles global parameters, and the GUI / visual representation.
 */
class AtomController {
private:
	std::vector<std::pair<AtomController *, int>> m_primaryInputs;
	std::vector<std::pair<AtomController *, int>> m_automationInputs;
	std::vector<Atom *> m_atoms;
	std::vector<std::string> m_inputIcons, m_outputIcons;
	AtomParameters m_parameters;
	int m_x, m_y;
	bool m_stopped, m_shouldBeDeleted;
protected:
	AtomGui m_gui; ///< The gui that will be shown when this AtomController is clicked on. Add knobs and stuff like that to this.
	AutomationSet m_automation; ///< Holds all automated controls for easy management.
	/**
	 * Call this in the constructor to initialize child atoms.
	 */
	void init();
	/**
	 * Get the Atom objects this controller controls.
	 * @return A vector of all the atoms this controller controls.
	 */
	std::vector<Atom *> & getAtoms() {
		return m_atoms;
	}
	/**
	 * Add an automated control to the automation set, for ease of use during Atom::execute().
	 * Also see AutomationSet::add()
	 * @param knob The knob to be added to the automation set.
	 * @param iterator An iterator to link to the value of the knob.
	 */
	void addAutomatedControl(AutomatedControl & knob, DVecIter & iterator) {
		m_automation.add(knob, iterator);
	}
	/**
	 * Add an icon to be rendered on an input. Call this once
	 * for each input to set icons for all of them.
	 * @param iconName The name of the icon to render. The _png suffix is added for you.
	 */
	void addInputIcon(std::string iconName) {
		m_inputIcons.push_back(iconName + "_png");
	}
	/**
	 * Sets the icon for a particular input.
	 * @param index The index of the icon to set.
	 * @param iconName The name of the icon to render. The _png suffix is added for you.
	 */
	void setInputIcon(int index, std::string iconName) {
		m_inputIcons[index] = iconName + "_png";
	}
	/**
	 * Add an icon to be rendered on an output. Call this once
	 * for each output to set icons for all of them.
	 * @param iconName The name of the icon to render. The _png suffix is added for you.
	 */
	void addOutputIcon(std::string iconName) {
		m_outputIcons.push_back(iconName + "_png");
	}
	/**
	 * Sets the icon for a particular output.
	 * @param index The index of the icon to set.
	 * @param iconName The name of the icon to render. The _png suffix is added for you.
	 */
	void setOutputIcon(int index, std::string iconName) {
		m_outputIcons[index] = iconName + "_png";
	}
public:
	/**
	 * Constructs an atom from an AtomParameters instance
	 * @param parameters The AtomParameters object to use to build things like input and output audio buffers.
	 */
	AtomController(AtomParameters parameters);
	/**
	 * Returns a new instance of this atom controller. (This is used when creating
	 * an AtomController from the add atoms screen.) When inheriting from AtomController,
	 * make sure to override this to return a new instance of the subclass.
	 * @return A pointer to a new instance of this class. (Default: nullptr)
	 */
	virtual AtomController * createNewInstance() {
		return nullptr;
	}
	/**
	 * Destructor.
	 */
	virtual ~AtomController();

	/**
	 * Creates a new atom of this type. (Used by init() to initialize child atoms.)
	 * When creating a subtype, make sure to override this function with a call to
	 * the subclass's relevant Atom subclass's constructor.
	 * @param index The polyphony index of this atom, to be passed on to the constructor.
	 * @return A new Atom object.
	 */
	virtual Atom * createAtom(int index);
	/**
	 * Gets the Atom at a particular polyphony index.
	 * @param index The polyphony index from which to retrieve the Atom.
	 * @return A pointer to the Atom at the requested index.
	 */
	virtual Atom * getAtom(int index);

	/**
	 * Loads the controller's state from a SaveState.
	 * @param state SaveState to load from.
	 */
	virtual void loadSaveState(SaveState state);
	/**
	 * Saves the current state of the controller to a SaveState.
	 * @return A SaveState representing the atom's current state.
	 */
	virtual SaveState saveSaveState();

	/**
	 * This is used for grouping atoms in the add atom view.
	 * @return Name of the category this controller belongs to.
	 */
	virtual std::string getCategory() {
		return "ungrouped";
	}
	/**
	 * Returns the human-readable name of this controller. (E.G. Waveform Osc)
	 * @return The human-readable name of this controller. (E.G. Waveform Osc)
	 */
	virtual std::string getName() {
		return "";
	}

	/**
	 * Links the output of another AtomController to a primary
	 * input on this AtomController.
	 * @param index The index of the primary input to link to.
	 * @param controller The controller to retrieve the output from.
	 * @param outputIndex The index of the output on the other AtomController to link.
	 */
	void linkPrimaryInput(int index, AtomController * controller, int outputIndex);
	/**
	 * Links the output of another AtomController to an automation
	 * input on this AtomController.
	 * @param index The index of the automation input to link to.
	 * @param controller The controller to retrieve the output from.
	 * @param outputIndex The index of the output on the other AtomController to link.
	 */
	void linkAutomationInput(int index, AtomController * controller, int outputIndex);
	/**
	 * Links the output of another AtomController to an input on
	 * this AtomController. This can link to both primary and
	 * automation inputs.
	 * @param index The index of the input to link to. Inputs are indexed as they appear on top of an atom (primary inputs first, then automation inputs.)
	 * @param controller The controller to retrieve the output from.
	 * @param outputIndex The index of the output on the other AtomController to link.
	 */
	void linkInput(int index, AtomController * controller, int outputIndex);
	/**
	 * Unlink a primary input such that it becomes a nullptr.
	 * @param index The index of the primary input to unlink.
	 */
	void unlinkPrimaryInput(int index);
	/**
	 * Unlink an automation input such that it becomes a nullptr.
	 * @param index The index of the automation input to unlink.
	 */
	void unlinkAutomationInput(int index);
	/**
	 * Unlink an input such that it becomes a nullptr. This
	 * can unlink both primary and automation inputs.
	 * @param index The index of the input to unlink. Inputs are indexed as they appear on top of an atom (primary inputs first, then automation inputs.)
	 */
	void unlinkInput(int index);

	/**
	 * Returns the number of primary inputs this atom has.
	 * @return The number of primary inputs this atom has.
	 */
	int getNumPrimaryInputs() {
		return m_parameters.m_numPrimaryInputs;
	}
	/**
	 * Returns how many automation inputs this atom has. (If
	 * automation is disabled, it will return 0)
	 * @return The number of automation inputs this atom has.
	 */
	int getNumAutomationInputs() {
		return (getAutomationEnabled()) ? AUTOMATION_INPUTS : 0;
	}
	/**
	 * Returns the total number of inputs this atom has. (Equivalent
	 * to getNumPrimaryInputs() + getNumAutomationInputs())
	 * @return The total number of inputs this atom has.
	 */
	int getNumInputs() {
		return getNumPrimaryInputs() + getNumAutomationInputs();
	}
	/**
	 * Returns the number of outputs this atom has.
	 * @return The number of outputs this atom has.
	 */
	int getNumOutputs() {
		return m_parameters.m_numOutputs;
	}
	/**
	 * Returns the unique ID of this class.
	 * @return The unique ID of this class.
	 */
	int getId() {
		return m_parameters.m_id;
	}
	/**
	 * Returns whether or not this atom has automation inputs enabled.
	 * @return Whether or not this atom has automation inputs enabled.
	 */
	bool getAutomationEnabled() {
		return m_parameters.m_automationEnabled;
	}
	/**
	 * Returns a copy of the AtomParameters object this controller
	 * was created with.
	 * @return A copy of the AtomParameters object this controller was created with.
	 */
	AtomParameters getParameters() {
		return m_parameters;
	}
	/**
	 * Gets a representation of a primary input at a particular index.
	 * @param index The index of the primary input to get.
	 * @return An std::pair with a pointer to the AtomController this input is linked to (can be nullptr) and an integer of which output on that controller it is connected to.
	 */
	std::pair<AtomController *, int> getPrimaryInput(int index) {
		return m_primaryInputs[index];
	}
	/**
	 * Gets a representation of an automation input at a particular index.
	 * @param index The index of the automation input to get.
	 * @return An std::pair with a pointer to the AtomController this input is linked to (can be nullptr) and an integer of which output on that controller it is connected to.
	 */
	std::pair<AtomController *, int> getAutomationInput(int index) {
		return m_automationInputs[index];
	}
	/**
	 * Gets a representation of an input at a particular index. Both
	 * primary and automation inputs can be retrieved this way.
	 * @param index The index of the input to get. Inputs are indexed as they appear on top of an atom (primary inputs first, then automation inputs.)
	 * @return An std::pair with a pointer to the AtomController this input is linked to (can be nullptr) and an integer of which output on that controller it is connected to.
	 */
	std::pair<AtomController *, int> getInput(int index);
	/**
	 * Gets a vector of representations of all the inputs.
	 * @return A vector of all inputs, each input is in the same format as getInput().
	 */
	std::vector<std::pair<AtomController *, int> > getAllInputs();

	/**
	 * Calls Atom::execute() on all child Atom objects.
	 */
	virtual void execute();
	/**
	 * Calls AutomatedControl::clearDisplayValue() for all AutomatedControl
	 * objects added via addAutomatedControl().
	 */
	virtual void stopControlAnimation();

	/**
	 * Sets the position of this controller. (Used when rendering in AtomNetworkWidget.)
	 * @param x The x coordinate of the position.
	 * @param y The y coordinate of the position.
	 */
	void setPosition(int x, int y) {
		m_x = x;
		m_y = y;
	}
	/**
	 * Returns a reference to this controller's gui.
	 * @return A reference to this controller's gui.
	 */
	AtomGui & getGui() {
		return m_gui;
	}
	/**
	 * Returns the x coordinate of the controller's position.
	 * @return The x coordinate of the controller's position.
	 */
	int getX() {
		return m_x;
	}
	/**
	 * Returns the y coordinate of the controller's position.
	 * @return The y coordinate of the controller's position.
	 */
	int getY() {
		return m_y;
	}
	/**
	 * Returns the vector of all the names of the input icons
	 * for this atom.
	 * @return The vector of all the names of the input icons.
	 */
	std::vector<std::string>& getInputIcons() {
		return m_inputIcons;
	}
	/**
	 * Returns the vector of all the names of the output icons
	 * for this atom.
	 * @return The vector of all the names of the output icons.
	 */
	std::vector<std::string>& getOutputIcons() {
		return m_outputIcons;
	}

	/**
	 * Sets a flag that will be read later by the main loop, and
	 * will cause this AtomController to be deleted.
	 */
	void markForDeletion() {
		m_shouldBeDeleted = true;
	}
	/**
	 * Returns true if markForDeletion() has previously been called.
	 * @return True if markForDeletion() has previously been called.
	 */
	bool getIsMarkedForDeletion() {
		return m_shouldBeDeleted;
	}
	/**
	 * Removes all inputs that link to the specified AtomController.
	 * @param source All links between the source and this controller will be deleted.
	 */
	void cleanupInputsFromAtom(AtomController * source);

	friend class Atom;
};

/**
 * Keeps track of iterators of inputs and outputs.
 *
 * This is used in Atom::execute() to manage iterators for the
 * inputs and outputs of the atom. See AudioBuffer for why this
 * is necessary.
 */
class IOSet {
private:
	std::vector<AudioBuffer*> m_constInputSources, m_incInputSources, m_outputSources;
	std::vector<DVecIter*> m_constInputs, m_incInputs, m_outputs;
	//typedef std::vector<double>::iterator DVecIter;
public:
	/**
	 * Constructor
	 */
	IOSet() {
	}
	/**
	 * Destructor
	 */
	virtual ~IOSet() {
	}

	/**
	 * Clears all inputs and outputs added to this IOSet.
	 * There shouldn't be a reason to use this.
	 */
	void clear();
	/**
	 * Add an input to this IOSet. The returned iterator
	 * is managed and incremented by this IOSet. See
	 * AudioBuffer for why this is necessary.
	 * @param input A pointer to an AudioBuffer to manage. (It must be the default size, but this is not checked.) (The pointer can be nullptr)
	 * @return A pointer to an iterator over the input managed by this IOSet. If the input is a nullptr, the return result will also be a nullptr.
	 */
	DVecIter* addInput(AudioBuffer * input);
	/**
	 * Add an input to this IOSet. The returned iterator
	 * is managed and incremented by this IOSet. See
	 * AudioBuffer for why this is necessary.
	 * @param output A reference to an AudioBuffer to manage. (It must be the default size, but this is not checked.)
	 * @return A pointer to an iterator over the output managed by this IOSet.
	 */
	DVecIter& addOutput(AudioBuffer& output);

	/**
	 * Resets all iterators managed by this IOSet to their starting positions.
	 */
	void resetPosition();
	/**
	 * Increments all iterators to move to the next
	 * sample of data in their respective buffers. See
	 * AudioBuffer for why you should use this over ++.
	 */
	void incrementPosition();
	/**
	 * Increments all iterators to move to the beginning
	 * of the next channel.
	 */
	void incrementChannel();
};

//Atom class
//Represents a particular polyphony of an atom controller.
//E.G. with 8x polyphony, there will be 8 envelope atoms for 1 envelope controller
//This has no gui, only audio processing.
/**
 * This represents a single polyphony of an atom. An AtomController
 * has multiple of these, one for each polyphony the synth has. It
 * is responsible for data processing, taking inputs and translating
 * them to outputs. It has no GUI code.
 */
class Atom {
private:
	AtomController & m_p;
	int m_updateTimer;
	AtomParameters m_parameters;
	std::vector<AudioBuffer *> m_automationInputs;
protected:
	int m_sampleRate; ///< Utility variable to easily check the current sample rate.
	double m_sampleRate_f; ///< Like m_sampleRate, but a double.
	bool m_shouldUpdateParent; ///< This is true if the Atom should update the GUI of the parent.
	std::vector<AudioBuffer *> m_primaryInputs; ///< Vector of pointers to AudioBuffer objects of outputs that each input is linked to.
	std::vector<AudioBuffer> m_outputs; ///< Vector of AudioBuffer objects, one for each output. Write to these to set the output of the atom.

	/**
	 * Like AtomController::linkInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to link to.
	 * @param buffer Buffer to link to selected input.
	 */
	void linkInput(int index, AudioBuffer * buffer);
	/**
	 * Like AtomController::linkPrimaryInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to link to.
	 * @param buffer Buffer to link to selected input.
	 */
	void linkPrimaryInput(int index, AudioBuffer * buffer) {
		m_primaryInputs[index] = buffer;
	}
	/**
	 * Like AtomController::linkAutomationInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to link to.
	 * @param buffer Buffer to link to selected input.
	 */
	void linkAutomationInput(int index, AudioBuffer * buffer) {
		m_automationInputs[index] = buffer;
	}
	/**
	 * Like AtomController::unlinkInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to unlink.
	 */
	void unlinkInput(int index);
	/**
	 * Like AtomController::unlinkPrimaryInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to unlink.
	 */
	void unlinkPrimaryInput(int index) {
		m_primaryInputs[index] = nullptr;
	}
	/**
	 * Like AtomController::unlinkAutomationInput(), but works on AudioBuffer objects instead of AtomController objects.
	 * @param index Index of input to unlink.
	 */
	void unlinkAutomationInput(int index) {
		m_automationInputs[index] = nullptr;
	}
public:
	/**
	 * Constructor, takes a reference to the parent and
	 * the polyphony index.
	 * @param parent A reference to an AtomController that is the parent of this Atom.
	 * @param index The polyphony index.
	 */
	Atom(AtomController & parent, int index);
	/**
	 * Destructor.
	 */
	virtual ~Atom();

	/**
	 * Gets a pointer to an audio buffer of an
	 * output at a particular index. This is
	 * mostly used internally to link Atom objects
	 * together.
	 * @param index Index of the output.
	 * @return A pointer to the audio buffer of the requested output.
	 */
	AudioBuffer * getOutput(int index) {
		return &m_outputs[index];
	}
	/**
	 * Returns the polyphony index of this Atom.
	 * @return The polyphony index of this Atom.
	 */
	int getIndex() {
		return m_parameters.m_id;
	}

	/**
	 * Calls execute(). Does some extra things before
	 * and after it, too.
	 */
	void executeWrapper();

	/**
	 * Called when the Atom's function should be executed.
	 * (E.G. if you are implementing an amplifier, this
	 * method should be overrided to amplify an input and
	 * send the result to an output.) If m_shouldUpdateParent
	 * is true, then this function should also update any
	 * custom GUI elements in the parent. (AutomatedControl
	 * objects are updated automatically.)
	 */
	virtual void execute();
	/**
	 * Called when whatever note this Atom was playing has
	 * stopped. Use this to reset internal variables to
	 * starting values, and to reset any custom GUI elements.
	 */
	virtual void reset();

	friend class AutomatedControl;
	friend class AtomController;
};

} /* namespace AtomSynth */

#endif /* SOURCE_ATOM2_H_ */
