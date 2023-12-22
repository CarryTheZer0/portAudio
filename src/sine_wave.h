/*
 * sine_wave.h
 *
 *  Created on: 21 Oct 2022
 *      Author: mchlp
 */

#ifndef SINE_WAVE_H_
#define SINE_WAVE_H_

#include "audioObject.h"

class SineWave : public AudioObject
{
public:
	SineWave() :
		AudioObject(),
		m_phase(0.0f),
		m_step(0.0f),
		m_power(0.5f)
	{};
	SineWave(float frequency, float power) :
		AudioObject(),
		m_phase(0.0f),
		m_step(0.0f),
		m_power(power)
	{
		setFrequency(frequency);
	};

	~SineWave() = default;

	int generateBlock(unsigned int frameCount, float* buffer) override;

	void setFrequency(float f);
	void setPower(float a);
private:
	float m_phase;
	float m_step;
	float m_power;
};

#endif /* SINE_WAVE_H_ */
