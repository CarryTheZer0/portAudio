/*
 * note.h
 *
 *  Created on: 05 Jan 2024
 *      Author: mchlp
 */

#ifndef NOTE_H_
#define NOTE_H_

#include <vector>

#include "mixer.h"
#include "sine_wave.h"
#include "adsrEnvelope.h" 

class Note
{
public:
    Note():
        m_fundamental_frequency(220.0f),
        m_envelope(&m_mixer)
    {init();}

    Note(float frequency) :
        m_fundamental_frequency(frequency),
        m_envelope(&m_mixer)
    {init();}
    ~Note() = default;

    void init();
    void noteDown() { m_envelope.noteDown(); }
    void noteUp() { m_envelope.noteUp(); }

    AdsrEnvelope* get_pOutput() {return &m_envelope;}

private:
    float m_fundamental_frequency;
    Mixer<5> m_mixer;
    SineWave m_sine1;
    SineWave m_sine2;
    SineWave m_sine3;
    SineWave m_sine4;
    SineWave m_sine5;
    AdsrEnvelope m_envelope;
};

#endif /* NOTE_H_ */
