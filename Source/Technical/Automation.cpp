/*
 * Automation.cpp
 *
 *  Created on: May 29, 2017
 *      Author: josh
 */

#include "Automation.h"

#include <iomanip>
#include <sstream>

#include "Atoms/Atom.h"

namespace AtomSynth {

void AutomatedControl::checkBounds() {
	if (m_bounded) {
		if (m_value > m_max)
			m_value = m_max;
		else if (m_value < m_min)
			m_value = m_min;
	}
}

void AutomatedControl::informListeners(bool byUser) {
	for (Listener * listener : m_listeners) {
		listener->automatedControlChanged(this, byUser);
	}
}

AutomationInfluence::AutomationInfluence() :
		m_inputIndex(-1),
		m_minRange(0.0),
		m_maxRange(1.0) {

}

AutomatedControl::AutomatedControl() :
		m_min(-1.0),
		m_max(1.0),
		m_value(0.0),
		m_display(0.0),
		m_mixMode(MixMode::AVERAGE),
		m_suffix(""),
		m_decimalPlaces(2),
		m_bounded(true),
		m_int(false),
		m_displaying(false),
		m_silent(false) {
	for (int i = 0; i < 4; i++) {
		m_influences[i] = AutomationInfluence();
	}
}

void AutomatedControl::setValue(double value) {
	m_value = value;
	checkBounds();
	valueChanged();
	if (!m_silent) {
		informListeners(true);
	}
}

void AutomatedControl::offsetValue(double value) {
	m_value += value;
	checkBounds();
	valueChanged();
	if (!m_silent) {
		informListeners(true);
	}
}

void AutomatedControl::setDisplayValue(double value) {
	m_display = value;
	m_displaying = true;
	valueChanged();
	if (!m_silent) {
		informListeners(false);
	}
}

void AutomatedControl::clearDisplayValue() {
	m_displaying = false;
	valueChanged();
	if (!m_silent) {
		informListeners(false);
	}
}

double AutomatedControl::getDisplayValue() {
	return (m_displaying) ? m_display : getValue();
}

std::string AutomatedControl::getFormattedValue() {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(m_int ? 0 : m_decimalPlaces) << getDisplayValue();
	return stream.str() + m_suffix;
}

bool AutomatedControl::isAutomated() {
	for (AutomationInfluence inf : m_influences) {
		if (inf.m_inputIndex != -1) {
			return true;
		}
	}
	return false;
}

void AutomatedControl::loadSaveState(SaveState state) {
	m_value = state.getNextValue();
	m_mixMode = static_cast<MixMode>(int(state.getNextValue()));
	for (AutomationInfluence & i : m_influences) {
		SaveState knobState = state.getNextState();
		i.m_inputIndex = int(knobState.getNextValue());
		i.m_minRange = knobState.getNextValue();
		i.m_maxRange = knobState.getNextValue();
	}
	if(state.getValues().size() > 2) {
		m_min = state.getNextValue();
		m_max = state.getNextValue();
		m_suffix = state.getNextString();
	}
	valueChanged();
	if (!m_silent) {
		informListeners(false);
	}
}

SaveState AutomatedControl::saveSaveState() {
	SaveState state = SaveState();
	state.addValue(m_value);
	state.addValue(static_cast<int>(m_mixMode));
	for (AutomationInfluence & i : m_influences) {
		SaveState knobState = SaveState();
		knobState.addValue(i.m_inputIndex);
		knobState.addValue(i.m_minRange);
		knobState.addValue(i.m_maxRange);
		state.addState(knobState);
	}
	state.addValue(m_min);
	state.addValue(m_max);
	state.addString(m_suffix);
	return state;
}

void AutomatedControl::calculateAutomation(std::vector<AutomatedControl *> & controls, Atom & source) {
	std::vector<double> baseValues;
	std::vector<double> divisors;
	baseValues.resize(controls.size());
	std::vector<bool> shouldCalculate;
	shouldCalculate.resize(controls.size());
	std::vector<std::array<double, 4>> multiply;
	multiply.resize(controls.size());
	std::vector<std::array<double, 4>> add;
	add.resize(controls.size());
	std::vector<std::array<AudioBuffer *, 4>> sources;
	sources.resize(controls.size());

	std::vector<int> calculateIndexes;
	std::vector<int> basicIndexes;

	bool calculate;
	int infIndex = 0;
	double divisor = 0.0, kmax, kmin, imax, imin;
	for (int i = 0; i < controls.size(); i++) {
		shouldCalculate[i] = false;
		baseValues[i] = controls[i]->getValue();
		infIndex = 0;
		divisor = 0.0;
		for (AutomationInfluence inf : controls[i]->m_influences) {
			if (inf.m_inputIndex != -1) {
				divisor++;
			}
		}
		if (divisor != 0.0) {
			divisor = 1.0 / divisor;
		}
		divisors.push_back(divisor);

		//Bounded controls have predetermined ranges.
		//Unbounded controls have user defined ranges.
		if (controls[i]->m_bounded) {
			kmax = controls[i]->m_max;
			kmin = controls[i]->m_min;
		} else {
			kmax = controls[i]->getValue();
			kmin = -controls[i]->getValue();
		}

		for (AutomationInfluence inf : controls[i]->m_influences) {
			if (inf.m_inputIndex != -1) {
				sources[i][infIndex] = source.m_automationInputs[inf.m_inputIndex];
				imax = inf.m_maxRange;
				imin = inf.m_minRange;
				//Simplifies the computation, more detail below.
				multiply[i][infIndex] = (((kmax * (imax - imin)) + (kmin * (imin - imax))) / 2.0);
				add[i][infIndex] = (((kmax - kmin) * (imax + imin) / 2.0) + kmin);
				shouldCalculate[i] = true;
			} else {
				multiply[i][infIndex] = 0.0;
				add[i][infIndex] = 0.0;
				sources[i][infIndex] = nullptr;
			}
			infIndex++;
		}

		if (shouldCalculate[i]) {
			calculateIndexes.push_back(i);
			switch (controls[i]->m_mixMode) {
			case MixMode::AVERAGE:
				controls[i]->m_result.fill(0.0);
				break;
			case MixMode::MULTIPLY:
				controls[i]->m_result.fill(1.0);
				break;
			case MixMode::MINIMUM:
				controls[i]->m_result.fill(controls[i]->m_max);
				break;
			case MixMode::MAXIMUM:
				controls[i]->m_result.fill(controls[i]->m_min);
				break;
			}
			controls[i]->m_result.setConstant(false);
		} else {
			basicIndexes.push_back(i);
			controls[i]->m_result.fill(baseValues[i]);
			controls[i]->m_result.setConstant(true);
		}
	}

	double value;
	std::vector<double>::iterator input, output;
	double mValue, aValue;
	bool isInt = false;
	for (int n : calculateIndexes) {
		isInt = controls[n]->m_int;
		divisor = divisors[n];
		for (int s = 0; s < 4; s++) {
			if (sources[n][s] != nullptr) {
				output = controls[n]->m_result.getData().begin();
				input = sources[n][s]->getData().begin();
				mValue = multiply[n][s];
				aValue = add[n][s];
				for (int i = 0; i < AudioBuffer::getDefaultChannels() * AudioBuffer::getDefaultSize(); i++) {
					//Values come in in range -1.0 to 1.0
					//Highly simplified version of converting from -1.0 - 1.0 to 0.0 - 1.0 to min - max to control top - control bottom
					//Original equation is ((((sample + 1) / 2) * (max - min) + min) * (hi - low) + low ) * percent
					//Algebra is really handy
					value = (*input) * mValue + aValue;
					switch (controls[n]->m_mixMode) {
					case MixMode::AVERAGE:
						(*output) += value * divisor;
						break;
					case MixMode::MULTIPLY:
						(*output) *= value;
						break;
					case MixMode::MINIMUM:
						(*output) = std::min(*output, value);
						break;
					case MixMode::MAXIMUM:
						(*output) = std::max(*output, value);
						break;
					}
					input++;
					output++;
				}
			}
		}

		if ((source.getIndex() == 0) && (source.m_shouldUpdateParent)) {
			controls[n]->setDisplayValue(controls[n]->m_result.getData()[0]);
		}
	}
}

void AutomationSet::add(AutomatedControl & control, std::vector<double>::iterator & iterator) {
	m_controls.push_back(&control);
	m_iterators.push_back(&iterator);
}

void AutomationSet::resetPosition() {
	m_incrementIndices.clear();
	m_constantIndices.clear();
	for (int i = 0; i < m_controls.size(); i++) {
		(*m_iterators[i]) = m_controls[i]->getResult().getData().begin();
		if (m_controls[i]->getResult().isConstant()) {
			m_constantIndices.push_back(i);
		} else {
			m_incrementIndices.push_back(i);
		}
	}
}

void AutomationSet::incrementPosition() {
	for (int i : m_incrementIndices) {
		(*m_iterators[i])++;
	}
}

void AutomationSet::incrementChannel() {
	for (int i : m_constantIndices) {
		(*m_iterators[i]) += AudioBuffer::getDefaultSize();
	}
}

} /* namespace AtomSynth */
