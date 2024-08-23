#include "234Tree.h"

void HoneycombNode::draw() {
    if (value == 0 || pos == -1) return;
    //Color gradientStart = ColorAlpha(YELLOW, alpha);
   //Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color fillColor = ColorAlpha(YELLOW, alpha);

    if (visit) fillColor = ColorAlpha(DARKGREEN, alpha);
    if (deleting) fillColor = ColorAlpha(RED, alpha);

    float size = 26.0f; // Size of the honeycomb
    float halfSize = size / 2.0f;
    float quarterSize = size / 4.0f;
    float sqrt3_over_2 = sqrtf(3.0f) / 2.0f;  // Factor to calculate vertical height

    Vector2 vertices[6] = {
        {position.x, position.y - size},              // Top vertex
        {position.x + sqrt3_over_2 * size, position.y - halfSize},  // Top-right vertex
        {position.x + sqrt3_over_2 * size, position.y + halfSize},  // Bottom-right vertex
        {position.x, position.y + size},              // Bottom vertex
        {position.x - sqrt3_over_2 * size, position.y + halfSize},  // Bottom-left vertex
        {position.x - sqrt3_over_2 * size, position.y - halfSize}   // Top-left vertex
    };

    // Draw the filled hexagon
    DrawTriangle(vertices[5], vertices[1], vertices[0], fillColor);
    DrawTriangle(vertices[5], vertices[2], vertices[1], fillColor);
    DrawTriangle(vertices[5], vertices[4], vertices[2], fillColor);
    DrawTriangle(vertices[4], vertices[3], vertices[2], fillColor);

    // Draw the outline of the hexagon
    for (int i = 0; i < 6; i++) {
        DrawLineEx(vertices[i], vertices[(i + 1) % 6], 4.0f, Fade(outlineColor, alpha));
    }

    // Draw the value inside the honeycomb
    std::string valueStr = std::to_string(value);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), valueStr.c_str(), 20, 1);
    Vector2 textPos = { position.x - textSize.x / 2.0f, position.y - textSize.y / 2.0f };
    DrawText(valueStr.c_str(), textPos.x, textPos.y, 20, BLACK);
}

// Tree234Node constructor
//Tree234Node::Tree234Node(int value, Vector2 pos) {
//    values.push_back(value);
//    parent = nullptr;
//    position = { 0, 0 };
//}



// Add value to the node
//void Tree234Node::addValue(int value) {
//    values.push_back(value);
//    std::sort(values.begin(), values.end());
//}

// Add a child to the node
//void Tree234Node::addChild(Tree234Node* child) {
//    children.push_back(child);
//    std::sort(children.begin(), children.end(), [](Tree234Node* a, Tree234Node* b) {
//        return a->values[0] < b->values[0];
//        });
//}


Tree234::Tree234() : size(0), root(nullptr) {}

// Tree234 destructor
Tree234::~Tree234() {
    clearTree();
}

void Tree234::insert(int value) {
    if (root == nullptr) {
        copyTree();
        root = new Tree234Node({0, 0}); 
        HoneycombNode* newComb = new HoneycombNode(value, {0,0}, size);
        root->addCombs(newComb);
        nodeList.push_back(newComb);
        size++;
        copyTree();
    }
    else {
        insertRecursive(root, value);
    }
    copyTree();  
}

void Tree234::insertRecursive(Tree234Node* &node, int value) {
    if (node == NULL) return;
    copyTree();
    node->updateVisit();
    copyTree();
    bool check = false;
    if (node->honeycombs.size() == 3) {
        node->unvisit();
        Tree234Node* parentRoot = node->parent;
        split(node);
        copyTree();
        if (parentRoot == NULL) return insertRecursive(root, value);
        else return insertRecursiveAfterSplit(parentRoot, value);
    }
  
    // If it's a leaf node (no children), insert the value directly
    if (node->children.empty()) {
        HoneycombNode* newComb = new HoneycombNode(value, { 0,0 }, size);
        node->addCombs(newComb);
        nodeList.push_back(newComb);
        size++;
        copyTree();
        node->unvisit();
        return;
    }

    // Determine which child to recurse into
    if (value < node->honeycombs[0]->value) {
        node->unvisit();
        insertRecursive(node->children[0], value);
    }
    else if (node->honeycombs.size() == 1 || (node->honeycombs.size() > 1 && value < node->honeycombs[1]->value)) {
        node->unvisit();
        insertRecursive(node->children[1], value);
    }
    else if (node->honeycombs.size() == 2 || (node->honeycombs.size() > 2 && value < node->honeycombs[2]->value)) {
        node->unvisit();
        insertRecursive(node->children[2], value);
    }
    else {
        node->unvisit();
        insertRecursive(node->children[3], value);
    }
    return;
}

