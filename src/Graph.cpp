/*
 *  Graph.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: mchlp
 */

#include <queue>
#include <algorithm>

#include "Graph.h"

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

        for (int i = 0; i < m_parents[currentNode].size(); i++) // (int parent : m_parents[currentNode])
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

std::vector<float> Graph::generateBlock(std::vector<float> buffer)
{
    std::map<int, NodeState> processed;
    m_nodes[0].outputBuffer = 0;
    m_buffers = {buffer};
    generateBlock(0, 0, processed);
    return m_buffers[0];
}

int Graph::generateBlock(
    int currentBufferIndex,  // TODO see if currentBufferIndex can be m_buffers.size() - 1
    int currentNode, 
    std::map<int, NodeState> &processed
) {
    m_buffers[m_nodes[currentNode].outputBuffer] = m_nodes[currentNode].process->processBlock(1024, 2, m_buffers[m_nodes[currentNode].outputBuffer]);  // TODO frame and channel count!
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

        if ( m_nodes[currentChild].outputBuffer == -1 ) m_nodes[currentChild].outputBuffer = currentBufferIndex; 

        // while there are still parallel child nodes to process, the parent's output buffer is unavailable
        if (i < childrenToProcess.size() - 1) 
        {
            // create a new buffer if the input buffer is in use
            m_buffers.push_back(m_buffers[m_nodes[currentNode].outputBuffer]);
            currentBufferIndex++;
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
                    mixBuffers(m_nodes[currentChild].outputBuffer, currentBufferIndex);
                    m_buffers.pop_back();
                    currentBufferIndex--;
                    processed[currentNode] == Mixed;
                }
                else if ( processed[parent] == Unvisited ) unvisited = true;
            
                if ( unvisited ) return currentBufferIndex;
            }
        }

        // process child nodes recursively
        currentBufferIndex = generateBlock(currentBufferIndex, currentChild, processed);
    }
    return currentBufferIndex;
}

void Graph::mixBuffers(int target, int source)
{
    // todo assert buffers are equal in length

    for ( int i = 0; i < m_buffers[target].size(); i++)
    {
         m_buffers[target][i] +=  m_buffers[source][i];
    }
}
