/*
 *  WhiteNoise.cpp
 *
 *  Created on: 20 Oct 2025
 *      Author: mchlp
 */

#include "WhiteNoise.h"

WhiteNoise::WhiteNoise() :
    m_power(0.3f)
{
    m_distribution = std::uniform_real_distribution<float>(-1.0f, 1.0f);
}

float WhiteNoise::processSample(float sample)
{
    float amplitude = m_power.step() * m_distribution(m_generator);
    return sample + amplitude;
}

void WhiteNoise::setPower(float a)
{
    m_power.setTarget(a, 0.1f);
}
