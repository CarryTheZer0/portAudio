/*
 *  AudioStream.cpp
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#include <math.h>

#include "AudioStream.h"
#include "Graph.h"


AudioStream::~AudioStream()
{
	closeStream();
}

void AudioStream::startPlayback()
{
	PaError err;
	err = Pa_StartStream(m_stream);
	if( err != paNoError )
		{} // TODO throw error
}

void AudioStream::stopPlayback()
{
	PaError err;
	err = Pa_StopStream(m_stream);
	if( err != paNoError )
		{} // TODO throw error
}

void AudioStream::openStream()
{
	double srate = 44100;
	unsigned long framesPerBuffer = FRAME_COUNT;
	
	m_inChannelCount = 2;
	m_outChannelCount = 2;

	m_buffer = std::vector<float>(framesPerBuffer * m_outChannelCount, 0.0f);

	PaError err;
	err = Pa_OpenDefaultStream(&m_stream, m_inChannelCount, m_outChannelCount, paFloat32, srate, framesPerBuffer, callback, this);
	if( err != paNoError )
		{} // TODO throw error
}

void AudioStream::openStream(int inChannels, PaDeviceIndex inDevNum, PaTime inLatency,
							 int outChannels, PaDeviceIndex outDevNum, PaTime outLatency)
{
	double srate = 44100;
	unsigned long framesPerBuffer = FRAME_COUNT;

	m_buffer = std::vector<float>(framesPerBuffer * outChannels, 0.0f);

	m_inChannelCount = inChannels;
	m_inputParameters.channelCount = inChannels;
	m_inputParameters.device = inDevNum;
	m_inputParameters.hostApiSpecificStreamInfo = NULL;
	m_inputParameters.sampleFormat = paFloat32;
	m_inputParameters.suggestedLatency = inLatency;

	m_outChannelCount = outChannels;
	m_outputParameters.channelCount = outChannels;
	m_outputParameters.device = outDevNum;
	m_outputParameters.hostApiSpecificStreamInfo = NULL;
	m_outputParameters.sampleFormat = paFloat32;
	m_outputParameters.suggestedLatency = outLatency;

	PaError err;
	err = Pa_OpenStream(&m_stream, &m_inputParameters, &m_outputParameters, srate, framesPerBuffer, paNoFlag, callback, this);
	if( err != paNoError )
		{} // TODO throw error
}

void AudioStream::closeStream()
{
	PaError err;
	err = Pa_CloseStream(m_stream);
	if( err != paNoError )
		{} // TODO throw error
}

int AudioStream::generateBlock(const void* input, void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags)
{
    float* out = (float*)output;
	float* in = (float*)input;

    for( unsigned int i=0; i < frameCount; i++ )
    {
        float inValue = *in;
		in += m_inChannelCount;  // for now this is reading just the first channel
		
		// todo handle channels
		for (unsigned int channelIndex=0; channelIndex < m_outChannelCount; channelIndex++)
		{
			m_buffer[m_bufferIndex] = 0.0f;
			if (true)  // if we are recording, fill the buffer with input data
				m_buffer[m_bufferIndex] += inValue;
			m_bufferIndex++;
		}

		if (m_bufferIndex >= FRAME_COUNT * m_outChannelCount)
        {
        	m_bufferIndex = 0;
        	m_pInput->processBlock(FRAME_COUNT, m_outChannelCount, m_buffer);

			while (m_bufferIndex < m_buffer.size())
				*out++ = m_buffer[m_bufferIndex++];
			m_bufferIndex = 0;
        }
    }

	return paContinue;
}
