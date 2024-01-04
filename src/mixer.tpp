
template< int NINPUTS >
int Mixer<NINPUTS>::generateBlock(unsigned int frameCount, float* buffer)
{
    m_inputs.at(0)->generateBlock(frameCount, buffer);
    
    for (int i = 1; i < m_inputs.size(); i++)
    {
        float currBuffer[frameCount*2];
        m_inputs.at(i)->generateBlock(frameCount, currBuffer);
        for ( unsigned int i=0; i < frameCount; i++ )
        {
            buffer[i*2] += currBuffer[i*2];
            buffer[(i*2) + 1] += currBuffer[(i*2) + 1];
        }
    }

    return 0;
}

template< int NINPUTS >
void Mixer<NINPUTS>::setInput(AudioObject* obj, int index)
{
    m_inputs.insert(m_inputs.begin() + index, obj);
}
