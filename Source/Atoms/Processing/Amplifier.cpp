/* ALL CODE SHOULD BE CONSIDERED AUTO-GENERATED UNLESS EXPLICITLY SPECIFIED */
// EDITOR SOURCE: [[30.000000:1.000000:1.000000:sAmplifier:sProcessing:ssignal:ssignal:swave:swave:[[24.000000:0.000000:0.000000:10.000000:10.000000:1.000000:sWaveformPlot:splot:s:]][[[16.000000:11.000000:0.000000:1.000000:6.000000:0.000000:sAtomSlider:samp2:sVol2:]][0.000000:4.000000:1.000000:0.000000:1.000000:s:]][[[16.000000:10.000000:0.000000:1.000000:6.000000:0.000000:sAtomSlider:samp1:sVol1:]][0.000000:4.000000:1.000000:0.000000:1.000000:s:]][[[24.000000:10.000000:7.000000:2.000000:3.000000:1.000000:sMultiButton:smixMode:sMix Mode:]][1.000000:0.000000:sSingle:sAverage:sMultiply:sMinimum:sMaximum:]][[[24.000000:0.000000:10.500000:2.000000:2.000000:1.000000:sMultiButton:sclip:sClipping?:]][1.000000:0.000000:sNo:sYes:]][[[4.000000:4.000000:10.500000:2.000000:2.000000:1.000000:sAtomKnob:sclipSkew:sClip Skew:]][-1.000000:1.000000:0.000000:0.000000:1.000000:s:]][[[4.000000:2.000000:10.500000:2.000000:2.000000:1.000000:sAtomKnob:sclipLevel:sClip Level:]][0.000000:1.000000:1.000000:0.000000:1.000000:s:]]]]

/* BEGIN AUTO-GENERATED INCLUDES */
#include "Amplifier.h"

#include "Adsp/Basic.h"
#include "Technical/SaveState.h"
/* END AUTO-GENERATED INCLUDES */

/* BEGIN USER-DEFINED INCLUDES */
/* END USER-DEFINED INCLUDES */

