/*
 *  AudioObject.h
 *
 *  Created on: 24 Oct 2022
 *      Author: mchlp
 */

#ifndef AUDIOOBJECT_H_
#define AUDIOOBJECT_H_

#include <vector>
#include <memory>

class AudioObject
{
public:
	AudioObject() = default;
	virtual ~AudioObject() = default;

	virtual std::shared_ptr<AudioObject> clone() 
	{
		return std::make_shared<AudioObject>(*this);
	}

	/**
	 * Process a block of samples with interleaved channels.
	 * @param frameCount 	The number of frames in the buffer.
	 * @param channelCount 	The number of channels per frame.
	 * @param buffer 		The buffer to be filled.
	 */
	virtual void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) 
	{
		if (m_bypass) return;

		for( unsigned int frameIndex=0; frameIndex < frameCount; frameIndex++ )
		{
			nextFrame();
			processFrame(frameIndex, channelCount, buffer);
		}
	}

	/**
	 * Process a block of samples with interleaved channels.
	 * @param frameIndex 	The index of the first sample in the frame.
	 * @param channelCount 	The number of channels in the frame.
	 * @param buffer 		The buffer to be filled.
	 */
	virtual void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer)
	{
		for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
		{
			unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
			buffer[bufferIndex] = processSample(buffer[bufferIndex]);
		}
	}

	/**
	 * Update parameters bewteen frames.
	 */
	virtual void nextFrame() {}

	void setBypass(bool bypass) { m_bypass = bypass; }
protected:
	bool m_bypass = false;

	/**
	 * Process a single sample.
	 * @param sample 	The input sample.
	 * @return 			The output sample.
	 */
	virtual float processSample(float sample) { return sample; }
};

#endif /* AUDIOOBJECT_H_ */
