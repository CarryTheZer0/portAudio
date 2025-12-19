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
#include <memory>

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
        mixingStage(0),
        outputBuffer(0)
    {}

    Node(std::weak_ptr<AudioObject> process) :
        process(process),
        mixingStage(0),
        outputBuffer(-1)  // starts unset
    {}

    std::weak_ptr<AudioObject> process;
    int mixingStage;
    int outputBuffer;
};

class Graph : public AudioObject
{
public:
    Graph() = delete;

    Graph(std::weak_ptr<AudioObject> input) :
        m_nextIndex(0)
    {
        addNode(input);
    }

    int addNode(std::weak_ptr<AudioObject> audioObject) 
    { 
        m_nodes.insert({m_nextIndex, Node(audioObject)}); 
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

    bool isMixer(int node) { return m_parents[node].size() > 1; }
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
