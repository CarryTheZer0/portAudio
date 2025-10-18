/*
 *  CombFilter.h
 *
 *  Created on: 28 Aug 2025
 *      Author: mchlp
 */

#ifndef COMB_FILTER_H_
#define COMB_FILTER_H_

#include <vector>

#include "AudioObject.h"
#include "Ramp.h"

class CombFilter : public AudioObject
{
public:
    CombFilter();
    CombFilter(unsigned int size, float damping=0.0f, float feedback=0.5f);

    void setSize(unsigned int size);

    void clear();

    float processSample(float sample) override;

    void setDamping(float damping) { m_damping.setTarget(damping, 0.1f); }
    void setFeedback(float feedback) { m_feedback.setTarget(feedback, 0.1f); }
private:
    std::vector<float> m_filterBuffer;
    unsigned int m_bufferSize = 0, m_bufferIndex = 0;

    float m_previousSample;
    Ramp m_damping;
    Ramp m_feedback;
};

#endif /* COMB_FILTER_H_ */