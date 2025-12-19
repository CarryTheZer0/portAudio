/*
 *  SampleSequencer.cpp
 *
 *  Created on: 08 Dec 2025
 *      Author: mchlp
 */

#include "SampleSequencer.h"

void SampleSequencer::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    if (m_bypass) return;

    m_sample.processBlock(frameCount, channelCount, buffer);
}

void SampleSequencer::noteDown(float velocity)
{
    m_sample.restart();
    setBypass(false);
}

void SampleSequencer::noteUp()
{
    setBypass(true);
}

bool SampleSequencer::isPlaying()
{
    return true;
}

void SampleSequencer::setSource(const char* path)
{
    m_sample.loadSample(path);
}
