/*
 *  Biquad.h
 *
 *  Created on: 20 Oct 2025
 *      Author: mchlp
 */

#ifndef BIQUAD_H_
#define BIQUAD_H_

#include "AudioObjectMono.h"

class Biquad : public AudioObjectMono
{
public:
    Biquad();
    Biquad(float a1, float a2, float b0, float b1, float b2);

    static std::shared_ptr<Biquad> makeLowPass();
private:
    float processSample(float sample) override;

    // In the difference equation
    // y[n] = b0x[n] + b1x[n-1] + b2x[n-2] + a1y[n-1] + a2y[n-2]
    // "previousComponent1" holds b1x[n-1] + a1x[n-1]
    // "previousComponent2" holds b2x[n-2] + a2x[n-2]
    float m_previousComponent1, m_previousComponent2;
    // coefficients. a is for the poles and b is for the zeros
    float m_a1, m_a2, m_b0, m_b1, m_b2;
};

#endif /* BIQUAD_H_ */
