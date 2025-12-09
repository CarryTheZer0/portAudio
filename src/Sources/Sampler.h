/*
 *  Sampler.h
 *
 *  Created on: 08 Dec 2025
 *      Author: mchlp
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "AudioObject.h"

// TODO WIP

class Sampler : public AudioObject
{
public:
    Sampler() :
        m_nextSample(0)
    {}

    Sampler(const char* path) :
        m_nextSample(0)
    {
        loadSample(path);
    }

    void loadSample(const char* path);
    void restart() { m_nextSample = 0; }
private:
    float processSample(float sample) override;

    int m_nextSample;
    std::vector<float> m_buffer;
};

#endif /* SAMPLER_H_ */
