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
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);
}

void Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char ch : word) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->isEndOfWord = true;
}
