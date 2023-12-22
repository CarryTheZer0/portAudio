/*
 * sine_wave.cpp
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif /* M_PI */

#include <math.h>

#include "sine_wave.h"

int SineWave::generateBlock(unsigned int frameCount, float* buffer)
{
    for( unsigned int i=0; i < frameCount; i++ )
    {
        float amplitude = sin(m_phase) * m_power;
    	buffer[i*2] = amplitude;  // left channel
    	buffer[(i*2) + 1] = amplitude;  // right channel

        m_phase += m_step;
    }

	return 0;
}

void SineWave::setFrequency(float f)
{
	m_step = 2 * M_PI / (44100/f);
}

void SineWave::setPower(float a)
{
    m_power = a;
}
