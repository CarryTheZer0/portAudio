/*
 *  Mixer. cpp
 *
 *  Created on: 19 Dec 2023
 *      Author: mchlp
 */

#include "Mixer.h"


void Mixer::processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer)
{
    for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
    {
        unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
        float sample = m_inputs[0]->processSample(buffer[bufferIndex]);

        for (unsigned int index = 1; index < m_inputs.size(); index++)
            sample += m_inputs[index]->processSample(buffer[bufferIndex]);

        buffer[bufferIndex] = sample;
    }
}

float Mixer::processSample(float sample)
{
    float outSample = 0.0f;
    sample = m_inputs[0]->processSample(sample);

    for (unsigned int index = 1; index < m_inputs.size(); index++)
        outSample += m_inputs[index]->processSample(sample);

    return outSample;
}

void Mixer::nextFrame()
{
    for (unsigned int index = 0; index < m_inputs.size(); index++)
        m_inputs[index]->nextFrame();
}

void Mixer::addInput(AudioObject* obj)
{
    m_inputs.push_back(obj);
}
