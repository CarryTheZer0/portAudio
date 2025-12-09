/*
 * oscillator.h
 *
 *  Created on: 14 Jan 2024
 *      Author: mchlp
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif /* M_PI */

#include "AudioObject.h"
#include "Ramp.h"

class Oscillator : public AudioObject
{
public:
    Oscillator() :
        AudioObject(),
        m_phase(0.0f),
        m_step(0.0f)
    {};

    Oscillator(float frequency) :
        AudioObject(),
        m_phase(0.0f),
        m_step(2 * M_PI / (44100/frequency))
    {
        setFrequency(frequency);
    };

    void setFrequency(float f) { m_step.setTarget(2 * M_PI / (44100/f), 0.1f); }
protected:
    float m_phase;
    Ramp m_step;
};

#endif /* OSCILLATOR_H_ */
