// /*
//  *  Reverb.h
//  *
//  *  Created on: 29 Aug 2025
//  *      Author: mchlp
//  */

#include "Reverb.h"

Reverb::Reverb(unsigned int channelCount)
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
                    combTunings[combIndex] + m_stereoSpread * channelIndex
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

int Reverb::processBlock(
    unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer
) {
		if (m_bypass) return 0;

		for( unsigned int frameIndex=0; frameIndex < frameCount; frameIndex++ )
		{
			nextFrame();
			for ( unsigned int channelIndex=0; channelIndex < channelCount; channelIndex++ )
			{
				unsigned int bufferIndex = (frameIndex * channelCount) + channelIndex;
				buffer[bufferIndex] = processSample(buffer[bufferIndex], channelIndex);
			}
		}

		return 0;
}

float Reverb::processSample(float sample, unsigned int channelIndex)
{
    if (m_bypass) return sample;

    sample = sample * 0.5f + m_chains[channelIndex]->processSample(sample) * 0.5f;

    return sample;
}

float Reverb::processSample(float sample)
{
    if (m_bypass) return sample;

    sample = sample * 0.5f + m_chains[0]->processSample(sample) * 0.5f;

    return sample;
}
