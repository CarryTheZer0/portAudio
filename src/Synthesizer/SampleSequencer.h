/*
 *  SampleSequencer.h
 *
 *  Created on: 07 Dec 2025
 *      Author: mchlp
 */

#ifndef SAMPLE_SEQUNCER_H_
#define SAMPLE_SEQUNCER_H_

#include "Synthesizer.h"
#include "Sources/Sampler.h"

class SampleSequencer : public ISynthNote
{
public:
    SampleSequencer() = default;

    SampleSequencer(const char* path)
    {
        setSource(path);
    }

    std::shared_ptr<AudioObject> clone() override;
    void init(int id) override;

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;
    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;

    void nextFrame() override;

    void noteDown(float velocity) override;
    void noteUp() override;

    bool isPlaying() override;

    void setSource(const char* path);
private:
    Sampler m_sample;
};

#endif /* SAMPLE_SEQUNCER_H_ */