void Tree234::insertRecursiveAfterSplit(Tree234Node*& node, int value) {
    if (node == NULL) return;
    copyTree();
    node->updateVisit();
    copyTree();

    // If it's a leaf node (no children), insert the value directly
    if (node->children.empty()) {
        HoneycombNode* newComb = new HoneycombNode(value, { 0,0 }, size);
        node->addCombs(newComb);
        nodeList.push_back(newComb);
        size++;
        copyTree();
        node->unvisit();
        return;
    }

    // Determine which child to recurse into
    if (value < node->honeycombs[0]->value) {
        node->unvisit();
        insertRecursive(node->children[0], value);
    }
    else if (node->honeycombs.size() == 1 || (node->honeycombs.size() > 1 && value < node->honeycombs[1]->value)) {
        node->unvisit();
        insertRecursive(node->children[1], value);
    }
    else if (node->honeycombs.size() == 2 || (node->honeycombs.size() > 2 && value < node->honeycombs[2]->value)) {
        node->unvisit();
        insertRecursive(node->children[2], value);
    }
    else {
        node->unvisit();
        insertRecursive(node->children[3], value);
    }
    return;
}




void Tree234::split(Tree234Node* node) {

    int midValue = node->honeycombs[1]->value;


    // Create new parent or move middle value to parent
    Tree234Node* parentNode;
    // Create new left and right child nodes from the split node
    Tree234Node* leftChild = new Tree234Node({ 0, 0 });
    Tree234Node* rightChild = new Tree234Node({ 0, 0 });


    if (node->parent == nullptr) {
        // If the node is the root, create a new root and move the middle value up
        parentNode = new Tree234Node({ 0, 0 });
        parentNode->addCombs(node->honeycombs[1]);
        root = parentNode; // Set the new root
        // Transfer the left and right honeycombs to the new child nodes
        leftChild->addCombs(node->honeycombs[0]);
        rightChild->addCombs(node->honeycombs[2]);

        // If the node has children, distribute them between the new child nodes
        if (!node->children.empty()) {
            leftChild->children.push_back(node->children[0]);
            leftChild->children.push_back(node->children[1]);
            rightChild->children.push_back(node->children[2]);
            rightChild->children.push_back(node->children[3]);

            for (auto child : leftChild->children) {
                child->parent = leftChild;
            }
            for (auto child : rightChild->children) {
                child->parent = rightChild;
            }
        }

        // Attach the new children to the parent node
        parentNode->children.push_back(leftChild);
        parentNode->children.push_back(rightChild);

        leftChild->parent = parentNode;
        rightChild->parent = parentNode;
    }
    else {
        // If the node has a parent, promote the middle value to the parent
        parentNode = node->parent;
        parentNode->addCombs(node->honeycombs[1]);

        leftChild->addCombs(node->honeycombs[0]);
        rightChild->addCombs(node->honeycombs[2]);

        leftChild->parent = parentNode;
        rightChild->parent = parentNode;

        if (!node->children.empty()) {
            if (node->children[0]) leftChild->children.push_back(node->children[0]);
            if (node->children[1]) leftChild->children.push_back(node->children[1]);
            if (node->children[2]) rightChild->children.push_back(node->children[2]);
            if (node->children[3]) rightChild->children.push_back(node->children[3]);

            for (auto child : leftChild->children) {
                child->parent = leftChild;
            }
            for (auto child : rightChild->children) {
                child->parent = rightChild;
            }
        }

        // Adjust the parent's child pointers based on the midValue
        auto it = std::find(parentNode->children.begin(), parentNode->children.end(), node);
        if (it != parentNode->children.end()) {
            *it = leftChild;
            parentNode->children.insert(it + 1, rightChild);
        }
    }

    Tree234Node* curNode = node;
    node = parentNode;

    delete curNode;
}



void Tree234::deleteValue(int value) {
    if (root == nullptr) {
        return;  // Tree is empty
    }

    // Start the deletion process
    deleteRecursive(root, value);
    // If the root is left with no keys, make its only child the new root
    if (root->honeycombs.empty() && !root->children.empty()) {
        Tree234Node* oldRoot = root;
        root = root->children[0];
        root->parent = nullptr;
        delete oldRoot;
    }
    copyTree();
}

void Tree234::deleteRecursive(Tree234Node* &node, int value) {
    int idx = findKey(node, value);
    copyTree();
    node->updateVisit();
    copyTree();
    // Case 1: The value is found in this node
    if (idx != -1) {
        if (node->children.empty()) {
            node->honeycombs[idx]->deleting = true;
            copyTree();
            removeKeyFromNode(node, idx);
        }
        else {
            // Case 1b: Node is an internal node
            // Replace with predecessor (or successor)
            Tree234Node* predNode = getPredecessor(node, idx);
            HoneycombNode* predKey = predNode->honeycombs.back();
            predKey->deleting = true;
            copyTree();
            int replace = predKey->value;
            HoneycombNode* target = node->honeycombs[idx];
            target->deleting = true;
            deleteRecursive(node, predKey->value);
            target->value = replace;
            copyTree();
            target->deleting = false;
            copyTree();

        }
        node->unvisit();
        copyTree();
    }
    else { //Not found in node
        node->unvisit();
        // Case 2: The value is not in this node, proceed with child node
        int childIdx = getChildIndex(node, value);
        Tree234Node* childNode = node->children[childIdx];

        if (childNode->honeycombs.size() == 1) {
            // If child node has only one key, we need to fix it before deletion
            childNode->updateVisit();
            copyTree();
            fixChild(node, childIdx);
            deleteRecursive(node, value);
        }
        else
        {
            deleteRecursive(node->children[childIdx], value);
        } 
    }
}

