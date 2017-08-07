/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[31.000000:4.000000:1.000000:s4x Mixer:sProcessing:sch1, ch2, ch3, ch4:sch1:swave, wave, wave, wave:swave:[[[16.000000:10.000000:1.000000:1.000000:8.000000:1.000000:sAtomSlider:svol4:sInput 4:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[24.000000:9.500000:12.500000:2.000000:3.000000:1.000000:sMultiButton:smixMode3:s:]][1.000000:0.000000:sAdd:sAverage:sMultiply:sMinimum:sMaximum:]][[[16.000000:1.000000:1.000000:1.000000:8.000000:1.000000:sAtomSlider:svol1:sInput 1:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[24.000000:3.500000:12.500000:2.000000:3.000000:1.000000:sMultiButton:smixMode1:s:]][1.000000:0.000000:sAdd:sAverage:sMultiply:sMinimum:sMaximum:]][[[16.000000:4.000000:1.000000:1.000000:8.000000:1.000000:sAtomSlider:svol2:sInput 2:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[16.000000:7.000000:1.000000:1.000000:8.000000:1.000000:sAtomSlider:svol3:sInput 3:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]][[[4.000000:0.500000:9.500000:2.000000:2.000000:1.000000:sAtomKnob:span1:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:3.500000:9.500000:2.000000:2.000000:1.000000:sAtomKnob:span2:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:6.500000:9.500000:2.000000:2.000000:1.000000:sAtomKnob:span3:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[24.000000:6.500000:12.500000:2.000000:3.000000:1.000000:sMultiButton:smixMode2:s:]][1.000000:0.000000:sAdd:sAverage:sMultiply:sMinimum:sMaximum:]][[[4.000000:9.500000:9.500000:2.000000:2.000000:1.000000:sAtomKnob:span4:sPanning:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[24.000000:10.000000:15.000000:1.000000:2.000000:0.000000:sArrow:sArrow:sOutput:]][2.000000:0.000000:]][[[24.000000:4.000000:11.000000:1.000000:1.500000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:1.000000:11.000000:1.000000:3.500000:1.000000:sArrow:sArrow:s:]][3.000000:0.000000:]][[[24.000000:10.000000:11.000000:1.000000:1.500000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:7.000000:11.000000:1.000000:1.500000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:5.000000:13.500000:1.500000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:8.000000:13.500000:1.500000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]][[[24.000000:1.000000:13.500000:2.500000:1.000000:1.000000:sArrow:sArrow:s:]][2.000000:0.000000:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Mixer.h"

#include "Adsp/Basic.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

MixerController::MixerController() :
		AtomController(AtomParameters(31, 4, true, 1)) {
	init();

	addInputIcon("wave");
	addInputIcon("wave");
	addInputIcon("wave");
	addInputIcon("wave");
	addOutputIcon("wave");

	m_gui.addComponent(&m_vol4);
	m_vol4.setBounds(CB(10.000000, 1.000000, 1.000000, 8.000000));
	m_vol4.addListener(this);
	m_vol4.setRange(0.000000, 1.000000);
	m_vol4.setValue(1.000000);
	m_vol4.setBounded(true);
	addAutomatedControl(m_vol4, m_vol4Iter);
	m_gui.addComponent(m_vol4.createLabel("Input 4", true));

	m_gui.addComponent(&m_mixMode3);
	m_mixMode3.setBounds(CB(9.500000, 12.500000, 2.000000, 3.000000));
	m_mixMode3.addListener(this);
	m_mixMode3.setVertical(true);
	m_mixMode3.addLabel("Add");
	m_mixMode3.addLabel("Average");
	m_mixMode3.addLabel("Multiply");
	m_mixMode3.addLabel("Minimum");
	m_mixMode3.addLabel("Maximum");

	m_gui.addComponent(&m_vol1);
	m_vol1.setBounds(CB(1.000000, 1.000000, 1.000000, 8.000000));
	m_vol1.addListener(this);
	m_vol1.setRange(0.000000, 1.000000);
	m_vol1.setValue(1.000000);
	m_vol1.setBounded(true);
	addAutomatedControl(m_vol1, m_vol1Iter);
	m_gui.addComponent(m_vol1.createLabel("Input 1", true));

	m_gui.addComponent(&m_mixMode1);
	m_mixMode1.setBounds(CB(3.500000, 12.500000, 2.000000, 3.000000));
	m_mixMode1.addListener(this);
	m_mixMode1.setVertical(true);
	m_mixMode1.addLabel("Add");
	m_mixMode1.addLabel("Average");
	m_mixMode1.addLabel("Multiply");
	m_mixMode1.addLabel("Minimum");
	m_mixMode1.addLabel("Maximum");

	m_gui.addComponent(&m_vol2);
	m_vol2.setBounds(CB(4.000000, 1.000000, 1.000000, 8.000000));
	m_vol2.addListener(this);
	m_vol2.setRange(0.000000, 1.000000);
	m_vol2.setValue(1.000000);
	m_vol2.setBounded(true);
	addAutomatedControl(m_vol2, m_vol2Iter);
	m_gui.addComponent(m_vol2.createLabel("Input 2", true));

	m_gui.addComponent(&m_vol3);
	m_vol3.setBounds(CB(7.000000, 1.000000, 1.000000, 8.000000));
	m_vol3.addListener(this);
	m_vol3.setRange(0.000000, 1.000000);
	m_vol3.setValue(1.000000);
	m_vol3.setBounded(true);
	addAutomatedControl(m_vol3, m_vol3Iter);
	m_gui.addComponent(m_vol3.createLabel("Input 3", true));

	m_gui.addComponent(&m_pan1);
	m_pan1.setBounds(CB(0.500000, 9.500000, 2.000000, 2.000000));
	m_pan1.addListener(this);
	m_pan1.setBounded(true);
	addAutomatedControl(m_pan1, m_pan1Iter);
	m_gui.addComponent(m_pan1.createLabel("Panning", true));

	m_gui.addComponent(&m_pan2);
	m_pan2.setBounds(CB(3.500000, 9.500000, 2.000000, 2.000000));
	m_pan2.addListener(this);
	m_pan2.setBounded(true);
	addAutomatedControl(m_pan2, m_pan2Iter);
	m_gui.addComponent(m_pan2.createLabel("Panning", true));

	m_gui.addComponent(&m_pan3);
	m_pan3.setBounds(CB(6.500000, 9.500000, 2.000000, 2.000000));
	m_pan3.addListener(this);
	m_pan3.setBounded(true);
	addAutomatedControl(m_pan3, m_pan3Iter);
	m_gui.addComponent(m_pan3.createLabel("Panning", true));

	m_gui.addComponent(&m_mixMode2);
	m_mixMode2.setBounds(CB(6.500000, 12.500000, 2.000000, 3.000000));
	m_mixMode2.addListener(this);
	m_mixMode2.setVertical(true);
	m_mixMode2.addLabel("Add");
	m_mixMode2.addLabel("Average");
	m_mixMode2.addLabel("Multiply");
	m_mixMode2.addLabel("Minimum");
	m_mixMode2.addLabel("Maximum");

	m_gui.addComponent(&m_pan4);
	m_pan4.setBounds(CB(9.500000, 9.500000, 2.000000, 2.000000));
	m_pan4.addListener(this);
	m_pan4.setBounded(true);
	addAutomatedControl(m_pan4, m_pan4Iter);
	m_gui.addComponent(m_pan4.createLabel("Panning", true));

	m_gui.addComponent(&m_arrow12);
	m_arrow12.setBounds(CB(10.000000, 15.000000, 1.000000, 2.000000));
	m_arrow12.setCaps(0, 2);
	m_gui.addComponent(m_arrow12.createLabel("Output", false));

	m_gui.addComponent(&m_arrow13);
	m_arrow13.setBounds(CB(4.000000, 11.000000, 1.000000, 1.500000));
	m_arrow13.setCaps(0, 2);

	m_gui.addComponent(&m_arrow14);
	m_arrow14.setBounds(CB(1.000000, 11.000000, 1.000000, 3.500000));
	m_arrow14.setCaps(0, 3);

	m_gui.addComponent(&m_arrow15);
	m_arrow15.setBounds(CB(10.000000, 11.000000, 1.000000, 1.500000));
	m_arrow15.setCaps(0, 2);

	m_gui.addComponent(&m_arrow16);
	m_arrow16.setBounds(CB(7.000000, 11.000000, 1.000000, 1.500000));
	m_arrow16.setCaps(0, 2);

	m_gui.addComponent(&m_arrow17);
	m_arrow17.setBounds(CB(5.000000, 13.500000, 1.500000, 1.000000));
	m_arrow17.setCaps(0, 2);

	m_gui.addComponent(&m_arrow18);
	m_arrow18.setBounds(CB(8.000000, 13.500000, 1.500000, 1.000000));
	m_arrow18.setCaps(0, 2);

	m_gui.addComponent(&m_arrow19);
	m_arrow19.setBounds(CB(1.000000, 13.500000, 2.500000, 1.000000));
	m_arrow19.setCaps(0, 2);

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * MixerController::createAtom(int index) {
	return new MixerAtom(*this, index);
}

SaveState MixerController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(4); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_vol4.saveSaveState());
	extraData.addState(m_mixMode3.saveSaveState());
	extraData.addState(m_vol1.saveSaveState());
	extraData.addState(m_mixMode1.saveSaveState());
	extraData.addState(m_vol2.saveSaveState());
	extraData.addState(m_vol3.saveSaveState());
	extraData.addState(m_pan1.saveSaveState());
	extraData.addState(m_pan2.saveSaveState());
	extraData.addState(m_pan3.saveSaveState());
	extraData.addState(m_mixMode2.saveSaveState());
	extraData.addState(m_pan4.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void MixerController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 4) {
		m_vol4.loadSaveState(extraData.getNextState());
		m_mixMode3.loadSaveState(extraData.getNextState());
		m_vol1.loadSaveState(extraData.getNextState());
		m_mixMode1.loadSaveState(extraData.getNextState());
		m_vol2.loadSaveState(extraData.getNextState());
		m_vol3.loadSaveState(extraData.getNextState());
		m_pan1.loadSaveState(extraData.getNextState());
		m_pan2.loadSaveState(extraData.getNextState());
		m_pan3.loadSaveState(extraData.getNextState());
		m_mixMode2.loadSaveState(extraData.getNextState());
		m_pan4.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	else if(version == 3) {
		m_vol4.loadSaveState(extraData.getNextState());
		m_mixMode3.loadSaveState(extraData.getNextState());
		m_vol1.loadSaveState(extraData.getNextState());
		m_mixMode1.loadSaveState(extraData.getNextState());
		m_vol2.loadSaveState(extraData.getNextState());
		m_vol3.loadSaveState(extraData.getNextState());
		m_pan1.loadSaveState(extraData.getNextState());
		m_pan2.loadSaveState(extraData.getNextState());
		m_pan3.loadSaveState(extraData.getNextState());
		m_mixMode2.loadSaveState(extraData.getNextState());
		m_pan4.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	else if(version == 1)
	{
		m_vol4.loadSaveState(extraData.getNextState());
		m_mixMode3.loadSaveState(extraData.getNextState());
		m_vol1.loadSaveState(extraData.getNextState());
		m_mixMode1.loadSaveState(extraData.getNextState());
		m_vol2.loadSaveState(extraData.getNextState());
		m_vol3.loadSaveState(extraData.getNextState());
		m_pan1.loadSaveState(extraData.getNextState());
		m_pan2.loadSaveState(extraData.getNextState());
		m_pan3.loadSaveState(extraData.getNextState());
		m_mixMode2.loadSaveState(extraData.getNextState());
		m_pan4.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void MixerController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void MixerController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

MixerAtom::MixerAtom(MixerController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void MixerAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & vol4Iter = m_parent.m_vol4Iter;
	DVecIter & vol1Iter = m_parent.m_vol1Iter;
	DVecIter & vol2Iter = m_parent.m_vol2Iter;
	DVecIter & vol3Iter = m_parent.m_vol3Iter;
	DVecIter & pan1Iter = m_parent.m_pan1Iter;
	DVecIter & pan2Iter = m_parent.m_pan2Iter;
	DVecIter & pan3Iter = m_parent.m_pan3Iter;
	DVecIter & pan4Iter = m_parent.m_pan4Iter;

	IOSet io = IOSet();
	DVecIter * ch1Input = io.addInput(m_primaryInputs[0]);
	DVecIter * ch2Input = io.addInput(m_primaryInputs[1]);
	DVecIter * ch3Input = io.addInput(m_primaryInputs[2]);
	DVecIter * ch4Input = io.addInput(m_primaryInputs[3]);
	DVecIter & ch1Output = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	double sample, chain, max;
	int index;
	std::vector<double> baseAmps(4), channelsToCompute, maxes(8, 0.0);
	std::vector<int> mixModes;
	std::vector<bool> useBaseAmps(4);
	useBaseAmps[0] = m_parent.m_vol1.getResult().isConstant() && m_parent.m_pan1.getResult().isConstant();
	useBaseAmps[1] = m_parent.m_vol2.getResult().isConstant() && m_parent.m_pan2.getResult().isConstant();
	useBaseAmps[2] = m_parent.m_vol3.getResult().isConstant() && m_parent.m_pan3.getResult().isConstant();
	useBaseAmps[3] = m_parent.m_vol4.getResult().isConstant() && m_parent.m_pan4.getResult().isConstant();
	std::vector<DVecIter*> volIters, panIters, chIters;
	volIters.push_back(&vol1Iter);
	volIters.push_back(&vol2Iter);
	volIters.push_back(&vol3Iter);
	volIters.push_back(&vol4Iter);
	panIters.push_back(&pan1Iter);
	panIters.push_back(&pan2Iter);
	panIters.push_back(&pan3Iter);
	panIters.push_back(&pan4Iter);
	chIters.push_back(ch1Input);
	chIters.push_back(ch2Input);
	chIters.push_back(ch3Input);
	chIters.push_back(ch4Input);
	for(int i = 0; i < 4; i++) {
		if(chIters[i] != nullptr) {
			channelsToCompute.push_back(i);
		}
	}
	mixModes.push_back(m_parent.m_mixMode1.getSelectedLabel());
	mixModes.push_back(m_parent.m_mixMode2.getSelectedLabel());
	mixModes.push_back(m_parent.m_mixMode3.getSelectedLabel());
	for(int i = 0; i < 3; i++) {
		switch(mixModes[i]) {
		case 0:
			mixModes[i] = Adsp::MIX_MODE_ADD;
			break;
		case 1:
			mixModes[i] = Adsp::MIX_MODE_AVERAGE;
			break;
		case 2:
			mixModes[i] = Adsp::MIX_MODE_MULTIPLY;
			break;
		case 3:
			mixModes[i] = Adsp::MIX_MODE_MINIMUM;
			break;
		case 4:
			mixModes[i] = Adsp::MIX_MODE_MAXIMUM;
			break;
		}
	}
	for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++) {
		for(int i : channelsToCompute) {
			if(useBaseAmps[i]) {
				if(c == 0) baseAmps[i] = **volIters[i] * Adsp::panLeftAmplitude(**panIters[i]); //left channel
				else baseAmps[i] = **volIters[i] * Adsp::panRightAmplitude(**panIters[i]); //right channel
			}
		}
		for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++) {
			for(int j = 0; j < channelsToCompute.size(); j++) {
				index = channelsToCompute[j];
				if(useBaseAmps[index]) {
					sample = **chIters[index] * baseAmps[index];
				} else {
					if(c == 0) { //left channel
						sample = Adsp::panLeft(**chIters[index] * **volIters[index], **panIters[index]);
					} else {
						sample = Adsp::panRight(**chIters[index] * **volIters[index], **panIters[index]);
					}
				}
				if(m_shouldUpdateParent) {
					maxes[index+c*4] = std::max(maxes[index+c*4], std::abs(sample));
				}
				if(j == 0) { //If this is the first valid input, put it directly into the chain. Otherwise, mix it with the chain.
					chain = sample;
				} else {
					chain = Adsp::mix2(chain, sample, mixModes[channelsToCompute[j-1]]);
				}
			}
			*ch1Output = chain;
			automation.incrementPosition();
			io.incrementPosition();
		}
		automation.incrementChannel();
		io.incrementChannel();
	}
	if(m_shouldUpdateParent) {
		m_parent.m_vol1.setAudioLevels(maxes[0], maxes[4]);
		m_parent.m_vol2.setAudioLevels(maxes[1], maxes[5]);
		m_parent.m_vol3.setAudioLevels(maxes[2], maxes[6]);
		m_parent.m_vol4.setAudioLevels(maxes[3], maxes[7]);
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void MixerAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	if(m_shouldUpdateParent) {
		m_parent.m_vol1.setAudioLevels(0.0, 0.0);
		m_parent.m_vol2.setAudioLevels(0.0, 0.0);
		m_parent.m_vol3.setAudioLevels(0.0, 0.0);
		m_parent.m_vol4.setAudioLevels(0.0, 0.0);
	}
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

