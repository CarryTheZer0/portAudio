/*
 *  SquareWave.h
 *
 *  Created on: 21 Jan 2025
 *      Author: mchlp
 */

#ifndef SQUARE_WAVE_H_
#define SQUARE_WAVE_H_

#include "Oscillator.h"

class SquareWave : public Oscillator
{
public:
	SquareWave() :
		Oscillator(),
		m_power(0.5f)
	{};
	SquareWave(float frequency, float power) :
		Oscillator(frequency),
		m_power(power)
	{};

	~SquareWave() = default;

	void nextFrame() override;

	void setPower(float a);
private:
	float processSample(float sample) override;

	float m_power;
};

#endif /* SQUARE_WAVE_H_ */
