#pragma once
#include "Render.h"

struct MinHeapNode {
    int value = 0;
    int ind = -1;
    Vector2 position;
    float radius = 25.0f;
    float alpha = 1.0f;
    bool visiting = false;

    MinHeapNode(int val=0, Vector2 pos = { 0, 0 })
        : value(val), position(pos) {}

    void draw();
};


struct EdgeHeap {
    Vector2 startPos;  // Start position of the edge
    Vector2 endPos;    // End position of the edge
    float alpha;       // Transparency of the edge

    EdgeHeap(Vector2 start = {0,0}, Vector2 end= { 0,0 }, float a = 1.0f)
        : startPos(start), endPos(end), alpha(a) {}

    void draw() const {
        DrawLineEx(startPos, endPos, 2.0f, Fade(BLACK, alpha));
    }
};


class MinHeap {
public:
    MinHeap() {}

    ~MinHeap();

    void insert(int value);
    void bubbleUp(int index);

    void MinHeap::deleteElement(int value);
    void MinHeap::heapifyDown(int index);

    void updatePositions();
    void draw();

    void MinHeap::updateEdges();

    void MinHeap::printHeapArray();
    void MinHeap::debug();
    std::vector<EdgeHeap*> edges;
    void copyHeap();
    void MinHeap::copyHeap2();
    bool MinHeap::search(int input);

    void MinHeap::mixedNode(MinHeapNode* target, MinHeapNode* start, MinHeapNode* end, float mixCoeff);
    void MinHeap::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step);
    int MinHeap::getStepsSize();
    
    bool MinHeap::isInteracting(int state);
    void MinHeap::finalFile(std::vector<int>& input, int& stateIndex, bool& pause);
    void MinHeap::finalInsert(int input, int& stateIndex, bool& pause);
    void MinHeap::finalDelete(int value, int& stateIndex, bool& pause);
    bool MinHeap::finalSearch(int value, int& stateIndex, bool& pause);
    void MinHeap::finalCreate(int value, int& stateIndex, bool& pause);
    void MinHeap::getTop(int& stateIndex, bool& pause);
    int MinHeap::getSize();
    void MinHeap::clearTree();

private:
    std::vector<MinHeapNode*> nodeList;    
    std::vector<MinHeapNode*> heapArray; 
    std::vector<std::vector<MinHeapNode*>> steps;
    std::vector<std::vector<EdgeHeap*>> stepsEdge;
    std::vector<MinHeapNode*> curList;
    std::vector<EdgeHeap*> curEdges;
    void addEdge(int parentIndex, int childIndex);
    Vector2 calculateNodePosition(int index);
};

MinHeap::~MinHeap() {
    for (MinHeapNode* node : nodeList) {
        if (node) delete node;
    }

    for (auto& e : edges) if (e) delete e;

    nodeList.clear();
    heapArray.clear();
    edges.clear();
}