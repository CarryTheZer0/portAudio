/*
 *  AllPassFilter.h
 *
 *  Created on: 26 Aug 2025
 *      Author: mchlp
 */

#ifndef ALL_PASS_FILTER_H_
#define ALL_PASS_FILTER_H_

#include <vector>

#include "AudioObject.h"
#include "Ramp.h"


class AllPassFilter : public AudioObject
{
public:
    AllPassFilter();
    AllPassFilter(unsigned int size, float feedback);

    void setSize (unsigned int size);

    void clear();

    float processSample(float sample) override;

    void setFeedback(float f) { m_feedback.setTarget(f, 0.1f); }
private:
    std::vector<float> m_filterBuffer;
    unsigned int m_bufferSize = 0, m_bufferIndex = 0;

    Ramp m_feedback;
};

#endif /* ALL_PASS_FILTER_H_ */