int Tree234::findKey(Tree234Node* node, int value) {
    for (int i = 0; i < node->honeycombs.size(); ++i) {
        if (node->honeycombs[i]->value == value) {
            return i;
        }
    }
    return -1;
}

Tree234Node* Tree234::getPredecessor(Tree234Node* node, int idx) {
    Tree234Node* current = node->children[idx];
    while (!current->children.empty()) {
        current = current->children.back();
    }
    return current;
}

void Tree234::removeKeyFromNode(Tree234Node* node, int idx) {
    HoneycombNode* temp = *(node->honeycombs.begin() + idx);
    node->honeycombs.erase(node->honeycombs.begin() + idx);
    temp->value = 0;
}

void Tree234::fixChild(Tree234Node* parentNode, int childIdx) {
    Tree234Node* child = parentNode->children[childIdx];

    // Case 1: Borrow from left sibling
    if (childIdx > 0 && parentNode->children[childIdx - 1]->honeycombs.size() > 1) {
        Tree234Node* leftSibling = parentNode->children[childIdx - 1];
        child->honeycombs.insert(child->honeycombs.begin(), parentNode->honeycombs[childIdx - 1]);
        parentNode->honeycombs[childIdx - 1] = leftSibling->honeycombs.back();
        leftSibling->honeycombs.pop_back();

        if (!leftSibling->children.empty()) {
            child->children.insert(child->children.begin(), leftSibling->children.back());
            leftSibling->children.pop_back();
        }
    }
    // Case 2: Borrow from right sibling
    else if (childIdx < parentNode->children.size() - 1 && parentNode->children[childIdx + 1]->honeycombs.size() > 1) {
        Tree234Node* rightSibling = parentNode->children[childIdx + 1];
        child->honeycombs.push_back(parentNode->honeycombs[childIdx]);
        parentNode->honeycombs[childIdx] = rightSibling->honeycombs.front();
        rightSibling->honeycombs.erase(rightSibling->honeycombs.begin());

        if (!rightSibling->children.empty()) {
            child->children.push_back(rightSibling->children.front());
            rightSibling->children.erase(rightSibling->children.begin());
        }
    }
    // Case 3: Merge with sibling
    else {
        if (childIdx > 0) {
            // Merge with left sibling
            Tree234Node* leftSibling = parentNode->children[childIdx - 1];
            leftSibling->honeycombs.push_back(parentNode->honeycombs[childIdx - 1]);
            leftSibling->honeycombs.insert(leftSibling->honeycombs.end(), child->honeycombs.begin(), child->honeycombs.end());

            if (!child->children.empty()) {
                leftSibling->children.insert(leftSibling->children.end(), child->children.begin(), child->children.end());
            }

            parentNode->honeycombs.erase(parentNode->honeycombs.begin() + (childIdx - 1));
            parentNode->children.erase(parentNode->children.begin() + childIdx);

            delete child;

            // Handle the case where the parent is now empty and was the root
            if (parentNode == root && parentNode->honeycombs.empty()) {
                root = leftSibling;
                root->parent = nullptr;
                delete parentNode;
            }
        }
        else {
            // Merge with right sibling
            Tree234Node* rightSibling = parentNode->children[childIdx + 1];

            child->honeycombs.push_back(parentNode->honeycombs[childIdx]);
            child->honeycombs.insert(child->honeycombs.end(), rightSibling->honeycombs.begin(), rightSibling->honeycombs.end());

            if (!rightSibling->children.empty()) {
                child->children.insert(child->children.end(), rightSibling->children.begin(), rightSibling->children.end());
            }

            parentNode->honeycombs.erase(parentNode->honeycombs.begin() + childIdx);
            parentNode->children.erase(parentNode->children.begin() + (childIdx + 1));

            delete rightSibling;

            // Handle the case where the parent is now empty and was the root
            if (parentNode == root && parentNode->honeycombs.empty()) {
                root = child;
                root->parent = nullptr;
                delete parentNode;
            }
        }
    }
}


int Tree234::getChildIndex(Tree234Node* node, int value) {
    for (int i = 0; i < node->honeycombs.size(); ++i) {
        if (value < node->honeycombs[i]->value) {
            return i;
        }
    }
    return node->honeycombs.size();
}


HoneycombNode* Tree234::search(int value) {
    if (!root) return NULL;
    HoneycombNode* res = searchRecursive(root, value);
    copyTree();
}