namespace AtomSynth {

/* BEGIN MISC. USER-DEFINED CODE */

/* END MISC. USER-DEFINED CODE */

AmplifierController::AmplifierController() :
		AtomController(AtomParameters(30, 1, true, 1)) {
	init();

	addInputIcon("wave");
	addOutputIcon("wave");

	m_gui.addComponent(&m_plot);
	m_plot.setBounds(CB(0.000000, 0.000000, 10.000000, 10.000000));

	m_gui.addComponent(&m_amp2);
	m_amp2.setBounds(CB(11.000000, 0.000000, 1.000000, 6.000000));
	m_amp2.addListener(this);
	m_amp2.setRange(0.000000, 4.000000);
	m_amp2.setValue(1.000000);
	m_amp2.setBounded(true);
	addAutomatedControl(m_amp2, m_amp2Iter);
	m_gui.addComponent(m_amp2.createLabel("Vol2", false));

	m_gui.addComponent(&m_amp1);
	m_amp1.setBounds(CB(10.000000, 0.000000, 1.000000, 6.000000));
	m_amp1.addListener(this);
	m_amp1.setRange(0.000000, 4.000000);
	m_amp1.setValue(1.000000);
	m_amp1.setBounded(true);
	addAutomatedControl(m_amp1, m_amp1Iter);
	m_gui.addComponent(m_amp1.createLabel("Vol1", false));

	m_gui.addComponent(&m_mixMode);
	m_mixMode.setBounds(CB(10.000000, 7.000000, 2.000000, 3.000000));
	m_mixMode.addListener(this);
	m_mixMode.setVertical(true);
	m_mixMode.addLabel("Single");
	m_mixMode.addLabel("Average");
	m_mixMode.addLabel("Multiply");
	m_mixMode.addLabel("Minimum");
	m_mixMode.addLabel("Maximum");
	m_gui.addComponent(m_mixMode.createLabel("Mix Mode", true));

	m_gui.addComponent(&m_clip);
	m_clip.setBounds(CB(0.000000, 10.500000, 2.000000, 2.000000));
	m_clip.addListener(this);
	m_clip.setVertical(true);
	m_clip.addLabel("No");
	m_clip.addLabel("Yes");
	m_gui.addComponent(m_clip.createLabel("Clipping?", true));

	m_gui.addComponent(&m_clipSkew);
	m_clipSkew.setBounds(CB(4.000000, 10.500000, 2.000000, 2.000000));
	m_clipSkew.addListener(this);
	m_clipSkew.setBounded(true);
	addAutomatedControl(m_clipSkew, m_clipSkewIter);
	m_gui.addComponent(m_clipSkew.createLabel("Clip Skew", true));

	m_gui.addComponent(&m_clipLevel);
	m_clipLevel.setBounds(CB(2.000000, 10.500000, 2.000000, 2.000000));
	m_clipLevel.addListener(this);
	m_clipLevel.setRange(0.000000, 1.000000);
	m_clipLevel.setValue(1.000000);
	m_clipLevel.setBounded(true);
	addAutomatedControl(m_clipLevel, m_clipLevelIter);
	m_gui.addComponent(m_clipLevel.createLabel("Clip Level", true));

	/* BEGIN USER-DEFINED CONSTRUCTION CODE */
	m_amp2.setVisible(false);
	/* END USER-DEFINED CONSTRUCTION CODE */

	m_gui.setAtomController(this);
}

Atom * AmplifierController::createAtom(int index) {
	return new AmplifierAtom(*this, index);
}

SaveState AmplifierController::saveSaveState() {
	SaveState & toReturn = *new SaveState();
	toReturn.addState(AtomController::saveSaveState());

	SaveState & extraData = *new SaveState();
	extraData.addValue(1); //Store the revision this was saved with, to preserve backwards compatibility.
	extraData.addState(m_amp2.saveSaveState());
	extraData.addState(m_amp1.saveSaveState());
	extraData.addState(m_mixMode.saveSaveState());
	extraData.addState(m_clip.saveSaveState());
	extraData.addState(m_clipSkew.saveSaveState());
	extraData.addState(m_clipLevel.saveSaveState());
	/* BEGIN USER-DEFINED SAVE CODE */

	/* END USER-DEFINED SAVE CODE */
	toReturn.addState(extraData);
	return toReturn;
}

void AmplifierController::loadSaveState(SaveState state) {
	AtomController::loadSaveState(state.getNextState());
	SaveState & extraData = state.getNextState();
	int version = extraData.getNextValue();
	/* BEGIN LOAD CODE */
	if(version == 1)
	{
		m_amp2.loadSaveState(extraData.getNextState());
		m_amp1.loadSaveState(extraData.getNextState());
		m_mixMode.loadSaveState(extraData.getNextState());
		m_clip.loadSaveState(extraData.getNextState());
		m_clipSkew.loadSaveState(extraData.getNextState());
		m_clipLevel.loadSaveState(extraData.getNextState());
		/* BEGIN USER-DEFINED LOAD CODE */

		/* END USER-DEFINED LOAD CODE */
	}
	/* END LOAD CODE */
}

void AmplifierController::automatedControlChanged(AutomatedControl * control, bool byUser) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	/* END USER-DEFINED LISTENER CODE */
}

void AmplifierController::multiButtonPressed(MultiButton * button) {
	/* BEGIN USER-DEFINED LISTENER CODE */
	if(button == &m_mixMode) {
		if(m_mixMode.getSelectedLabel() == 2) {
			//The most the other modes can get to is 4. With
			//multiplication, 2 * 2 = 4.
			m_amp1.setRange(0.0, 2.0);
			m_amp2.setRange(0.0, 2.0);
		} else {
			m_amp1.setRange(0.0, 4.0);
			m_amp2.setRange(0.0, 4.0);
		}
		m_amp1.repaint();
		m_amp2.repaint();
		if(m_mixMode.getSelectedLabel() == 0) {
			//Normal 1-slider mode.
			m_amp2.setVisible(false);
		} else {
			m_amp2.setVisible(true);
		}
	}
	/* END USER-DEFINED LISTENER CODE */
}

