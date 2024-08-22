#include "trie.h"

void TrieNode::draw() {
    // Determine the color with fading effect using the alpha value
    Color gradientStart = ColorAlpha(YELLOW, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);

    if (visiting) {
        outlineColor = ColorAlpha(RED, alpha);
    }

    if (ch == '0') return;

    DrawCircleV(position, radius, outlineColor);

    DrawCircleGradient(position.x, position.y, radius - 5.0f, WHITE, gradientEnd);
    // Draw ring outline

    // Calculate positions for the value and balance text
    int valueFontSize = 20; // Adjust font size as needed
    int balanceFontSize = 15; // Adjust font size as needed

    // Draw value text at the center of the node
    std::string valueStr = std::to_string(ch);
    if (ch == '1') valueStr = 'root';
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode({0,0}, ch);
            size++;
        }
        current = current->children[ch];
    }
    current->isEndOfWord = true;
}

void Trie::updatePositions() {
    if (size == 0) return; // If the tree is empty, do nothing

    // Initial settings: root node is centered at the top
    float initialX = constants::scene_width; // X position
    float levelSpacing = 50.0f;      // Vertical distance between levels
    updateNodePosition(root, 0, 0, initialX, levelSpacing);
}

void Trie::updateNodePosition(TrieNode* root, int level, float minX, float maxX, float levelSpacing) {
    if (root == NULL) return;

    // Set the position of the current node
    root->position.x = (minX + maxX) / 2;
    root->position.y = 120.0f + level * levelSpacing;

    int totalLeaves = 0;
    for (auto& pair : root->children) {
        totalLeaves += countLeaves(pair.second);
    }

    float currentX = minX;
    for (auto& pair : root->children) {
        int childLeaves = countLeaves(pair.second);
        float childMinX = currentX;
        float childMaxX = minX + (maxX - minX) * (static_cast<float>(childLeaves) / totalLeaves);

        // Recursively set positions for each child node
        updateNodePosition(pair.second, level + 1, childMinX, childMaxX, levelSpacing);
        currentX = childMaxX;
    }
}

int Trie::countLeaves(TrieNode* node) {
    if (node == nullptr) return 0;
    if (node->children.empty()) return 1; 

    int leafCount = 0;
    for (auto& pair : node->children) {
        leafCount += countLeaves(pair.second);
    }
    return leafCount;
}

bool Trie::removeHelper(TrieNode* node, const std::string& word, int depth) {
    // Base case: if we've reached the end of the word
    if (depth == word.size()) {
        // If this node is the end of the word, unmark it
        if (!node->isEndOfWord) {
            return false;  // The word was not found
        }

        node->isEndOfWord = false;

        // If the node has no children, it can be deleted
        return node->children.empty();
    }

    char ch = word[depth];
    TrieNode* childNode = node->children[ch];

    if (childNode == nullptr) {
        return false;  // The word was not found
    }

    // Recursively delete the child node
    bool shouldDeleteChild = removeHelper(childNode, word, depth + 1);

    // If the child should be deleted, remove it from the map
    if (shouldDeleteChild) {
        delete childNode;
        node->children.erase(ch);

        // If the current node has no other children and is not the end of another word, it can be deleted
        return !node->isEndOfWord && node->children.empty();
    }

    return false;
}


bool Trie::remove(const std::string& word) {
    if (root == nullptr || word.empty()) {
        return false;
    }

    return removeHelper(root, word, 0);
}
