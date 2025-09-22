/*
 *  CombFilter.cpp
 *
 *  Created on: 28 Aug 2025
 *      Author: mchlp
 */

#include "CombFilter.h"


CombFilter::CombFilter()
{
    setSize(0);
}

CombFilter::CombFilter(unsigned int size)
{
    setSize(size);
}

void CombFilter::setSize(unsigned int bufferSize)
{
    if (bufferSize != m_bufferSize)
    {
        m_bufferIndex = 0;
        m_filterBuffer = std::vector<float>(bufferSize, 0.0f);
        m_bufferSize = bufferSize;
    }

    clear();
}

void CombFilter::clear()
{
    m_filterBuffer.clear();
}

float CombFilter::processSample(float sample)
{
    // read previous delayed sample
    const float delayedSample = m_filterBuffer[m_bufferIndex];

    // write new delayed sample
    m_bufferIndex = (m_bufferIndex + 1) % m_bufferSize;
    m_filterBuffer[m_bufferIndex] = sample;  
    
    // write processed sample
    return delayedSample + (sample * -0.5f);
}
