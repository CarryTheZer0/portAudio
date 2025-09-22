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


class CombFilter : public AudioObject
{
public:
    CombFilter();
    CombFilter(unsigned int size);

    void setSize (unsigned int size);

    void clear();

    float processSample(float sample) override;

private:
    std::vector<float> m_filterBuffer;
    unsigned int m_bufferSize = 0, m_bufferIndex = 0;
};

#endif /* COMB_FILTER_H_ */