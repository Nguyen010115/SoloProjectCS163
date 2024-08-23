#pragma once
#include "Render.h"

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    Vector2 position;
    char ch;
    float radius = 0.25f;
    float alpha = 1.0f;
    bool visiting = false;
    bool isEndOfWord;

    TrieNode(Vector2 pos = {0 ,0}, char ch = '0') : position(pos), ch(ch), isEndOfWord(false) {}

    void draw();
};

struct TrieEdge {
    Vector2 startPos;  
    Vector2 endPos;   
    float alpha;       

   TrieEdge(Vector2 start = { 0,0 }, Vector2 end = { 0,0 }, float a = 1.0f)
        : startPos(start), endPos(end), alpha(a) {}

    void draw() const {
        DrawLineEx(startPos, endPos, 2.0f, Fade(BLACK, alpha));
    }
};

class Trie {
public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool remove(const std::string& word);
    void updatePositions();
    void updateNodePosition(TrieNode* root, int level, float minX, float maxX, float levelSpacing);
    int countLeaves(TrieNode* node);
    void copyTree();
    void copyHeap2();
    bool search(int input);

    void mixedNode(TrieNode* target, TrieNode* start, TrieNode* end, float mixCoeff);
    void updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step);
    int getStepsSize();

    bool isInteracting(int state);
    void finalFile(std::vector<int>& input, int& stateIndex, bool& pause);
    void finalInsert(int input, int& stateIndex, bool& pause);
    void finalDelete(int value, int& stateIndex, bool& pause);
    bool finalSearch(int value, int& stateIndex, bool& pause);
    void finalCreate(int value, int& stateIndex, bool& pause);
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
    root = new TrieNode({0,0}, '1');
}

Trie::~Trie() {
    deleteNode(root);
}

void Trie::deleteNode(TrieNode* node) {
    if (node == nullptr) return;
    for (auto& pair : node->children) {
        deleteNode(pair.second);
    }
    delete node;
}

