/*
 * adsrEnvelope.h
 *
 *  Created on: 25 Oct 2022
 *      Author: mchlp
 */

#ifndef ADSRENVELOPE_H_
#define ADSRENVELOPE_H_

#include "Ramp.h"

enum AdsrState
{
	Idle,
	Attack,
	Decay,
	Sustain,
	Release
};

class AdsrEnvelope : public Ramp
{
public:
	AdsrEnvelope():
		Ramp(0.0f),
		m_state(Idle),
		m_attackTime(0.1f),
		m_decayTime(0.5f),
		m_releaseTime(0.3f),
		m_attackPower(1.0f),
		m_sustainPower(0.6f)
	{};

	AdsrEnvelope(float value) :
		Ramp(value),
		m_state(Idle),
		m_attackTime(0.1f),
		m_decayTime(0.5f),
		m_releaseTime(0.3f),
		m_attackPower(1.0f),
		m_sustainPower(0.6f)
	{};
	~AdsrEnvelope() = default;

	float step() override;

	// set attack time in seconds
	void setAttackTime(float t) { m_attackTime = t; }
	// set decay time in seconds
	void setDecayTime(float t) { m_decayTime = t; }
	// set release time in seconds
	void setReleaseTime(float t) { m_releaseTime = t; }

	// press note
	void noteDown();
	// release note
	void noteUp();

	AdsrState getState() { return m_state; }
private:
	AdsrState m_state;
	float m_attackTime;
	float m_decayTime;  
	float m_releaseTime; 
	float m_attackPower;
	float m_sustainPower;
};

#endif /* ADSRENVELOPE_H_ */
