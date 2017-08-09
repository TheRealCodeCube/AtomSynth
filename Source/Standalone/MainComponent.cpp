/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include <algorithm>
#include <cmath>
#include <iostream>

#include "JuceLibraryCode/JuceHeader.h"
#include "Atoms/Atom.h"
#include "Gui/Global.h"
#include "Gui/HighLevelGui.h"
#include "Technical/AudioBuffer.h"
#include "Technical/SaveState.h"
#include "Technical/Synth.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
private:
    AtomSynth::AtomSynthEditor * m_editor = nullptr;
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (C::VST_WIDTH, C::VST_HEIGHT);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
	    AtomSynth::Synth::initialize(sampleRate, int(samplesPerBlockExpected));
	    AtomSynth::Synth::getInstance()->getNoteManager().addFrequency(440.0);
	    if(m_editor != nullptr) {
	        removeAllChildren();
	        delete(m_editor);
	    }
	    m_editor = new AtomSynth::AtomSynthEditor();
	    addAndMakeVisible(m_editor);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
	    const int numSamples = bufferToFill.numSamples;
	    AtomSynth::Synth::getInstance()->getParameters().m_timestamp += numSamples;
    
	    if (numSamples != AtomSynth::AudioBuffer::getDefaultSize())
		    prepareToPlay(AtomSynth::Synth::getInstance()->getParameters().m_sampleRate, numSamples);
    
	    AtomSynth::AudioBuffer result = AtomSynth::Synth::getInstance()->getAtomManager().execute();
	    for (int c = 0; c < AtomSynth::AudioBuffer::getDefaultChannels(); c++) {
		    for (int s = bufferToFill.startSample; s < bufferToFill.startSample + bufferToFill.numSamples; s++) {
			    bufferToFill.buffer->setSample(c, s, result.getValue(c, s));
		    }
	    }
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
