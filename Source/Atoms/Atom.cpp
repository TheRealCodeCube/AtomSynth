/*
 * Atom.cpp
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#include "Atom.h"

#include <array>
#include <iostream>

#include "Technical/SaveState.h"

namespace AtomSynth
{

AtomParameters::AtomParameters(int id, int numPrimaryInputs, bool automationEnabled, int numOutputs)
	: m_numPrimaryInputs(numPrimaryInputs),
	  m_numOutputs(numOutputs),
	  m_id(id),
	  m_automationEnabled(automationEnabled)
{

}

AtomParameters AtomParameters::withId(int newId)
{
	AtomParameters toReturn = * this;
	toReturn.m_id = newId;
	return toReturn;
}

void AtomController::init()
{
	for(int i = 0; i < GlobalNoteStates::getPolyphony(); i++)
	{
		m_atoms.push_back(createAtom(i));
	}
}

AtomController::AtomController(AtomParameters parameters)
	: m_primaryInputs(std::vector<std::pair<AtomController *, int>>()),
	  m_automationInputs(std::vector<std::pair<AtomController *, int>>()),
	  m_atoms(std::vector<Atom *>()),
	  m_parameters(parameters),
	  m_x(0), m_y(0),
	  m_stopped(false),
	  m_shouldBeDeleted(false),
	  m_gui()
{
	m_primaryInputs.resize(m_parameters.m_numPrimaryInputs, std::pair<AtomController *, int>(nullptr, 0));
	m_automationInputs.resize(AUTOMATION_INPUTS, std::pair<AtomController *, int>(nullptr, 0));
}

AtomController::~AtomController()
{

}

Atom * AtomController::createAtom(int index)
{
	return new Atom(* this, index);
}

Atom * AtomController::getAtom(int index)
{
	return m_atoms[index];
}

void AtomController::linkPrimaryInput(int index, AtomController* controller, int outputIndex)
{
	m_primaryInputs[index].first = controller;
	m_primaryInputs[index].second = outputIndex;
	int atomIndex = 0;
	for(Atom * atom : m_atoms)
	{
		atom->linkPrimaryInput(index, controller->getAtom(atomIndex)->getOutput(outputIndex));
		atomIndex++;
	}
}

void AtomController::linkAutomationInput(int index, AtomController* controller, int outputIndex)
{
	m_automationInputs[index].first = controller;
	m_automationInputs[index].second = outputIndex;
	int atomIndex = 0;
	for(Atom * atom : m_atoms)
	{
		atom->linkAutomationInput(index, controller->getAtom(atomIndex)->getOutput(outputIndex));
		atomIndex++;
	}
}

void AtomController::linkInput(int index, AtomController * controller, int outputIndex)
{
	if(index < m_parameters.m_numPrimaryInputs)
	{
		linkPrimaryInput(index, controller, outputIndex);
	}
	else
	{
		linkAutomationInput(index - m_parameters.m_numPrimaryInputs, controller, outputIndex);
	}
}

void AtomController::unlinkPrimaryInput(int index)
{
	m_primaryInputs[index].first = nullptr;
	for(Atom * atom : m_atoms)
	{
		atom->unlinkPrimaryInput(index);
	}
}

void AtomController::unlinkAutomationInput(int index)
{
	m_automationInputs[index].first = nullptr;
	for(Atom * atom : m_atoms)
	{
		atom->unlinkAutomationInput(index);
	}
}

void AtomController::unlinkInput(int index)
{
	if(index < m_parameters.m_numPrimaryInputs)
	{
		unlinkPrimaryInput(index);
	}
	else
	{
		unlinkAutomationInput(index - m_parameters.m_numPrimaryInputs);
	}
}

std::pair<AtomController*, int> AtomController::getInput(int index)
{
	if(index < m_parameters.m_numPrimaryInputs)
	{
		return getPrimaryInput(index);
	}
	else
	{
		return getAutomationInput(index - m_parameters.m_numPrimaryInputs);
	}
}

std::vector<std::pair<AtomController*, int> > AtomController::getAllInputs()
{
	std::vector<std::pair<AtomController*, int> > toReturn;
	for(std::pair<AtomController*, int> input : m_primaryInputs)
	{
		toReturn.push_back(input);
	}
	for(std::pair<AtomController*, int> input : m_automationInputs)
	{
		toReturn.push_back(input);
	}
	return toReturn;
}

void AtomController::loadSaveState(SaveState state)
{
	m_x = int(state.getValue(0));
	m_y = int(state.getValue(1));
}

SaveState AtomController::saveSaveState()
{
	SaveState state = SaveState();
	state.addValue(m_x);
	state.addValue(m_y);
	return state;
}

void AtomController::execute()
{
	//TODO: Change this so only active notes are calculated.
	for(int i = 0; i < GlobalNoteStates::getPolyphony(); i++)
	{
		if(GlobalNoteStates::getIsActive(i)) //Only bother calculating active notes
		{
			m_atoms[i]->execute();
		}
	}

	if(GlobalNoteStates::getIsActive(0))
	{
		m_stopped = true;
	}
	else if(m_stopped)
	{
		stopControlAnimation();
		m_atoms[0]->reset();
		m_stopped = false;
	}
}

void AtomController::stopControlAnimation()
{
	m_automation.stopAutomationAnimation();
}

void AtomController::cleanupInputsFromAtom(AtomController * source)
{
	for(std::pair<AtomController *, int> & input : m_primaryInputs)
	{
		if(input.first == source)
		{
			input.first = nullptr;
			input.second = -1;
		}
	}

	for(std::pair<AtomController *, int> & input : m_automationInputs)
	{
		if(input.first == source)
		{
			input.first = nullptr;
			input.second = -1;
		}
	}
}

Atom::Atom(AtomController & parent, int index)
	: m_p(parent),
	  m_updateTimer(0),
	  m_parameters(parent.getParameters().withId(index)),
	  m_sampleRate(0),
	  m_sampleRate_f(0.0f),
	  m_shouldUpdateParent(false)
{
	m_primaryInputs.resize(m_parameters.m_numPrimaryInputs, nullptr);
	m_automationInputs.resize(AUTOMATION_INPUTS, nullptr);
	m_outputs.resize(m_parameters.m_numOutputs, AudioBuffer());
}

Atom::~Atom()
{
	// TODO Auto-generated destructor stub
}

void Atom::execute()
{
	m_sampleRate = GlobalNoteStates::s_sampleRate;
	m_sampleRate_f = double(m_sampleRate);
	if(m_parameters.m_id == 0)
	{
		if(m_updateTimer == 0)
		{
			m_updateTimer = 5;
			m_shouldUpdateParent = true;
		}
		else
		{
			m_updateTimer--;
			m_shouldUpdateParent = false;
		}
	}

	if(m_parameters.m_automationEnabled)
	{
		m_p.m_automation.calculateAutomation(* this);
	}
}

void Atom::linkInput(int index, AudioBuffer * buffer)
{
	if(index < m_parameters.m_numPrimaryInputs)
	{
		linkPrimaryInput(index, buffer);
	}
	else
	{
		linkAutomationInput(index - m_parameters.m_numPrimaryInputs, buffer);
	}
}

void Atom::unlinkInput(int index)
{
	if(index < m_parameters.m_numPrimaryInputs)
	{
		unlinkPrimaryInput(index);
	}
	else
	{
		unlinkAutomationInput(index - m_parameters.m_numPrimaryInputs);
	}
}

} /* namespace AtomSynth */