HoneycombNode* Tree234::searchRecursive(Tree234Node* node, int value) {
    if (!node) return nullptr;
    int idx = findKey(node, value);
    node->updateVisit();
    copyTree();
    if (idx != -1) {
        node->honeycombs[idx]->deleting = true;
        copyTree();
        node->honeycombs[idx]->deleting = false;
        node->unvisit();
        return node->honeycombs[idx];
    }
    else {
        node->unvisit();
        int child = getChildIndex(node, value);
        return searchRecursive(node->children[child], value);
    }
    
}

void Tree234::finalDelete(int value, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdges.clear();
    copyTree();
    deleteValue(value);
    pause = false;

}

void Tree234::finalInsert(int value, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdges.clear();
    copyTree();
    insert(value);
    pause = false;

}

void Tree234::finalCreate(int value, int& stateIndex, bool& pause) {
    pause = true;
    clearTree();
    root = nullptr;
    stateIndex = 0;
    for (int i = 0; i < value; i++) {
        int s = rand() % 1000;
        if (s == 0) s++;
        insert(s);
    }
    pause = false;
}


bool Tree234::finalSearch(int value, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdges.clear();
    copyTree();
    HoneycombNode* searchNode = searchRecursive(root, value);
    if (!searchNode) return false;
    searchNode->deleting = true;
    copyTree();
    searchNode->deleting = false;
    copyTree();
    searchNode->deleting = true;
    copyTree();
    searchNode->deleting = false;
    copyTree();
    pause = false;
    return true;
}

void Tree234::finalFile(std::vector<int>& input, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;
    clearTree();
    for (auto i : input) insert(i);
    pause = false;
}



// Clear the tree
void Tree234::clearTree() {
    clearTreeRecursive(root);
    root = nullptr;
    for (auto s : steps) {
        for (auto h : s) {
            if (h) delete h;
        }
    }
    nodeList.clear();
    curList.clear();
    curEdges.clear();
    steps.clear();
    stepEdges.clear();
    size = 0;
}

// Recursive clearing of the tree
void Tree234::clearTreeRecursive(Tree234Node* node) {
    if (node == nullptr) return;

    for (auto honey : node->honeycombs) {
        delete honey;
    }


    for (auto child : node->children) {
        clearTreeRecursive(child);
    }
    delete node;
}

// Update positions of nodes for visualization
void Tree234::updatePositions() {
    if (size == 0) return; // If the tree is empty, do nothing

    // Initial settings: root node is centered at the top
    float initialX = constants::scene_width; // X position
    float levelSpacing = 50.0f;        // Vertical distance between levels
    updateNodePosition(root, 0, 0, initialX, levelSpacing);
}


// Update node position recursively
void Tree234::updateNodePosition(Tree234Node* node, int level, float minX, float maxX, float levelSpacing) {
    if (node == nullptr) return;

    // Set the position of the current node
    node->position.x = (minX + maxX) / 2;
    node->position.y = 120.0f + level * levelSpacing;

    // Update the positions of the honeycombs within this node
    if (node->honeycombs.size() == 1) {
        node->honeycombs[0]->position = node->position;
    }
    else if (node->honeycombs.size() == 2) {
        float offset = 25.0f; // Adjust the offset as needed
        node->honeycombs[0]->position = { node->position.x - offset, node->position.y };
        node->honeycombs[1]->position = { node->position.x + offset, node->position.y };

    }
    else if (node->honeycombs.size() == 3) {
        float offset = 50.0f; // Adjust the offset as needed
        node->honeycombs[0]->position = { node->position.x - offset, node->position.y };
        node->honeycombs[1]->position = node->position;
        node->honeycombs[2]->position = { node->position.x + offset, node->position.y };
    }

    // Recursively set positions for children
    if (!node->children.empty()) {
        float segmentWidth = (maxX - minX) / node->children.size();
        float childMinX = minX;
        for (size_t i = 0; i < node->children.size(); i++) {
            float childMaxX = childMinX + segmentWidth;
            updateNodePosition(node->children[i], level + 1, childMinX, childMaxX, levelSpacing);
            childMinX = childMaxX;
        }
    }
}


void Tree234::updateEdges(Tree234Node* node) {
    if (node == nullptr) return;

    // Update edges for the current node
    node->updateEdges();

    // Recursively update edges for all children
    for (Tree234Node* child : node->children) {
        updateEdges(child);
    }
}

// Helper function to recursively collect edges
void collectEdges(Tree234Node* node, std::vector<Edge>& edgesCopy) {
    if (node == nullptr) return;

    // Copy edges from the current node
    for (const Edge& edge : node->edges) {
        edgesCopy.push_back(edge);
    }

    // Recursively collect edges from children
    for (Tree234Node* child : node->children) {
        collectEdges(child, edgesCopy);
    }
};

void Tree234::copyEdges() {
    std::vector<Edge> edgesCopy;
    

    // Start the recursive collection from the root
    collectEdges(root, edgesCopy);

    // Store the collected edges in stepsEdges
    stepsEdges.push_back(edgesCopy);
}



