/*
 *  Graph.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: mchlp
 */

#include <queue>
#include <algorithm>

#include "Graph.h"

std::shared_ptr<AudioObject> Graph::clone() 
{
    // TODO shouldn't clone Graph as it uses weak pointers i.e. does not own the AudioObjects in m_nodes
    // Instead clone the objects it points to and make a new Graph for them

    std::shared_ptr<Graph> newGraph = std::make_shared<Graph>(*this);
    for (auto node : m_nodes)
    {
        newGraph->m_nodes[node.first] = Node(node.second.process.lock()->clone());
    }
    return newGraph;
}

void Graph::updateMixingOrder(int currentNode)
{
    std::queue<int> nodeQueue;
    std::map<int, bool> visited;
    int currentStage = 0;

    visited[currentNode] = true;
    nodeQueue.push(currentNode);

    while (!nodeQueue.empty())
    {
        // take the next node
        currentNode = nodeQueue.front();
        nodeQueue.pop();

        for (int i = 0; i < m_parents[currentNode].size(); i++)
        {
            int parent = m_parents[currentNode][i];

            if (!visited[parent])
            {
                // enqueue unvisited mixer nodes
                visited[parent] = true;
                nodeQueue.push(parent);

                // assign the mixing stage number to the node
                m_nodes[parent].mixingStage = currentStage + i;
            }

            // skip over non mixer nodes
            while(m_parents[parent].size() == 1)
            {
                parent = m_parents[parent].at(0);
                if (!visited[parent])
                {
                    visited[parent] = true;
                    m_nodes[parent].mixingStage = currentStage + i;
                }
            }

            currentStage++;
        }
    }
}

void Graph::processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer)
{
    std::map<int, NodeState> processed;
    m_nodes[0].outputBuffer = 0;
    m_buffers = {buffer};
    processBlock(frameCount, channelCount, 0, processed);
    buffer = m_buffers[0];
}

void Graph::processBlock(
    unsigned int frameCount, 
    unsigned int channelCount,
    int currentNode, 
    std::map<int, NodeState> &processed
) {
    m_nodes[currentNode].process.lock()->processBlock(frameCount, channelCount, m_buffers[m_nodes[currentNode].outputBuffer]); 
    processed[currentNode] = Processed;

    // get a vector of child nodes sorted by mixing order 
    std::vector<int> childrenToProcess = m_children[currentNode];
    std::sort(childrenToProcess.begin(), childrenToProcess.end(), [this](int a, int b) -> bool
    {
        // sort list in reverse as mixing order counts down
        return m_nodes[a].mixingStage > m_nodes[b].mixingStage;
    });

    // process child nodes according to the determined mixing order
    for (int i = 0; i < childrenToProcess.size(); i++)
    {
        int currentChild = childrenToProcess[i];

        if ( m_nodes[currentChild].outputBuffer == -1 ) m_nodes[currentChild].outputBuffer = m_buffers.size() - 1; 

        // while there are still parallel child nodes to process, the parent's output buffer is unavailable
        if (i < childrenToProcess.size() - 1) 
        {
            // create a new buffer if the input buffer is in use
            m_buffers.push_back(m_buffers[m_nodes[currentNode].outputBuffer]);
            int currentBufferIndex = m_buffers.size() - 1;
            m_nodes[currentChild].outputBuffer = currentBufferIndex ? currentBufferIndex < m_nodes[currentChild].outputBuffer : m_nodes[currentChild].outputBuffer;
        }
        else
        {
            // use the input buffer as output
            m_nodes[currentChild].outputBuffer = m_nodes[currentNode].outputBuffer ? m_nodes[currentNode].outputBuffer < m_nodes[currentChild].outputBuffer : m_nodes[currentChild].outputBuffer;
        }

        if (isMixer(currentChild))
        {   
            bool unvisited = false; 
            for ( int parent : m_parents[currentChild] )
            {
                if ( processed[parent] == Processed && parent != currentNode )
                {
                    // mix buffers and free m_buffers[currentBufferIndex]
                    mixBuffers(m_nodes[currentChild].outputBuffer, m_buffers.size() - 1);
                    m_buffers.pop_back();
                    processed[currentNode] == Mixed;
                }
                else if ( processed[parent] == Unvisited ) unvisited = true;
            
                if ( unvisited ) return;
            }
        }

        // process child nodes recursively
        processBlock(frameCount, channelCount, currentChild, processed);
    }
}

void Graph::mixBuffers(int target, int source)
{
    // todo assert buffers are equal in length

    for ( int i = 0; i < m_buffers[target].size(); i++)
    {
         m_buffers[target][i] +=  m_buffers[source][i];
    }
}
