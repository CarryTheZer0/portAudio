/*
 *  WhiteNoise.h
 *
 *  Created on: 20 Oct 2025
 *      Author: mchlp
 */

#include <random> 

#include "AudioObject.h"
#include "Ramp.h"

class WhiteNoise : public AudioObject
{
public:
    WhiteNoise();

    float processSample(float sample) override;
private:
    Ramp m_power;
    std::default_random_engine m_generator;
    std::uniform_real_distribution<float> m_distribution;
};
