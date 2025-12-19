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

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;

    void addInput(std::shared_ptr<AudioObject> obj);
private:
    std::vector<std::weak_ptr<AudioObject>> m_inputs;
};

#endif /* CHAIN_H_ */
