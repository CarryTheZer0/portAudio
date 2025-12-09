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

const int FRAME_COUNT = 1024;  // power of 2 please

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
	~AudioStream();

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
		AudioStream* audioStream = ((AudioStream*)userData);
	    return audioStream->generateBlock(input, output, frameCount, timeInfo, statusFlags);
	}

	int generateBlock(const void *input, void *output,
			unsigned long frameCount,
			const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags);
private:
	AudioObject* m_pInput;
	std::vector<float> m_buffer;
	unsigned int m_framesPerBuffer;
	unsigned int m_bufferIndex;
	unsigned int m_inChannelCount;
	unsigned int m_outChannelCount;
	
	PaStream* m_stream;

	PaStreamParameters m_outputParameters;
	PaStreamParameters m_inputParameters;
};

#endif /* AUDIOSTREAM_H_ */
