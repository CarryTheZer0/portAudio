/*
 *  Reverb.h
 *
 *  Created on: 29 Aug 2025
 *      Author: mchlp
 */

#include <memory>

#include "CombFilter.h"
#include "AllPassFilter.h"
#include "Mixer.h"
#include "Chain.h"

#include "AudioObject.h"

static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
static const short allPassTunings[] = { 556, 441, 341, 225 };

class Reverb : public AudioObject
{
public:
    Reverb() = default;
    Reverb(unsigned int channelCount);

    void setChannels(unsigned int channelCount);
    
    void processFrame(unsigned int frameIndex, unsigned int channelCount, std::vector<float> &buffer) override;
    void nextFrame() override;

    void setDamping(float damping);
    void setFeedback(float feedback);

    void setWetGain1(float gain) { m_wetGainMain.setTarget(gain, 0.1f); }
    void setWetGain2(float gain) { m_wetGainOther.setTarget(gain, 0.1f); }
    void setDryGain(float gain) { m_dryGain.setTarget(gain, 0.1f); }
private:
    const static int m_combCount = 8;
    const static int m_allPassCount = 4; 

    int m_stereoSpread;

    Ramp m_dryGain;
    Ramp m_wetGainMain;
    Ramp m_wetGainOther;

    AudioObject* m_pInput;

    std::vector<std::shared_ptr<Mixer>> m_combMixers;
    std::vector<std::shared_ptr<Chain>> m_chains;

    std::vector<std::vector<std::shared_ptr<CombFilter>>> m_combs;
    std::vector<std::vector<std::shared_ptr<AllPassFilter>>> m_allpasses;
};
