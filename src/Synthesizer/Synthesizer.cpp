/*
 *  Synth.cpp
 *
 *  Created on: 06 Nov 2025
 *      Author: mchlp
 */

#include <vector>

#include "Synthesizer.h"

Synthesizer::Synthesizer(std::shared_ptr<ISynthNote> source)
{
    addNote(0, source);
}

void Synthesizer::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    std::vector<unsigned int> deleteQueue;
    for ( std::pair<unsigned int, std::shared_ptr<ISynthNote>> current_pair : m_notes )
    {
        // if (current_pair.first == 0) continue;

        if (!current_pair.second->isPlaying() && current_pair.first != 0) { deleteQueue.push_back(current_pair.first); }
        current_pair.second->processBlock(frameCount, channelCount, buffer);
    }

    for ( unsigned int id : deleteQueue )
    {
        m_notes.erase(id);
    }
}

void Synthesizer::addNote(unsigned int id, std::shared_ptr<ISynthNote> source)
{
    m_notes.emplace(id, source);
}

void Synthesizer::noteDown(unsigned int id, float velocity)
{
    if(m_notes.find(id) == m_notes.end())
    {
        std::shared_ptr<ISynthNote> newNote = std::dynamic_pointer_cast<ISynthNote>(m_notes[0]->clone());
        newNote->init(id);
        m_notes.emplace(id, newNote);
    }
    m_notes[id]->noteDown(velocity);
}

void Synthesizer::noteUp(unsigned int id)
{
    if (m_notes.find(id) != m_notes.end()) m_notes[id]->noteUp();
}
