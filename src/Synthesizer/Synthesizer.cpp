/*
 *  Synth.cpp
 *
 *  Created on: 06 Nov 2025
 *      Author: mchlp
 */

#include <vector>

#include "Synthesizer.h"

void Synthesizer::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    for ( std::pair<unsigned int, std::shared_ptr<ISynthNote>> current_pair : m_notes )
    {
        if (!current_pair.second->isPlaying()) removeNote(current_pair.first);
        current_pair.second->processBlock(frameCount, channelCount, buffer);
    }

    for ( unsigned int id : m_deleteQueue )
    {
        m_notes.erase(id);
    }
}

void Synthesizer::addNote(unsigned int id, std::shared_ptr<ISynthNote> source)
{
    m_notes.emplace(id, source);
}

void Synthesizer::removeNote(unsigned int id)
{
    m_deleteQueue.push_back(id);
}

void Synthesizer::noteDown(unsigned int id, float velocity)
{
    if(m_makeNote && m_notes.find(id) == m_notes.end())
    {
        std::shared_ptr<ISynthNote> newNote = m_makeNote(id);
        addNote(id, newNote);
        m_notes[id]->noteDown(velocity);
    } 
    else if (m_notes.find(id) != m_notes.end())
    {
        m_notes[id]->noteDown(velocity);
    }
}

void Synthesizer::noteUp(unsigned int id)
{
    if (m_notes.find(id) != m_notes.end()) m_notes[id]->noteUp();
}
