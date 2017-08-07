/*
 * AudioBuffer.h
 *
 *  Created on: Jul 22, 2016
 *      Author: josh
 */

#ifndef SOURCE_TECHNICAL_AUDIOBUFFER_H_
#define SOURCE_TECHNICAL_AUDIOBUFFER_H_

#include <vector>

namespace AtomSynth {

/**
 * A container to hold audio samples. Samples
 * are grouped into an arbitrary number of
 * channels. By default, AudioBuffer objects
 * are created with a default length and a
 * default number of channels corresponding
 * to the number of samples and channels that
 * is being requested for the synth to output.
 * Internally, an audio buffer might look
 * something like this:
 * @verbatim
 * | DATA FOR CHANNEL 0 | DATA FOR CHANNEL 1 |
 * 019820819285701928750019820819285701928750
 * @endverbatim
 * Buffers can also be 'constant', meaning that
 * only the first value should be considered.
 * For example, a buffer that is marked as
 * constant and looks like this:
 * @verbatim
 * | DATA FOR CHANNEL 0 | DATA FOR CHANNEL 1 |
 * 500000000000000000000300000000000000000000
 * @endverbatim
 * should be considered to have all 5's on
 * channel 0 and all 3's on channel 1. This
 * feature is used for improving computation
 * time when calculating things that change
 * slowly. (Only the first value has to be
 * calculated, since calculating every single
 * sample would yield an almost unchanging
 * value.)
 */
class AudioBuffer {
private:
	std::vector<double> m_data;
	bool m_constant;
	int m_size, m_channels;
	static int s_size, s_channels;
public:
	/**
	 * Create an audio buffer with the default
	 * length and number of channels.
	 */
	AudioBuffer();
	/**
	 * Create an audio buffer with a different
	 * length, but the default number of channels.
	 * @param samples The number of samples in a single channel.
	 */
	AudioBuffer(int samples);
	/**
	 * Create an audio buffer with a different length
	 * and a different number of channels.
	 * @param channels The number of channels to create.
	 * @param samples The number of samples in a single channel.
	 */
	AudioBuffer(int channels, int samples);
	virtual ~AudioBuffer();

	/**
	 * Outputs the contents of the audio buffer to the
	 * console. Only use this when you can't use a real
	 * debugger.
	 */
	void dumpToConsole();

	/**
	 * Copies data from another audio buffer to
	 * this audio buffer. If the source and destination
	 * are different sizes, the smallest size will be
	 * used
	 * @param copyFrom The AudioBuffer to copy data from.
	 */
	void copyData(AudioBuffer & copyFrom);
	/**
	 * Copies data from another audio buffer to this
	 * audio buffer, with an optional offset. If the
	 *  source and destination are different sizes,
	 * the smallest size will be used. An offfset can
	 * also be provided, which will copy all data from
	 * copyFrom into an earlier (if negative) or later
	 * (if positive) point in this AudioBuffer.
	 * @param copyFrom The source to copy data from.
	 * @param sampleOffset What sample index to start copying at.
	 * @param channelOffset What channel index to start copying at.
	 */
	void copyData(AudioBuffer & copyFrom, int sampleOffset, int channelOffset = 0);
	/**
	 * Moves all data in the buffer by an offset. The
	 * offset can be positive or negative. Samples that
	 * go outside of the range of the buffer because of
	 * this will be discarded. Samples moved into the
	 * buffer because of this will be initialized to
	 * zero. For example, if this audio buffer:
	 *
	 * [1, 4, 1, 3, 2]
	 *
	 * has an offset of -2 applied to it, this will be
	 * the result:
	 *
	 * [1, 3, 2, 0, 0]
	 * @param offset The number of samples to offset the data by.
	 */
	void offsetData(int offset);
	/**
	 * Fills the entire buffer with a single value.
	 * @param value The value to fill the AudioBuffer with.
	 */
	void fill(double value);