// Update edges from a node
void Tree234::updateEdgesFromNode(Tree234Node* node) {
    // Implement recursive edge updating logic
}

bool Tree234::isInteracting(int state) {
    if (steps.size() == 0) return false;
    return (state < steps.size() - 1);
}

// Draw the tree
void Tree234::draw() {

    for (Edge e : curEdges) {
        e.draw();
    }

    // Iterate over all nodes in the nodeList and draw them
    for (HoneycombNode* node : curList) {
        node->draw();
    }
}

void Tree234::copyTree() {
    updatePositions();
    updateEdges(root);

    copyEdges();

    std::vector<HoneycombNode*> nodeListCopy;
    for (HoneycombNode* node : nodeList) {
        HoneycombNode* copyNode = new HoneycombNode(node->value, node->position, node->pos);
        copyNode->alpha = node->alpha;
        copyNode->visit = node->visit;
        copyNode->deleting = node->deleting;

        //copyNode->outlineColor = node->outlineColor;
        //copyNode->fillColor = node->fillColor;
        nodeListCopy.push_back(copyNode);
    }

    steps.push_back(nodeListCopy);
}



void Tree234::mixedComb(HoneycombNode* target, HoneycombNode* start, HoneycombNode* end, float mixCoeff) {
    if (!start && !end) return;

    if (start && end) {
        // Interpolate position
        if (fabs(start->position.x - end->position.x) > 1.0f) target->position.x = start->position.x * (1.0f - mixCoeff) + end->position.x * mixCoeff;
        else  target->position.x = end->position.x;

        if (fabs(start->position.y - end->position.y) > 1.0f) target->position.y = start->position.y * (1.0f - mixCoeff) + end->position.y * mixCoeff;
        else  target->position.y = end->position.y;

        // Interpolate alpha
        target->alpha = start->alpha * (1.0f - mixCoeff) + end->alpha * mixCoeff;
        //target->alphavisiting = start->alphavisiting * (1.0f - mixCoeff) + end->alphavisiting * mixCoeff;

        // Interpolate radius (if needed)
        target->visit = end->visit;
        //target->searchVisit = end->searchVisit;
        target->deleting = end->deleting;

        target->value = start->value;
    }
    else if (start) {
        // Fading out effect for deleted nodes
        target->position = start->position;
        if (fabs(mixCoeff - 1.0f) < 0.05f) target->alpha = 0;
        else target->alpha = start->alpha * (1.0f - mixCoeff);

        target->value = start->value;
        //target->balance = start->balance;
    }
    else if (end) {
        // Fading in effect for new nodes
        target->position = end->position;
        target->alpha = end->alpha * mixCoeff;
        //target->radius = end->radius;
        target->value = end->value;
        //target->balance = end->balance;
    }
}

void Tree234::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step) {

    if (steps.size() == 0 || stateIndex < 0 || stateIndex >= steps.size() - 1) return; // Ensure stateIndex is within valid range

    elapsedTime += deltaTime;
    float G = elapsedTime / step; 
    if (G > step) G = step;

    const std::vector<HoneycombNode*>& startCombList = steps[stateIndex];
    const std::vector<HoneycombNode*>& endCombList = steps[stateIndex + 1];

    const std::vector<Edge>& startEdgeList = stepsEdges[stateIndex];
    const std::vector<Edge>& endEdgeList = stepsEdges[stateIndex + 1];

    curList.clear();  // Clear current list of mixed combs
    curEdges.clear(); // Clear current list of mixed edges

    size_t maxSize = std::max(startCombList.size(), endCombList.size());
    curList.resize(maxSize);


    for (size_t i = 0; i < maxSize; ++i) {
        HoneycombNode* startComb = (i < startCombList.size()) ? startCombList[i] : nullptr;
        HoneycombNode* endComb = (i < endCombList.size()) ? endCombList[i] : nullptr;

        HoneycombNode* newmixedComb = new HoneycombNode(0, { 0, 0 }, i); // Create a new mixed comb node
        mixedComb(newmixedComb, startComb, endComb, G);

        curList[i] = newmixedComb;  // Add the mixed comb to the current list
    }

    // Mix edges
    size_t maxEdgeSize = std::max(startEdgeList.size(), endEdgeList.size());
    for (size_t i = 0; i < maxEdgeSize; ++i) {
        // Determine if the edge exists in the start and end lists
        bool hasStartEdge = (i < startEdgeList.size());
        bool hasEndEdge = (i < endEdgeList.size());

        Vector2 mixedStartPos;
        Vector2 mixedEndPos;
        float mixedAlpha;

        if (hasStartEdge && hasEndEdge) {
            // Both start and end edges exist, interpolate between them
            mixedStartPos.x = startEdgeList[i].startPos.x + G * (endEdgeList[i].startPos.x - startEdgeList[i].startPos.x);
            mixedStartPos.y = startEdgeList[i].startPos.y + G * (endEdgeList[i].startPos.y - startEdgeList[i].startPos.y);
            mixedEndPos.x = startEdgeList[i].endPos.x + G * (endEdgeList[i].endPos.x - startEdgeList[i].endPos.x);
            mixedEndPos.y = startEdgeList[i].endPos.y + G * (endEdgeList[i].endPos.y - startEdgeList[i].endPos.y);
            mixedAlpha = startEdgeList[i].alpha + G * (endEdgeList[i].alpha - startEdgeList[i].alpha);
        }
        else if (hasStartEdge) {
            // Only the start edge exists, fade it out
            mixedStartPos = startEdgeList[i].startPos;
            mixedEndPos = startEdgeList[i].endPos;
            mixedAlpha = startEdgeList[i].alpha * (step - G);  // Fade out
        }
        else if (hasEndEdge) {
            // Only the end edge exists, fade it in
            mixedStartPos = endEdgeList[i].startPos;
            mixedEndPos = endEdgeList[i].endPos;
            mixedAlpha = endEdgeList[i].alpha * G;  // Fade in
        }

        // Create the mixed edge and add it to the current edges
        Edge mixedEdge(mixedStartPos, mixedEndPos, mixedAlpha);
        curEdges.push_back(mixedEdge);
    }
    if (G >= step) {
        stateIndex++;  // Move to the next state
        elapsedTime = 0.0f;  // Reset the elapsed time
    }
}



