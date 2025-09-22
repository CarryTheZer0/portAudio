/*
 *  Ramp.cpp
 *
 *  Created on: 30 Aug 2025
 *      Author: mchlp
 */

#include "Ramp.h"


Ramp::Ramp(float value) :
    m_currentValue(value),
    m_targetValue(value),
    m_rate(0.0f),
    m_stepsRemaining(0),
    m_time(0.0f)
{}

void Ramp::step()
{
    if (m_stepsRemaining <= 0) return;
    
    m_currentValue += m_rate;
    m_stepsRemaining--;
}

void Ramp::setTarget(float target)
{
    m_targetValue = target;
    m_rate = (m_targetValue - m_currentValue)/(m_time * 44100.0f);
    m_stepsRemaining = (int)(m_time * 44100.0f);
}

void Ramp::setTarget(float target, float time)
{
    setTime(time);
    setTarget(target);
}

float Ramp::getValueAndStep()
{
    step();
    return m_currentValue;
}
