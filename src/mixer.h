/*
 * mixer.h
 *
 *  Created on: 19 Dec 2023
 *      Author: mchlp
 */

#ifndef MIXER_H_
#define MIXER_H_

#include <vector>

#include "audioObject.h"

template< int NINPUTS >
class Mixer : public AudioObject
{
public:
    Mixer() = default;
    ~Mixer() = default;

    int generateBlock(unsigned int frameCount, float* buffer) override;
    void setInput(AudioObject* obj, int index);
private:
    std::vector<AudioObject*> m_inputs = std::vector<AudioObject*> (NINPUTS);
};

#include "mixer.tpp"

#endif /* MIXER_H_ */
