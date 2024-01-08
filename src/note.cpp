
#include "note.h"

void Note::init()
{
    m_sine1.setFrequency(m_fundamental_frequency * 1);
    m_sine1.setPower(0.5f);
    m_output.setInput(&m_sine1, 0);
    
    m_sine2.setFrequency(m_fundamental_frequency * 2);
    m_sine2.setPower(0.4f);
    m_output.setInput(&m_sine2, 1);

    m_sine3.setFrequency(m_fundamental_frequency * 3);
    m_sine3.setPower(0.3f);
    m_output.setInput(&m_sine3, 2);

    m_sine4.setFrequency(m_fundamental_frequency * 4);
    m_sine4.setPower(0.2f);
    m_output.setInput(&m_sine4, 3);

    m_sine5.setFrequency(m_fundamental_frequency * 5);
    m_sine5.setPower(0.1f);
    m_output.setInput(&m_sine5, 4);
}