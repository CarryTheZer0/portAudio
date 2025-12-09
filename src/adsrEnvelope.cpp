/*
 *  AdsrEnvelope.cpp
 *
 *  Created on: 25 Oct 2022
 *      Author: mchlp
 */

#include "AdsrEnvelope.h"


float AdsrEnvelope::step()
{
	Ramp::step();
	
	switch(m_state)
	{
	case Attack:
	{
		if (m_stepsRemaining <= 0)
		{
			m_state = Decay;
		}
		break;
	}
	case Decay:
	{
		if (m_stepsRemaining <= 0)
		{
			m_state = Sustain;
			setTarget(m_sustainPower, m_decayTime);
		}
		break;
	}
	case Release:
	{
		if (m_stepsRemaining <= 0)
		{
			m_state = Idle;
		}
		break;
	}
	default:
		break;
	}

	return m_currentValue;
}

void AdsrEnvelope::noteDown()
{
	m_state = Attack;
	setTarget(m_attackPower, m_attackTime);
}

void AdsrEnvelope::noteUp()
{
	m_state = Release;
	setTarget(0.0f, m_releaseTime);
}
