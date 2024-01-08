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

class Note
{
public:
    Note():
        m_fundamental_frequency(220.0f)
    {init();}

    Note(float frequency) :
        m_fundamental_frequency(frequency)
    {init();}
    ~Note() = default;

    void init();
    Mixer<5>* get_pOutput() {return &m_output;}

private:
    float m_fundamental_frequency;
    Mixer<5> m_output;
    SineWave m_sine1;
    SineWave m_sine2;
    SineWave m_sine3;
    SineWave m_sine4;
    SineWave m_sine5;
};

#endif /* NOTE_H_ */