AmplifierAtom::AmplifierAtom(AmplifierController & parent, int index) :
		Atom(parent, index),
		m_parent(parent) {
	/* BEGIN USER-DEFINED CONSTRUCTION CODE */

	/* END USER-DEFINED CONSTRUCTION CODE */
}

void AmplifierAtom::execute() {
	Atom::execute();
	AutomationSet & automation = m_parent.m_automation;
	automation.resetPosition();
	DVecIter & amp2Iter = m_parent.m_amp2Iter;
	DVecIter & amp1Iter = m_parent.m_amp1Iter;
	DVecIter & clipSkewIter = m_parent.m_clipSkewIter;
	DVecIter & clipLevelIter = m_parent.m_clipLevelIter;

	IOSet io = IOSet();
	DVecIter * signalInput = io.addInput(m_primaryInputs[0]);
	DVecIter & signalOutput = io.addOutput(m_outputs[0]);

	/* BEGIN USER-DEFINED EXECUTION CODE */
	if(signalInput == nullptr) {
		m_outputs[0].fill(0.0);
	} else {
		int mixMode;
		bool skipMixing = false;
		switch(m_parent.m_mixMode.getSelectedLabel()) {
		case 0:
			mixMode = Adsp::MIX_MODE_AVERAGE;
			skipMixing = true;
			break;
		case 1:
			mixMode = Adsp::MIX_MODE_AVERAGE;
			break;
		case 2:
			mixMode = Adsp::MIX_MODE_MULTIPLY;
			break;
		case 3:
			mixMode = Adsp::MIX_MODE_MINIMUM;
			break;
		case 4:
			mixMode = Adsp::MIX_MODE_MAXIMUM;
			break;
		}

		double baseAmp, sample;
		bool clipping = m_parent.m_clip.getSelectedLabel(), //1 = clipping.
				useBaseAmp;
		//True if the amplification can be calculated more efficiently ahead of time.
		useBaseAmp = (m_parent.m_amp1.getResult().isConstant() &&
				(m_parent.m_amp2.getResult().isConstant() || skipMixing));
		for(int c = 0; c < AudioBuffer::getDefaultChannels(); c++)
		{
			if(useBaseAmp) {
				if(skipMixing) {
					baseAmp = *amp1Iter;
				} else {
					baseAmp = Adsp::mix2(*amp1Iter, *amp2Iter, mixMode);
				}
			}
			for(int s = 0; s < AudioBuffer::getDefaultSamples(); s++)
			{
				//PUT YOUR EXECUTION CODE HERE
				sample = **signalInput;
				if(useBaseAmp) {
					sample = Adsp::amplify(sample, baseAmp);
				} else if(skipMixing) {
					sample = Adsp::amplify(sample, *amp1Iter);
				} else {
					sample = Adsp::amplify(sample, Adsp::mix2(*amp1Iter, *amp2Iter, mixMode));
				}
				if(clipping) {
					sample += *clipSkewIter;
					sample = Adsp::clip(sample, -*clipLevelIter, *clipLevelIter);
				}
				*signalOutput = sample;
				automation.incrementPosition();
				io.incrementPosition();
			}
			automation.incrementChannel();
			io.incrementChannel();
		}

		if(m_shouldUpdateParent) {
			m_parent.m_plot.setDataFromAudioBuffer(m_outputs[0]);
		}
	}
	/* END USER-DEFINED EXECUTION CODE */
}

void AmplifierAtom::reset() {
	Atom::reset();
	/* BEGIN USER-DEFINED RESET CODE */
	if(m_shouldUpdateParent) {
		m_parent.m_plot.clear();
	}
	/* END USER-DEFINED RESET CODE */
}

} /* namespace AtomSynth */

