/*
 *  SynthTemp.h
 *
 *  Created on: 03 Dec 2025
 *      Author: mchlp
 */

#ifndef SYNTH_TEMP_H_
#define SYNTH_TEMP_H_
 
#include "Synthesizer.h"

#include "Sources/Oscillator.h"

class SynthTemp : public ISynthNote
{
public:
    SynthTemp() : 
        m_envelope(0.0f)
    {}

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;
    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;

    void nextFrame() override;

    void noteDown(float velocity) override;
    void noteUp() override;

    bool isPlaying() override;

    void addSource(std::shared_ptr<Oscillator> source);

    static std::shared_ptr<SynthTemp> makeNote(unsigned int id);
private:
    std::vector<std::shared_ptr<Oscillator>> m_sources;
    std::vector<float> m_buffer;
    AdsrEnvelope m_envelope;
};

#endif /* SYNTH_TEMP_H_ */
