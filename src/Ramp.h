/*
 *  Ramp.h
 *
 *  Created on: 30 Aug 2025
 *      Author: mchlp
 */

#ifndef RAMP_H_
#define RAMP_H_


class Ramp
{
public:
    Ramp() = default;
    Ramp(float value);

    void step();

	void setTarget(float target);
    void setTarget(float target, float time);

    void setRate(float r) { m_rate = r; }
    void setTime(float t) { m_time = t; }

    float getValue() { return m_currentValue; }
    float getValueAndStep();
private:
    float m_rate;
    float m_time;

    float m_targetValue;
    float m_currentValue;
    unsigned int m_stepsRemaining; 
};

#endif /* RAMP_H_ */