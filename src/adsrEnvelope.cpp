/*
 * adsrEnvelope.cpp
 *
 *  Created on: 25 Oct 2022
 *      Author: mchlp
 */

#include "adsrEnvelope.h"

int AdsrEnvelope::generateBlock(unsigned int frameCount, float* buffer)
{
	m_pInput->generateBlock(frameCount, buffer);

    for( unsigned int i=0; i < frameCount; i++ )
    {
    	process();

    	buffer[i*2] *= m_currentPower;  // left channel
    	buffer[(i*2) + 1] *= m_currentPower;  // right channel
    }

	return 0;
}

void AdsrEnvelope::process()
{
	switch(m_state)
	{
	case Attack:
	{
		m_currentPower += m_attackRate;
		if (m_currentPower > m_target)
		{
			m_state = Decay;
			m_target = m_sustainPower;
		}
		break;
	}
	case Decay:
	{
		m_currentPower -= m_decayRate;
		if (m_currentPower < m_target)
		{
			m_state = Sustain;
		}
		break;
	}
	case Release:
		m_currentPower -= m_releaseRate;
		if (m_currentPower <= 0.0f)
		{
			m_currentPower = 0.0f;
			m_state = Idle;
		}
		break;
	default:
		break;
	}
}

void AdsrEnvelope::noteDown()
{
	m_state = Attack;
	m_target = m_attackPower;
}

void AdsrEnvelope::noteUp()
{
	m_state = Release;
	m_target = 0.0f;
}