void Tree234::debug() {
    for (auto& tree : steps) {
        std::cout << std::endl;
        for (auto& node : tree) {
            std::cout << "Value: " << node->value << ", Position:" << node->position.x << " " << node->position.y << ", Index: " << node->pos << std::endl;
        }
    }
}

void Tree234::print() {
    printTree(root, 0); // Start printing from the root at level 0
}

void Tree234::printTree(Tree234Node* node, int level) {
    if (node == nullptr) return;

    // Indent according to the level in the tree
    for (int i = 0; i < level; i++) {
        std::cout << "    "; // 4 spaces per level
    }

    // Print the values in the current node
    std::cout << "[ ";
    for (size_t i = 0; i < node->honeycombs.size(); i++) {
        std::cout << node->honeycombs[i]->value;
        if (i < node->honeycombs.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " ]" << std::endl;

    // Recursively print the children
    for (Tree234Node* child : node->children) {
        printTree(child, level + 1);
    }
}

int Tree234::getStepsSize() {
    return steps.size();
}


////////////////////////////////////////////////////////////////////////////////////////////////


//HoneycombNode tests(888, { 200,200 });
Tree234 testTree;

void initialize234Tree() {
    for (int i = 1; i <= 10; i++) testTree.insert(i);
}

Interact tree234curInteract = REST;
bool tree234curInteracting = false;

float deltaTime234 = 0.0f;
float elapsedTime234 = 0.0f;
float stepTime234 = 1.0f;
int stateIndex234 = 0;
bool pause234 = false;
bool isDragging234 = false;


void render234Tree(Screen& currentScreen) {
    DrawTexture(tree234BG, 0, 0, WHITE);
    deltaTime234 = GetFrameTime();

    if (!pause234) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pause234 = !pause234;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });
    //if (checkCollision(forWard)) DrawRectangleRec(forWard, Color{ 0, 255, 0, 32 });
    //if (checkCollision(backWard)) DrawRectangleRec(backWard, Color{ 0, 255, 0, 32 });

    if (checkClick(backWard)) {
        pause234 = true;
        if (stateIndex234 > 0) {
            stateIndex234--;
            elapsedTime234 = 0.4f;
            testTree.updateState(stateIndex234, elapsedTime234, deltaTime234, stepTime234);
        }
    }

    if (checkClick(forWard)) {
        pause234 = true;
        if (stateIndex234 < (testTree.getStepsSize() - 2)) {
            stateIndex234++;
            elapsedTime234 = 0.9f;
            testTree.updateState(stateIndex234, elapsedTime234, deltaTime234, stepTime234);
        }
    }


    if (!pause234) testTree.updateState(stateIndex234, elapsedTime234, deltaTime234, stepTime234);;

    // Detect if the mouse is clicking on the slidingButton
    if (checkClick(slidingButton)) {
        isDragging234 = true;  // Start dragging when the button is clicked
    }

    // If the mouse button is released, stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDragging234 = false;
    }

    // Update the position of the slidingButton if it is being dragged
    if (isDragging234) {
        float mouseX = GetMouseX(); // Get the current mouse X position

        // Constrain the slidingButton within the slider bar's range
        float minX = 800.0f; // Left bound of the slider
        float maxX = 1205.0f; // Right bound of the slider

        slidingButton.x = mouseX;

        // Ensure slidingButton doesn't go out of bounds
        if (slidingButton.x < minX) slidingButton.x = minX;
        if (slidingButton.x > maxX) {
            slidingButton.x = maxX;
            stateIndex234 = testTree.getStepsSize() - 1;
        }


        // Update the stateIndex based on the slidingButton's position
        if (testTree.getStepsSize() > 0) {
            float relativePosition = (slidingButton.x - minX) / (maxX - minX);
            stateIndex234 = static_cast<int>(relativePosition * (testTree.getStepsSize() - 1));
            elapsedTime234 = 0.8f; // Reset elapsed time when manually adjusting the state
            pause234 = true;  // Pause the animation when dragging the slider
            testTree.updateState(stateIndex234, elapsedTime234, deltaTime234, stepTime234);
        }
    }
    else {
        // If not dragging, keep the button in sync with the stateIndex
        if (testTree.getStepsSize() > 0)
            slidingButton.x = 800.0f + static_cast<float>(stateIndex234) / static_cast<float>(testTree.getStepsSize() - 1) * 405.0f;
        else
            slidingButton.x = 800.0f;
    }

    // Draw the sliding button
    DrawTexture(slidingBut, slidingButton.x, slidingButton.y, WHITE);

    testTree.draw();

    for (int i = 0; i < 5; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;


    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) {
            if (!tree234curInteracting) {
                tree234curInteract = constants::hashInter[i];
                tree234curInteracting = true;
            }
            else {
                tree234curInteracting = false;
                tree234curInteract = REST;
            }
        }
    }
    tree234Interacting(tree234curInteract);
}

