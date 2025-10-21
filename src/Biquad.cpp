/*
 *  Biquad.cpp
 *
 *  Created on: 20 Oct 2025
 *      Author: mchlp
 */

#include "Biquad.h"


Biquad::Biquad() :
    m_previousComponent1(0.0f),
    m_previousComponent2(0.0f),
    m_a1(0.0f),
    m_a2(0.0f),
    m_b0(1.0f),
    m_b1(0.0f),
    m_b2(0.0f)
{}

Biquad::Biquad(float a1, float a2, float b0, float b1, float b2) :
    m_previousComponent1(0.0f),
    m_previousComponent2(0.0f),
    m_a1(a1),
    m_a2(a2),
    m_b0(b0),
    m_b1(b1),
    m_b2(b2)
{}

float Biquad::processSample(float sample)
{
    if (m_bypass) return sample;

    float out = m_b0 * sample + m_previousComponent1;

    m_previousComponent1 = m_b1 * sample - m_a1 * out + m_previousComponent2;
    m_previousComponent2 = m_b2 * sample - m_a2 * out;

    return out;
}

Biquad Biquad::makeLowPass()
{
    return Biquad(
        -1.9398020364666517,
        0.9415753404872774,
        0.00044332600515640017,
        0.0008866520103128003,
        0.00044332600515640017
    );
}

