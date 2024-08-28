#pragma once
#include "Render.h"

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    Vector2 position;
    int index = -1;
    char ch;
    float radius = 25.0f;
    float alpha = 1.0f;
    bool visiting = false;
    bool isEndOfWord = false;

    TrieNode(int ind = -1, Vector2 pos = {0 ,0}, char ch = '0') : index(ind), position(pos), ch(ch), isEndOfWord(false) {}

    void draw();
};

struct TrieEdge {
    int startNode;
    int endNode;
    float alpha;       

   TrieEdge(int s = -1, int e = -1, float a = 1.0f)
        : startNode(s), endNode(e), alpha(a) {}

   void draw(std::vector<TrieNode*>& list);
};

class Trie {
public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool search(const std::string& word);
    bool remove(const std::string& word);
    void updatePositions();
    void updateNodePosition(TrieNode* root, int level, float minX, float maxX, float levelSpacing);
    int countLeaves(TrieNode* node);

    void updateEdges(TrieNode* node);
    void updateEdgesHelper(TrieNode* node, std::vector<TrieEdge*>& edges);
    void copyTree();

    void mixNodes(TrieNode* target, TrieNode* start, TrieNode* end, float mixCoeff);
    void mixEdge(TrieEdge& target, const TrieEdge* start, const TrieEdge* end, float mixCoeff);
    void updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step);
    int getStepsSize();

    bool isInteracting(int state);
    void finalInsert(const std::string& word, int& stateIndex, bool& pause);
    void finalDelete(const std::string& word, int& stateIndex, bool& pause);
    void finalCreate(int count, int& stateIndex, bool& pause);
    void finalFile(const std::vector<std::string>& input, int& stateIndex, bool& pause);
    bool finalSearch(const std::string& word, int& stateIndex, bool& pause);
    void clearTree();
    void draw();  

private:
    TrieNode* root;
    std::vector<TrieNode*> nodeList;
    std::vector<TrieNode*> curList;
    std::vector<TrieEdge*> curEdge;
    std::vector<std::vector<TrieNode*>> steps;
    std::vector<std::vector<TrieEdge*>> stepsEdge;

    int size = 0;
    bool removeHelper(TrieNode* node, const std::string& word, int depth);
    void deleteNode(TrieNode* node);
};

Trie::Trie() {
    root = new TrieNode(0,{683,120}, '1');
    size++;
    nodeList.push_back(root);
}

Trie::~Trie() {
    clearTree();
}

void Trie::deleteNode(TrieNode* node) {
    if (node == nullptr) return;
    for (auto pair : node->children) {
        if (pair.second) deleteNode(pair.second);
    }
    delete node;
}

