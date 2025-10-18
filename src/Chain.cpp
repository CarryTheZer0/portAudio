/*
 *  Chain. cpp
 *
 *  Created on: 30 Aug 2025
 *      Author: mchlp
 */

#include "Chain.h"

int Chain::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    for (unsigned int index = 0; index < m_inputs.size(); index++)
        m_inputs[index]->processBlock(frameCount, channelCount, buffer);

    return 0;
}

void Chain::processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer)
{
    for (unsigned int index = 0; index < m_inputs.size(); index++)
        m_inputs[index]->processFrame(frameIndex, channelCount, buffer);
}

float Chain::processSample(float sample)
{
    sample = m_inputs[0]->processSample(sample);

    for (unsigned int index = 1; index < m_inputs.size(); index++)
        sample = m_inputs[index]->processSample(sample);

    return sample;
}

void Chain::nextFrame()
{
    for (unsigned int index = 0; index < m_inputs.size(); index++)
        m_inputs[index]->nextFrame();
}

void Chain::addInput(AudioObject* obj)
{
    m_inputs.push_back(obj);
}

