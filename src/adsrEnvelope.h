/*
 * adsrEnvelope.h
 *
 *  Created on: 25 Oct 2022
 *      Author: mchlp
 */

#ifndef ADSRENVELOPE_H_
#define ADSRENVELOPE_H_

#include "audioObject.h"

enum AdsrState
{
	Idle,
	Attack,
	Decay,
	Sustain,
	Release
};

class AdsrEnvelope : public AudioObject
{
public:
	AdsrEnvelope() = delete;
	AdsrEnvelope(AudioObject* pInput) :
		AudioObject(),
		m_pInput(pInput),
		m_state(Idle),
		m_currentPower(0.0f),
		m_attackRate(0.0000680f),
		m_decayRate(0.0000680f),
		m_releaseRate(0.00000680f),
		m_attackPower(0.5f),
		m_sustainPower(0.1f),
		m_target(0.0f)
	{};
	~AdsrEnvelope() = default;

	int generateBlock(unsigned int frameCount, float* buffer) override;

	void process();

	// set attack time in seconds
	void setAttackTime(float t) { m_attackRate = m_attackPower/(t * 44100.0f); }
	// set decay time in seconds
	void setDecayTime(float t) { m_decayRate = (m_attackPower - m_sustainPower)/(t * 44100.0f); }
	// set release time in seconds
	void setReleaseTime(float t) { m_releaseRate = m_sustainPower/(t * 44100.0f); }

	// set attack rate per sample
	void setAttackRate(float r) { m_attackRate = r; }
	// set decay rate per sample
	void setDecayRate(float r) { m_decayRate = r; }
	// set release rate per sample
	void setReleaseRate(float r) { m_releaseRate = r; }

	// press note
	void noteDown();
	// release note
	void noteUp();
private:
	AudioObject* m_pInput;
	AdsrState m_state;
	float m_currentPower;
	float m_attackRate;  // rate in samples
	float m_decayRate;  // rate in samples
	float m_releaseRate;  // rate in samples
	float m_attackPower;
	float m_sustainPower;
	float m_target;
};

#endif /* ADSRENVELOPE_H_ */
