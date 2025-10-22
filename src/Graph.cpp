/*
 *  Graph.cpp
 *
 *  Created on: 21 Oct 2025
 *      Author: mchlp
 */

#include <iostream>  // todo temp
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

        if (isMixer(currentNode))
            // increase mixer count 
            currentStage++;

        for (int parent : m_parents[currentNode])
        {
            // assign the mixing stage number to the node
            m_nodes[parent].mixingStage = (currentStage > m_nodes[parent].mixingStage) ? currentStage : m_nodes[parent].mixingStage;

            // skip over non mixer nodes
            while(m_parents[parent].size() == 1)
            {
                m_nodes[parent].mixingStage = (currentStage > m_nodes[parent].mixingStage) ? currentStage : m_nodes[parent].mixingStage;
                parent = m_parents[parent].at(0);
            }

            // enqueue unvisited mixer nodes
            if (!visited[parent])
            {
                visited[parent] = true;
                nodeQueue.push(parent);
            }
        }
    }
}

std::vector<float> Graph::generateBlock(std::vector<float> buffer)
{
    std::map<int, bool> processed;
    m_buffers = {buffer};
    generateBlock(0, 0, processed);
    return m_buffers[0];
}

int Graph::generateBlock(
    int currentBufferIndex,
    int currentNode, 
    std::map<int, bool> &processed
) {
    processed[currentNode] = true;

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

        // while there are still parallel child nodes to process, the parent's output buffer is unavailable
        if (i < childrenToProcess.size() - 1) 
        {
            m_buffers.push_back(m_buffers[currentBufferIndex]);
            m_nodes[currentChild].outputBuffer = ++currentBufferIndex;
        }
        else
        {
            m_nodes[currentChild].outputBuffer = m_nodes[currentNode].outputBuffer;
        }

        // nodes that mix buffers will free one of the buffers
        if (isMixer(currentChild))
        {   
            for ( int parent : m_parents[currentChild] )
                if ( !processed[parent] )
                {
                    // mixer has an unprocessed input so return
                    return currentBufferIndex;
                }
            
            // use the lowest buffer as an output
            for ( int parent : m_parents[currentChild] )
            {
                m_nodes[currentChild].outputBuffer = ( m_nodes[parent].outputBuffer < m_nodes[currentChild].outputBuffer) ? m_nodes[parent].outputBuffer : m_nodes[currentChild].outputBuffer;
            }
            // mix and free the top buffer
            mixBuffers(m_nodes[currentChild].outputBuffer, currentBufferIndex);
            m_buffers.pop_back();
            currentBufferIndex--;
        }
        else
        {
            m_buffers[m_nodes[currentChild].outputBuffer] = m_nodes[currentChild].process->processBlock(1024, 2, m_buffers[m_nodes[currentNode].outputBuffer]);  // TODO frame and channel count!
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
