/*
 *  Mixer.h
 *
 *  Created on: 19 Dec 2023
 *      Author: mchlp
 */

#ifndef MIXER_H_
#define MIXER_H_

#include <vector>

#include "AudioObject.h"

class Mixer : public AudioObject
{
public:
    Mixer() = default;
    ~Mixer() = default;

    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;
    float processSample(float sample) override;
    void nextFrame() override;

    void addInput(AudioObject* obj);
private:
    std::vector<AudioObject*> m_inputs;
};

#endif /* MIXER_H_ */
