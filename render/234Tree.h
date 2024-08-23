#pragma once
#include "Render.h"


struct Edge {
    Vector2 startPos;  // Start position of the edge (from parent honeycomb)
    Vector2 endPos;    // End position of the edge (to child node)
    float alpha;       // Transparency of the edge

    Edge(Vector2 start, Vector2 end, float a = 1.0f)
        : startPos(start), endPos(end), alpha(a) {}

    void draw() {
        if (endPos.x > 100.0f) DrawLineEx(startPos, endPos, 2.0f, Fade(BLACK, alpha));
    }
};


struct HoneycombNode {
    int value = 0;           // Value inside the honeycomb
    Vector2 position;    // Position of the honeycomb
    float alpha = 0.0f;         // Opacity of the honeycomb
    //Color outlineColor;  // Outline color of the honeycomb
    //Color fillColor;     // Fill color of the honeycomb
    int pos;
    bool visit = false;
    bool deleting = false;

    HoneycombNode(int val = 0, Vector2 pos ={0,0}, int index = -1)
        : value(val), position(pos), alpha(1.0f), pos(index) {}


    void draw();


    void update(float deltaTime, Vector2 targetPosition, float targetAlpha) {
        // Smoothly interpolate the position and alpha (for animation purposes)
        position.x += (targetPosition.x - position.x) * deltaTime;
        position.y += (targetPosition.y - position.y) * deltaTime;
        alpha += (targetAlpha - alpha) * deltaTime;
    }
};



struct Tree234Node {
    std::vector<HoneycombNode*> honeycombs;  // Pointers to Honeycomb nodes
    std::vector<Tree234Node*> children;      // Children of this node
    Tree234Node* parent;
    Vector2 position;
    std::vector<Edge> edges;

    Tree234Node(Vector2 pos = {0,0})
        : parent(nullptr), position(pos) {
    }

    void addCombs(HoneycombNode* comb) {
        honeycombs.push_back(comb);
        if (honeycombs.size() > 1) std::sort(honeycombs.begin(), honeycombs.end(), [](const HoneycombNode* a, const HoneycombNode* b) {
            return a->value < b->value;
            });
    }

    void updateEdges() {
        edges.clear();

        if (children.empty()) return;

        float honeycombWidth = 46.0f; // Assuming the honeycomb width is twice the offset used earlier (26.0f)

        for (size_t i = 0; i < children.size(); i++) {
            Vector2 start;

            // Determine the connection point based on the child index
            if (i == 0) {
                // First child connects to the midpoint of the left edge of the first honeycomb
                start = { honeycombs[0]->position.x - honeycombWidth / 2, honeycombs[0]->position.y };
            }
            else if (i == 1) {
                // Second child connects to the midpoint of the right edge of the first honeycomb
                start = { honeycombs[0]->position.x + honeycombWidth / 2, honeycombs[0]->position.y };
            }
            else if (i == 2) {
                // Third child connects to the midpoint of the right edge of the second honeycomb (if it exists)
                if (honeycombs.size() > 1) {
                    start = { honeycombs[1]->position.x + honeycombWidth / 2, honeycombs[1]->position.y };
                }
            }
            else if (i == 3) {
                // Fourth child connects to the midpoint of the right edge of the third honeycomb (if it exists)
                if (honeycombs.size() > 2) {
                    start = { honeycombs[2]->position.x + honeycombWidth / 2, honeycombs[2]->position.y };
                }
            }

            Vector2 end = children[i]->position;
            edges.emplace_back(start, end);
        }
    }

    void updateVisit() {
       if (honeycombs.size()>0) for (auto& comb : honeycombs) if (comb) comb->visit = true;
    }

    void unvisit() {
        if (honeycombs.size()>0) for (auto& comb : honeycombs) if (comb)  comb->visit = false;
    }

    void draw() {
        // Draw edges first
        for (auto& edge : edges) {
            edge.draw();
        }

        // Then draw the honeycombs
        for (auto& honeycomb : honeycombs) {
            honeycomb->draw();
        }
    }

};


class Tree234 {
public:
    Tree234();
    ~Tree234();

    //Inserting functions set
    void insert(int value);
    void insertRecursive(Tree234Node* &node, int value);
    void insertRecursiveAfterSplit(Tree234Node*& node, int value);
    void split(Tree234Node* node);

    //Deleting functions set
    void Tree234::deleteRecursive(Tree234Node* &node, int value);
    void Tree234::deleteValue(int value);
    int Tree234::findKey(Tree234Node* node, int value);
    Tree234Node* Tree234::getPredecessor(Tree234Node* node, int idx);
    void Tree234::removeKeyFromNode(Tree234Node* node, int idx);
    void Tree234::fixChild(Tree234Node* parentNode, int childIdx);
    int Tree234::getChildIndex(Tree234Node* node, int value);

    //Searching function set
    HoneycombNode* search(int value);
    HoneycombNode* searchRecursive(Tree234Node* node, int value);

    void clearTree();
    void clearTreeRecursive(Tree234Node* node);

    void updatePositions();
    void updateNodePosition(Tree234Node* node, int level, float minX, float maxX, float levelSpacing);

    void updateEdges(Tree234Node* node);
    void updateEdgesFromNode(Tree234Node* node);
    void Tree234::copyEdges();

    void draw();
    void copyTree();
    bool Tree234::isInteracting(int state);
    std::vector<std::vector<Tree234Node*>> stepRoots;

    int getStepsSize();

    void debug();
    void Tree234::print();
    void Tree234::printTree(Tree234Node* node, int level);

    void mixedComb(HoneycombNode* target, HoneycombNode* start, HoneycombNode* end, float mixCoeff);
    void Tree234::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step);

    void Tree234::finalDelete(int value, int& stateIndex, bool& pause);

    void Tree234::finalInsert(int value, int& stateIndex, bool& pause);

    void Tree234::finalCreate(int value, int& stateIndex, bool& pause);

    bool Tree234::finalSearch(int value, int& stateIndex, bool& pause);

    void Tree234::finalFile(std::vector<int>& input, int& stateIndex, bool& pause);

private:
    Tree234Node* root;
    std::vector<HoneycombNode*> nodeList;
    std::vector<std::vector<HoneycombNode*>> steps;

    std::vector<Edge> curEdges;
    std::vector<std::vector<Edge>> stepsEdges;


    std::vector<HoneycombNode*> curList;
    std::vector<std::vector<Tree234Node*>> stepEdges;
    int size = 0;
};
