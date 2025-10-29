/*
 *  Graph.h
 *
 *  Created on: 21 Oct 2025
 *      Author: mchlp
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <map>
#include <stack>

#include "AudioObject.h"


enum NodeState
{
	Unvisited,
    Processed,
    Mixed
};

struct Node
{
    Node() :
        process(nullptr),
        mixingStage(0),
        outputBuffer(0)  // starts unset
    {}

    Node(AudioObject* process) :
        process(process),
        mixingStage(0),
        outputBuffer(-1)  // starts unset
    {}

    AudioObject* process;
    int mixingStage;
    int outputBuffer;
};

class Graph : public AudioObject
{
public:
    Graph() :
        m_nextIndex(0)
    {}
    ~Graph() = default;

    bool isMixer(int node) { return m_parents[node].size() > 1; }

    int addNode(AudioObject* pNode) 
    { 
        m_nodes.insert({m_nextIndex, Node(pNode)}); 
        return m_nextIndex++;
    }

    void addChild(int parent, int child)
    {
        m_children[parent].push_back(child);
        m_parents[child].push_back(parent);
        updateMixingOrder(child);
    }

    void processBlock(unsigned int frameCount, unsigned int channelCount, std::vector<float> &buffer) override;

private:
    int m_nextIndex;

    std::map<int, Node> m_nodes;
    std::map<int, std::vector<int>> m_children;
    std::map<int, std::vector<int>> m_parents;
    std::vector<std::vector<float>> m_buffers;

    void updateMixingOrder(int outputNode);
    void processBlock(
        unsigned int frameCount,
        unsigned int channelCount,
        int currentNode, 
        std::map<int, NodeState> &processed
    );
    void mixBuffers(int target, int source);
};

#endif /* GRAPH_H_ */
