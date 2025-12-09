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
  
    std::shared_ptr<AudioObject> clone() override;
    void init(int id) override;

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;
    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;

    void nextFrame() override;

    void noteDown(float velocity) override;
    void noteUp() override;

    bool isPlaying() override;

    void addSource(std::shared_ptr<Oscillator> source, int harmonic);
private:
    std::vector<std::pair<int, std::shared_ptr<Oscillator>>> m_sources;
    std::vector<float> m_buffer;
    AdsrEnvelope m_envelope;
};

#endif /* SYNTH_TEMP_H_ */
