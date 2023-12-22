/*
 * kickTest.cpp
 *
 *  Created on: 26 Oct 2022
 *      Author: mchlp
 */

#include "kickTest.h"

int KickTest::generateBlock(unsigned int frameCount, float* buffer)
{
	m_src.generateBlock(frameCount, buffer);

	m_currentFreq -= m_decayRate * frameCount;
	m_src.setFrequency(m_currentFreq);

	if (m_currentFreq <= m_targetFreq)
		m_currentFreq = m_targetFreq;

	return 0;
}

void KickTest::hit()
{
	m_currentFreq = m_initFreq;
}

