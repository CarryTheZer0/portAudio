/*
 *  KickTest.h
 *
 *  Created on: 26 Oct 2022
 *      Author: mchlp
 */

#ifndef KICKTEST_H_
#define KICKTEST_H_

#include "AudioObject.h"

#include "SineWave.h"
#include "AdsrEnvelope.h"

class KickTest : public AudioObject
{
public:
	KickTest() :
		AudioObject(),
		m_src(),
		m_currentFreq(0.0f),
		m_decayRate(0.0f),
		m_initFreq(400.0f),
		m_targetFreq(20.0f)
	{};
	~KickTest() = default;

	int processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;

	void setDecayTime(float t) { m_decayRate = (m_initFreq - m_targetFreq)/(t * 44100.0f); }

	void hit();
private:
	SineWave m_src;

	float m_currentFreq;
	float m_decayRate;
	float m_initFreq;
	float m_targetFreq;
};

#endif /* KICKTEST_H_ */