	/**
	 * Gets the underlying data vector.
	 * @return The raw data vector of the buffer.
	 */
	std::vector<double> & getData() {
		return m_data;
	}
	/**
	 * Gets a value on a particular channel at a
	 * particular sample.
	 * @param channel The index of the channel from which to retrieve the sample.
	 * @param sample The index of the sample to retrieve.
	 * @return The sample at the requested position.
	 */
	double getValue(int channel, int sample) {
		return m_data[channel * m_size + sample];
	}
	/**
	 * Sets a value on a particular channel at a
	 * particular sample.
	 * @param channel The index of the channel from which to set the sample.
	 * @param sample The index of the sample to set.
	 * @param value The value to set the sample to.
	 */
	void setValue(int channel, int sample, double value) {
		m_data[channel * m_size + sample] = value;
	}
	/**
	 * Gets a value on a particular channel at
	 * a particular position. Other implementations
	 * can add interpolation, but a basic AudioBuffer
	 * has no interpolation.
	 * @param channel The channel to get the value from.
	 * @param sample The sample position to get the value from. Can be a decimal value.
	 * @return The value at that position. (Not interpolated, use one of the subclasses if you want interpolation.)
	 */
	virtual double get(int channel, double sample) {
		return m_data[channel * m_size + int(sample + 0.5)];
	}
	/**
	 * Sets a value on a particular channel at
	 * a particular position. Other implementations
	 * can add interpolation, but a basic AudioBuffer
	 * has no interpolation.
	 * @param channel The channel to set the value on.
	 * @param sample The sample position to set the value on. Can be a decimal value.
	 * @return The value to set.
	 */
	virtual void set(int channel, double sample, double value) {
		m_data[channel * m_size + int(sample + 0.5)] = value;
	}

	/**
	 * Marks the AudioBuffer as being constant. (See
	 * the description of constant audio buffers in
	 * the description of AudioBuffer.)
	 * @param constant True if the audio buffer should be considered constant.
	 */
	void setConstant(bool constant) {
		m_constant = constant;
	}
	/**
	 * Gets if the AudioBuffer has been marked
	 * as constant. (See the description of
	 * constant audio buffers in the description
	 * of AudioBuffer.)
	 * @return True if the audio buffer has been marked as constant.
	 */
	bool isConstant() {
		return m_constant;
	}

	/**
	 * Sets the number of samples in each channel.
	 * By default, the size is set to the buffer
	 * size of the audio host (E.G. a DAW.) Old
	 * data will not be preserved, the AudioBuffer
	 * should be considered to be filled with
	 * garbage data.
	 * @param size The desired number of samples in each channel.
	 */
	void setSize(int size);
	/**
	 * Sets the number of channels. By default,
	 * the number of channels is set to however
	 * many the audio host (E.G. a DAW) has
	 * requested. Previous data will be preserved.
	 * (E.G. if going from 2 to 3 channels, the
	 * data in channels 0 and 1 will be preserved.)
	 * New channels will be filled with 0.
	 * @param channels The desired number of channels.
	 */
	void setChannels(int channels);

	/**
	 * Gets the number of samples in each channel.
	 * By default, the size is set to the buffer
	 * size of the audio host (E.G. a DAW.)
	 * garbage data.
	 * @return The number of samples in each channel.
	 */
	int getSize() {
		return m_size;
	}
	/**
	 * Gets the number of channels. By default,
	 * the number of channels is set to however
	 * many the audio host (E.G. a DAW) has
	 * requested.
	 * @return The number of channels.
	 */
	int getChannels() {
		return m_channels;
	}

	/**
	 * Sets the default number of samples a
	 * newly created AudioBuffer should have
	 * in each channel. This should only be
	 * used after finding out e.g. the buffer
	 * size requested of the synth from a
	 * DAW.
	 * @param size The desired number of samples in each channel.
	 */
	static void setDefaultSize(int size) {
		s_size = size;
	}
	/**
	 * Sets the default number of channels a
	 * newly created AudioBuffer should have.
	 * This should only be used after finding
	 * out e.g. the number of channels requested
	 * of the synth from a DAW.
	 * @param channels The desired number of channels.
	 */
	static void setDefaultChannels(int channels) {
		s_channels = channels;
	}
	/**
	 * Sets the default number of samples a
	 * newly created AudioBuffer should have
	 * in each channel.
	 * @return The default number of samples in each channel.
	 */
	static int getDefaultSize() {
		return s_size;
	}
	/**
	 * Sets the default number of samples a
	 * newly created AudioBuffer should have
	 * in each channel.
	 * @return The default number of samples in each channel.
	 */
	static int getDefaultSamples() {
		return s_size;
	}
	/**
	 * Gets the default number of channels a
	 * newly created AudioBuffer should have.
	 * @return The default number of channels.
	 */
	static int getDefaultChannels() {
		return s_channels;
	}
};

} /* namespace AtomSynth */

#endif /* SOURCE_TECHNICAL_AUDIOBUFFER_H_ */
