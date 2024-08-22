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

    void visualize();  // Optional: Method to visualize the Trie

private:
    TrieNode* root;
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

