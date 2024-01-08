/*
 * mixer.tpp
 *
 *  Created on: 19 Dec 2023
 *      Author: mchlp
 */


template< int NINPUTS >
int Mixer<NINPUTS>::generateBlock(unsigned int frameCount, float* buffer)
{
    // todo check m_inputs' elements are initialised

    m_inputs.at(0)->generateBlock(frameCount, buffer);

    for (int i = 1; i < NINPUTS; i++)
    {
        float currBuffer[frameCount*2];
        m_inputs.at(i)->generateBlock(frameCount, currBuffer);
        for ( unsigned int i=0; i < frameCount * 2; i++ )
        {
            buffer[i] += currBuffer[i];
        }
    }

    return 0;
}

template< int NINPUTS >
void Mixer<NINPUTS>::setInput(AudioObject* obj, int index)
{
    m_inputs.insert(m_inputs.begin() + index, obj);
}