void tree234Interacting(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        tree234Create(state);
    } break;
    case INSERT:
    {
        tree234Insert(state);
    } break;
    case DELETE:
    {
        tree234Delete(state);
    } break;
    case SEARCH:
    {
        tree234Search(state);
    } break;
    case FILEINTER:
    {
        tree234File(state);
    }

    default: break;
    }
};

char tree234inputNumber[4] = "\0";
int tree234numCount = 0;
bool tree234inputClick = false;
bool tree234randomClick = false;
float tree234timePassed = 0.0f;
bool tree234getFile = false;


void tree234File(Interact& state) {
    if (!tree234getFile) {
        std::string selectedFilePath = FileSelectDialog();
        std::vector<int> numbers = ReadNumbersFromFile(selectedFilePath);
        testTree.clearTree();
        testTree = Tree234();
        testTree.finalFile(numbers, stateIndex234, pause234);
        tree234getFile = true;
    }
}

void tree234Insert(Interact& state) {
    DrawTexture(insertSection, hashtableOptions[1].x + 90.0f, hashtableOptions[1].y, WHITE);

    if (checkCollision(randomInsert)) DrawRectangleRec(randomInsert, Color{ 0, 255, 0, 32 });
    if (checkCollision(okInput)) DrawRectangleRec(okInput, Color{ 0, 255, 0, 32 });
    if (checkCollision(inputSection)) {
        DrawRectangleRec(inputSection, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(inputSection)) tree234inputClick = true;
    else if (!checkClick(inputSection) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tree234inputClick = false;

    if (tree234inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (tree234numCount < 3))
            {
                tree234inputNumber[tree234numCount] = (char)key;
                tree234inputNumber[tree234numCount + 1] = '\0'; // Add null terminator at the end of the string.
                tree234numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            tree234numCount--;
            if (tree234numCount < 0) tree234numCount = 0;
            tree234inputNumber[tree234numCount] = '\0';
        }

        if (fmod(tree234timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)inputSection.x + 10 + MeasureText(tree234inputNumber, 20), (int)inputSection.y + 8, 20, DARKGREEN);
        }
    }

    tree234timePassed = GetTime();

    DrawText(tree234inputNumber, (int)inputSection.x + 7, (int)inputSection.y + 4, 20, DARKGREEN);

    if ((checkClick(okInput) || IsKeyPressed(KEY_ENTER)) && !testTree.isInteracting(stateIndex234)) {
        if (tree234numCount > 0) {
            int input = std::stoi(tree234inputNumber);
            testTree.finalInsert(input, stateIndex234, pause234);
            while (tree234numCount > 0) {
                tree234numCount--;
                tree234inputNumber[tree234numCount] = '\0';
            }
        }
    }



    if (checkClick(randomInsert) && !testTree.isInteracting(stateIndex234)) {
        int number = rand() % 1000;
        if (number == 0) number++;
        std::string str = std::to_string(number);
        for (int i = 0; i < str.size(); i++) {
            tree234inputNumber[i] = str[i];
        }
        tree234numCount = str.size();
        tree234inputNumber[tree234numCount] = '\0';
    }


};


