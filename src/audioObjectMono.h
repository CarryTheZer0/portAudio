/*
 *  AudioObject.h
 *
 *  Created on: 24 Oct 2022
 *      Author: mchlp
 */

#ifndef AUDIO_OBJECT_MONO_H_
#define AUDIO_OBJECT_MONO_H_

#include "AudioObject.h"

class AudioObjectMono : public AudioObject
{
public:
	AudioObjectMono() :
		m_offset(0)
	{}

	AudioObjectMono(int offset) :
		m_offset(offset)
	{}
	
	/**
	 * Process a block of samples with interleaved channels.
	 * @param frameCount 	The number of frames in the buffer.
	 * @param channelCount 	The number of channels per frame.
	 * @param buffer 		The buffer to be filled.
	 */
	virtual void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override
	{
		if (m_bypass) return;

		for( unsigned int frameIndex=0; frameIndex < frameCount; frameIndex++ )
		{
			nextFrame();
			processFrame(frameIndex, channelCount, buffer);
		}
	}

	/**
	 * Process one sample in the first channel.
	 * @param frameIndex 	The index of the current frame.
	 * @param channelCount 	The number of channels in the frame.
	 * @param buffer 		The buffer to be filled.
	 */
	virtual void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override
	{
		unsigned int bufferIndex = (frameIndex * channelCount) + m_offset;
		buffer[bufferIndex] = processSample(buffer[bufferIndex]);
	}

	void setOffset(int offset) { m_offset = offset; }
private:
	int m_offset;
};

#endif /* AUDIO_OBJECT_MONO_H_ */
