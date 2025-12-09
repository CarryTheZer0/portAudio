/*
 *  SineWave.cpp
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#include <math.h>

#include "SineWave.h"


std::shared_ptr<AudioObject> SineWave::clone()
{
    return std::make_shared<SineWave>(*this);
}

float SineWave::processSample(float sample)
{
	float ampltude = sin(m_phase) * m_power;
    return sample + ampltude;
}

void SineWave::nextFrame()
{
    m_phase += m_step.step();
    if (m_phase > 2 * M_PI) m_phase -= 2 * M_PI;
}

void SineWave::setPower(float a)
{
    m_power = a;
}