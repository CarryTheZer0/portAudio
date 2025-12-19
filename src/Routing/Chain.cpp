/*
 *  Chain. cpp
 *
 *  Created on: 30 Aug 2025
 *      Author: mchlp
 */

#include "Chain.h"

void Chain::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    if (m_bypass) return;

    for (unsigned int index = 0; index < m_inputs.size(); index++)
        m_inputs[index].lock()->processBlock(frameCount, channelCount, buffer);

    return;
}

void Chain::addInput(std::shared_ptr<AudioObject> obj)
{
    m_inputs.push_back(obj);
}

