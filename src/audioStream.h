/*
 * audioStream.h
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#ifndef AUDIOSTREAM_H_
#define AUDIOSTREAM_H_

#include <functional>
#include "portaudio.h"

const int BUFFER_SIZE = 1024;

class AudioObject;

class AudioStream
{
public:
	AudioStream() = delete;
	AudioStream(AudioObject* pInput) :
		m_pInput(pInput),
		m_bufferIndex(0),
		m_stream()
	{};
	~AudioStream() = default;

	void startPlayback();
	void stopPlayback();

	void openStream();
	void openStream(int inChannels, PaDeviceIndex inDevNum, PaTime inLatency,
					int outChannels, PaDeviceIndex outDevNum, PaTime outLatency);
	void closeStream();

	static int callback(const void* input, void* output,
			unsigned long frameCount,
			const PaStreamCallbackTimeInfo *timeInfo,
			PaStreamCallbackFlags statusFlags,
			void *userData)
	{
		// This breaks type safety but its ok
		AudioStream* audioStream = ((AudioStream*)userData);
	    return audioStream->generateBlock(input, output, frameCount, timeInfo, statusFlags);
	}

	int generateBlock(const void *input, void *output,
			unsigned long frameCount,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags);
private:
	AudioObject* m_pInput;
	float m_buffer[BUFFER_SIZE];
	unsigned int m_framesPerBuffer;
	unsigned int m_bufferIndex;
	PaStream* m_stream;

	PaStreamParameters m_outputParameters;
	PaStreamParameters m_inputParameters;
};

#endif /* AUDIOSTREAM_H_ */
