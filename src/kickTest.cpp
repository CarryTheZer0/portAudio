/*
 * kickTest.cpp
 *
 *  Created on: 26 Oct 2022
 *      Author: mchlp
 */

#include "KickTest.h"

int KickTest::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
	m_src.processBlock(frameCount, channelCount, buffer);

	m_currentFreq -= (m_decayRate * frameCount * channelCount);
	m_src.setFrequency(m_currentFreq);

	if (m_currentFreq <= m_targetFreq)
		m_currentFreq = m_targetFreq;

	return 0;
}

void KickTest::hit()
{
	m_currentFreq = m_initFreq;
}
