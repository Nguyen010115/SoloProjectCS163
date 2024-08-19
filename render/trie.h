#pragma once
#include "Render.h"

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool remove(const std::string& word);

    void visualize();  // Optional: Method to visualize the Trie

private:
    TrieNode* root;

    bool removeHelper(TrieNode* node, const std::string& word, int depth);
    void deleteNode(TrieNode* node);
};

Trie::Trie() {
    root = new TrieNode();
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

