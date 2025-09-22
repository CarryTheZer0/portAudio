/*
 *  AllPassFilter.cpp
 *
 *  Created on: 26 Aug 2025
 *      Author: mchlp
 */

#include "AllPassFilter.h"


AllPassFilter::AllPassFilter() :
    m_feedback(0.5f)
{
    setSize(0);
}

AllPassFilter::AllPassFilter(unsigned int size, float feedback) :
    m_feedback(feedback)
{
    setSize(size);
}

void AllPassFilter::setSize(unsigned int bufferSize)
{
    if (bufferSize != m_bufferSize)
    {
        m_bufferIndex = 0;
        m_filterBuffer = std::vector<float>(bufferSize, 0.0f);
        m_bufferSize = bufferSize;
    }

    clear();
}

void AllPassFilter::clear()
{
    m_filterBuffer.clear();
}

float AllPassFilter::processSample(float sample)
{
    if (m_bypass || m_bufferSize == 0) return sample;

    // read previous delayed sample
    const float delayedSample = m_filterBuffer[m_bufferIndex];

    // write new delayed sample
    m_bufferIndex = (m_bufferIndex + 1) % m_bufferSize;
    m_filterBuffer[m_bufferIndex] = sample + (delayedSample * m_feedback.getValue());  
    
    // write processed sample
    return delayedSample + (sample * -m_feedback.getValueAndStep());
}
