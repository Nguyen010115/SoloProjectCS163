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

//struct EdgeHeap {
//    int startIndex = -1;  // Start index in heapArray
//    int endIndex = -1;    // End index in heapArray
//    float alpha=1.0f;     // Transparency of the edge
//
//    EdgeHeap(int start =-1, int end=-1, float a = 1.0f)
//        : startIndex(start), endIndex(end), alpha(a) {}
//
//    void draw(std::vector<MinHeapNode*>& heapArray) {
//        Vector2 startPos = heapArray[startIndex]->position;
//        Vector2 endPos = heapArray[endIndex]->position;
//        DrawLineEx(startPos, endPos, 2.0f, Fade(BLACK, alpha));
//        
//    }
//};

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

    MinHeap::~MinHeap() {
        for (MinHeapNode* node : nodeList) {
            if (node) delete node;
        }

        for (auto& e : edges) if (e) delete e;

        nodeList.clear();
        heapArray.clear();
        edges.clear();
    }

    void insert(int value);
    void bubbleUp(int index);

    void MinHeap::deleteElement(int value);
    void MinHeap::heapifyDown(int index);

    void updatePositions();
    void draw();

    void MinHeap::updateEdges();

    void MinHeap::printEdges();
    void MinHeap::printHeapArray();
    void MinHeap::debug();
    std::vector<EdgeHeap*> edges;
    void copyHeap();
    void MinHeap::copyHeap2();


    void MinHeap::mixedNode(MinHeapNode* target, MinHeapNode* start, MinHeapNode* end, float mixCoeff);
    void MinHeap::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step);
    int MinHeap::getStepsSize();
    

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

