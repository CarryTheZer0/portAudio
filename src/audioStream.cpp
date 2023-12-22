/*
 * audioStream.cpp
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#include <math.h>

#include "audioStream.h"
#include "audioObject.h"

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
	unsigned long framesPerBuffer = BUFFER_SIZE / 2;

	PaError err;
	err = Pa_OpenDefaultStream(&m_stream, 0, 2, paFloat32, srate, framesPerBuffer, callback, this);
	if( err != paNoError )
		{} // TODO throw error
}

void AudioStream::openStream(int inChannels, PaDeviceIndex inDevNum, PaTime inLatency,
							 int outChannels, PaDeviceIndex outDevNum, PaTime outLatency)
{
	double srate = 44100;
	unsigned long framesPerBuffer = BUFFER_SIZE / 2;
	
	m_inputParameters.channelCount = inChannels;
	m_inputParameters.device = inDevNum;
	m_inputParameters.hostApiSpecificStreamInfo = NULL;
	m_inputParameters.sampleFormat = paFloat32;
	m_inputParameters.suggestedLatency = inLatency;

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
        *out++ = m_buffer[m_bufferIndex++];  // left channel
        *out++ = m_buffer[m_bufferIndex++];  // right channel
        if (m_bufferIndex >= BUFFER_SIZE)
        {
        	m_bufferIndex = 0;
        	m_pInput->generateBlock(BUFFER_SIZE / 2, m_buffer);
        }

		// float curr = *in;
		// in += 2;

		// *out++ = curr;  // left channel
        // *out++ = curr;  // right channel
    }

	return paContinue;
}



