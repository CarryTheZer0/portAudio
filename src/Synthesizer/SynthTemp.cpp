/*
 *  SynthTemp.cpp
 *
 *  Created on: 04 Dec 2025
 *      Author: mchlp
 */

#include <cmath>

#include "SynthTemp.h"

#include "Sources/SineWave.h"
#include "Sources/SquareWave.h"

void SynthTemp::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    if ( m_bypass ) return;

    m_buffer = std::vector<float>(frameCount * channelCount, 0.0f);

    for (auto source : m_sources)
        source->processBlock(frameCount, channelCount, m_buffer);
    AudioObject::processBlock(frameCount, channelCount, buffer); 
}

void SynthTemp::processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer)
{
    for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
    {
        unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
        buffer[bufferIndex] = m_buffer[bufferIndex] * m_envelope.getValue() + buffer[bufferIndex];
    }
}

void SynthTemp::nextFrame()
{
    m_envelope.step();
}

void SynthTemp::addSource(std::shared_ptr<Oscillator> source)
{
    m_sources.push_back(source);
}

void SynthTemp::noteDown(float velocity)
{
    m_envelope.noteDown();
}

void SynthTemp::noteUp()
{
    m_envelope.noteUp();
}

bool SynthTemp::isPlaying()
{
    return m_envelope.getState() != AdsrState::Idle;
}

std::shared_ptr<SynthTemp> SynthTemp::makeNote(unsigned int id)
{
    float f0 = pow(2, ((float)(id - 69.0f)/12.0f)) * 440.0f;
    auto t = std::make_shared<SynthTemp>();
    std::shared_ptr<SquareWave> sine1 = std::make_shared<SquareWave>(f0, 0.3f);
    std::shared_ptr<SineWave> sine2 = std::make_shared<SineWave>(f0 * 0.5f, 0.5f);
    t->addSource(sine1);
    t->addSource(sine2);
    return t; 
};