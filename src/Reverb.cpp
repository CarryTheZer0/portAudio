/*
 *  Reverb.h
 *
 *  Created on: 29 Aug 2025
 *      Author: mchlp
 */

#include "Reverb.h"

Reverb::Reverb(unsigned int channelCount) :
    m_dryGain(0.8f),
    m_wetGainMain(0.1f),
    m_wetGainOther(0.1f)
{
    setChannels(channelCount);
}

void Reverb::setChannels(unsigned int channelCount)
{
    /// todo clear chain/mixer

    for (int channelIndex = 0; channelIndex < channelCount; channelIndex++)
    {
        m_combMixers.emplace_back(std::make_shared<Mixer>());
        m_chains.emplace_back(std::make_shared<Chain>());

        m_chains[channelIndex]->addInput(m_combMixers[channelIndex].get());
        m_combs.emplace_back();

        for (int combIndex = 0; combIndex < m_combCount; combIndex++)
        {
            m_combs[channelIndex].emplace_back(
                std::make_shared<CombFilter>(
                    combTunings[combIndex] + m_stereoSpread * channelIndex, 0.8f, 0.5f
                )
            );
            m_combMixers[channelIndex]->addInput(m_combs[channelIndex][combIndex].get());
        }
        
        m_allpasses.emplace_back();

        for (int allPassIndex = 0; allPassIndex < m_allPassCount; allPassIndex++)
        {
            m_allpasses[channelIndex].emplace_back(
                std::make_shared<AllPassFilter>(
                    allPassTunings[allPassIndex] + m_stereoSpread * channelIndex, 0.5f
                )
            );
            m_chains[channelIndex]->addInput(m_allpasses[channelIndex][allPassIndex].get());
        }
    }
}

void Reverb::setDamping(float damping)
{
    int index = 0;
    for (auto channel : m_combs)
        for (auto comb : m_combs[index++])
            comb.get()->setDamping(damping);
}

void Reverb::setFeedback(float feedback)
{
    int index = 0;
    for (auto channel : m_combs)
        for (auto comb : m_combs[index++])
            comb.get()->setFeedback(feedback);
}

void Reverb::processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer)
{
    float outputs[channelCount]; 
    float outputsSummed = 0;
    
    for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
    {
        unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
        float newSample = m_chains[channelIndex]->processSample(buffer[bufferIndex]);
        outputs[channelIndex] = newSample;
        outputsSummed += newSample;
    }

    for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
    {
        unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
        
        float outputsOther = (outputsSummed - outputs[channelIndex]) / (channelCount - 1);
        
        buffer[bufferIndex] = 
            (outputs[channelIndex] * m_wetGainMain.getValue()) +
            (outputsOther * m_wetGainOther.getValue()) + 
            (buffer[bufferIndex] * m_dryGain.getValue());
    }
}

void Reverb::nextFrame()
{
    m_wetGainMain.step();
    m_wetGainOther.step();
    m_dryGain.step();
}
