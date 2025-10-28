/*
 *  Chain.h
 *
 *  Created on: 30 Aug 2025
 *      Author: mchlp
 */

#ifndef CHAIN_H_
#define CHAIN_H_

#include <vector>

#include "AudioObject.h"

class Chain : public AudioObject
{
public:
    Chain() = default;
    ~Chain() = default;

    std::vector<float> processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> buffer) override;
    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;
    float processSample(float sample) override;
    void nextFrame() override;

    void addInput(AudioObject* obj);
private:
    std::vector<AudioObject*> m_inputs;
};

#endif /* CHAIN_H_ */
