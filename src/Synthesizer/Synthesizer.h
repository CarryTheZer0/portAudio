/*
 *  Synthesizer.h
 *
 *  Created on: 06 Nov 2025
 *      Author: mchlp
 */

#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <unordered_map>
#include <memory>
#include <functional>

#include "AudioObject.h"
#include "AdsrEnvelope.h"

class ISynthNote : public AudioObject
{
public:
    ISynthNote() = default;

    virtual void noteDown(float velocity) = 0;
    virtual void noteUp() = 0;

    virtual bool isPlaying() = 0;
};

class Synthesizer : public AudioObject
{
public:
    Synthesizer() = default;
    Synthesizer(std::function<std::shared_ptr<ISynthNote>(unsigned int)> makeNote) :
        m_makeNote(makeNote)
    {}

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;

    void addNote(unsigned int id, std::shared_ptr<ISynthNote> note);
    void removeNote(unsigned int id);

    void noteDown(unsigned int id, float velocity);
    void noteUp(unsigned int id);
private:
    std::unordered_map<unsigned int, std::shared_ptr<ISynthNote>> m_notes;
    std::vector<unsigned int> m_deleteQueue;

    std::function<std::shared_ptr<ISynthNote>(unsigned int)> m_makeNote;
};

#endif /* SYNTHESIZER_H_ */
