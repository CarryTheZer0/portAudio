/*
 *  SquareWave.cpp
 *
 *  Created on: 21 Jan 2025
 *      Author: mchlp
 */

#include <math.h>

#include "SquareWave.h"


float SquareWave::processSample(float sample)
{
    float amplitude = (1.0f ? m_phase > M_PI : 0.0f) * m_power;

	return amplitude;
}

void SquareWave::nextFrame()
{
    m_phase += m_step.getValueAndStep();
    if (m_phase > 2 * M_PI) m_phase -= 2 * M_PI;
}

void SquareWave::setPower(float a)
{
    m_power = a;
}
