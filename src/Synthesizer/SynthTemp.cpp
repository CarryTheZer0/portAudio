/*
 *  SynthTemp.cpp
 *
 *  Created on: 04 Dec 2025
 *      Author: mchlp
 */

#include <cmath>

#include "SynthTemp.h"

std::shared_ptr<AudioObject> SynthTemp::clone()
{
    std::shared_ptr<SynthTemp> newNote = std::make_shared<SynthTemp>();
    for (auto source : m_sources)
    {
        std::shared_ptr<Oscillator> newSource = std::dynamic_pointer_cast<Oscillator>(source.second->clone());
        newNote->addSource(newSource, source.first);
    }
    return newNote;
}

void SynthTemp::init(int id) 
{
    float f0 = pow(2, ((float)(id - 69.0f)/12.0f)) * 440.0f;
    for (auto source : m_sources)
    {
        source.second->setFrequency(source.first * f0);
    }
}

void SynthTemp::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    m_buffer = std::vector<float>(frameCount * channelCount, 0.0f);

    for (auto source : m_sources)
        source.second->processBlock(frameCount, channelCount, m_buffer);
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

void SynthTemp::addSource(std::shared_ptr<Oscillator> source, int harmonic)
{
    m_sources.push_back(std::make_pair(harmonic + 1, source));
    init(0);
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