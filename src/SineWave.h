/*
 *  SineWave.h
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#ifndef SINE_WAVE_H_
#define SINE_WAVE_H_

#include "Oscillator.h"

class SineWave : public Oscillator
{
public:
	SineWave() :
		Oscillator(),
		m_phase(0.0f),
		m_power(0.5f)
	{};
	SineWave(float frequency, float power) :
		Oscillator(frequency),
		m_phase(0.0f),
		m_power(power)
	{};

	~SineWave() = default;

	float processSample(float sample) override;
	void nextFrame() override;

	void setPower(float a);
private:
	float m_phase;
	float m_power;
};

#endif /* SINE_WAVE_H_ */
