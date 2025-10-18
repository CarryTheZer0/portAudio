/*
 *  CombFilter.cpp
 *
 *  Created on: 28 Aug 2025
 *      Author: mchlp
 */

#include "CombFilter.h"


CombFilter::CombFilter() :
    m_previousSample(0.0f),
    m_damping(0.0f),
    m_feedback(0.5f)
{
    setSize(0);
}

CombFilter::CombFilter(unsigned int size, float damping, float feedback) :
    m_previousSample(0.0f),
    m_damping(damping),
    m_feedback(feedback)
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
    // when damping is 0, m_previousSample = delayedSample i.e. a normal comb filter
    m_previousSample = (delayedSample * (1.0f - m_damping.getValue())) + 
                       (m_previousSample * m_damping.step());

    // write new delayed sample
    m_filterBuffer[m_bufferIndex] = sample + (m_previousSample * m_feedback.step());  
    m_bufferIndex = (m_bufferIndex + 1) % m_bufferSize;

    // write processed sample
    return delayedSample;
}