void tree234Delete(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[2].x + 90.0f, hashtableOptions[2].y, WHITE);

    if (checkCollision(okDelete)) DrawRectangleRec(okDelete, Color{ 0, 255, 0, 32 });
    if (checkCollision(deleteSectionBox)) {
        DrawRectangleRec(deleteSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(deleteSectionBox)) tree234inputClick = true;
    else if (!checkClick(deleteSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tree234inputClick = false;

    if (tree234inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (tree234numCount < 3))
            {
                tree234inputNumber[tree234numCount] = (char)key;
                tree234inputNumber[tree234numCount + 1] = '\0'; // Add null terminator at the end of the string.
                tree234numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            tree234numCount--;
            if (tree234numCount < 0) tree234numCount = 0;
            tree234inputNumber[tree234numCount] = '\0';
        }

        if (fmod(tree234timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)deleteSectionBox.x + 10 + MeasureText(tree234inputNumber, 20), (int)deleteSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    tree234timePassed = GetTime();

    DrawText(tree234inputNumber, (int)deleteSectionBox.x + 7, (int)deleteSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okDelete) || IsKeyPressed(KEY_ENTER)) && !testTree.isInteracting(stateIndex234)) {
        if (tree234numCount > 0) {
            int input = std::stoi(tree234inputNumber);
            testTree.finalDelete(input, stateIndex234, pause234);
            while (tree234numCount > 0) {
                tree234numCount--;
                tree234inputNumber[tree234numCount] = '\0';
            }
        }
    }

};


void tree234Search(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[3].x + 90.0f, hashtableOptions[3].y, WHITE);

    if (checkCollision(okSearch)) DrawRectangleRec(okSearch, Color{ 0, 255, 0, 32 });
    if (checkCollision(searchSectionBox)) {
        DrawRectangleRec(searchSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(searchSectionBox)) tree234inputClick = true;
    else if (!checkClick(searchSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tree234inputClick = false;

    if (tree234inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (tree234numCount < 2))
            {
                tree234inputNumber[tree234numCount] = (char)key;
                tree234inputNumber[tree234numCount + 1] = '\0'; // Add null terminator at the end of the string.
                tree234numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            tree234numCount--;
            if (tree234numCount < 0) tree234numCount = 0;
            tree234inputNumber[tree234numCount] = '\0';
        }

        if (fmod(tree234timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)searchSectionBox.x + 10 + MeasureText(tree234inputNumber, 20), (int)searchSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    tree234timePassed = GetTime();

    DrawText(tree234inputNumber, (int)searchSectionBox.x + 7, (int)searchSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okSearch) || IsKeyPressed(KEY_ENTER)) && !testTree.isInteracting(stateIndex234)) {
        if (tree234numCount > 0) {
            int input = std::stoi(tree234inputNumber);
            testTree.finalSearch(input, stateIndex234, pause234);
            while (tree234numCount > 0) {
                tree234numCount--;
                tree234inputNumber[tree234numCount] = '\0';
            }
        }
    }

}


void tree234Create(Interact& state) {
    DrawTexture(insertSection, hashtableOptions[0].x + 90.0f, hashtableOptions[0].y, WHITE);

    if (checkCollision(randomCreate)) DrawRectangleRec(randomCreate, Color{ 0, 255, 0, 32 });
    if (checkCollision(ok1random)) DrawRectangleRec(ok1random, Color{ 0, 255, 0, 32 });
    if (checkCollision(random1Section)) {
        DrawRectangleRec(random1Section, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(random1Section)) tree234inputClick = true;
    else if (!checkClick(random1Section) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) tree234inputClick = false;

    if (tree234inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (tree234numCount < 2))
            {
                tree234inputNumber[tree234numCount] = (char)key;
                tree234inputNumber[tree234numCount + 1] = '\0'; // Add null terminator at the end of the string.
                tree234numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            tree234numCount--;
            if (tree234numCount < 0) tree234numCount = 0;
            tree234inputNumber[tree234numCount] = '\0';
        }

        if (fmod(tree234timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)random1Section.x + 10 + MeasureText(tree234inputNumber, 20), (int)random1Section.y + 8, 20, DARKGREEN);
        }
    }

    tree234timePassed = GetTime();

    DrawText(tree234inputNumber, (int)random1Section.x + 7, (int)random1Section.y + 4, 20, DARKGREEN);


    if (tree234numCount > 0) {
        int inputValue = std::stoi(tree234inputNumber);  // Convert the string to an integer

        if (inputValue > 30) {
            // Reset the input to "30"
            tree234inputNumber[0] = '3';
            tree234inputNumber[1] = '0';
            tree234inputNumber[2] = '\0';
            tree234numCount = 2;  // Update the count to reflect the new length of the input
        }
    }

    if ((checkClick(okInput) || IsKeyPressed(KEY_ENTER)) && !testTree.isInteracting(stateIndex234)) {
        if (tree234numCount > 0) {
            int input = std::stoi(tree234inputNumber);
            testTree.clearTree();
            testTree = Tree234();
            testTree.finalCreate(input, stateIndex234, pause234);
            while (tree234numCount > 0) {
                tree234numCount--;
                tree234inputNumber[tree234numCount] = '\0';
            }
        }
    }



    if (checkClick(randomCreate) && !testTree.isInteracting(stateIndex234)) {
        int number = rand() % 31;
        if (number == 0) number++;
        std::string str = std::to_string(number);
        for (int i = 0; i < str.size(); i++) {
            tree234inputNumber[i] = str[i];
        }
        tree234numCount = str.size();
        tree234inputNumber[tree234numCount] = '\0';
    }



}