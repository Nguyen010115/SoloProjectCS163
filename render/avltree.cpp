#include "avltree.h"

//Node constructing
void avlNode::draw() {
    // Determine the color with fading effect using the alpha value
    Color gradientStart = ColorAlpha(YELLOW, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);

    if (visiting) {
        outlineColor = ColorAlpha(RED, alphavisiting);
    }
    if (balanceVisit) {
        gradientStart = ColorAlpha(BLUE, alphavisiting);
        outlineColor = ColorAlpha(BLUE, alphavisiting);
    }
    if (searchVisit) {
        gradientEnd = ColorAlpha(RED, alphavisiting);
    }

    if (value == 0) return;

    DrawCircleV(position, radius, outlineColor);

    DrawCircleGradient(position.x, position.y, radius - 5.0f, WHITE, gradientEnd);
    // Draw ring outline

    // Calculate positions for the value and balance text
    int valueFontSize = 20; // Adjust font size as needed
    int balanceFontSize = 15; // Adjust font size as needed

    // Draw value text at the center of the node
    std::string valueStr = std::to_string(value);
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);

    // Draw balance text above the node
    std::string balanceStr = std::to_string(balance);
    Vector2 balancePos = { position.x - MeasureText(balanceStr.c_str(), balanceFontSize) / 2,
                          position.y - radius
        - balanceFontSize };
    DrawText(balanceStr.c_str(), balancePos.x, balancePos.y, balanceFontSize, textColor);
}


//Line constructring
// edge constructor
edge::edge(int st, int en) : startIndex(st), endIndex(en) {}

// edge draw method
void edge::draw(const avlNode* startNode, const avlNode* endNode) {
    if (!startNode || !endNode) return;
    Color color = ColorAlpha(BLACK, alpha);
    if (startNode->value != 0 && endNode->value != 0) DrawLineEx(startNode->position, endNode->position, 3.0f, color);
}



//AVL Class constructing
void avlTree::insert(int value) {
    bool inserted = false;
    root = insertRecursive(root, value, inserted);
    updatePositions();
    updateEdges();
    copyTree();
}

avlNode* avlTree::insertRecursive(avlNode* &node, int value, bool &inserted) {
    if (node == nullptr) {
        avlNode* temp = new avlNode(value);
        temp->pos = size;
        nodeList.push_back(temp);
        size++;
        //inserted = true;
        if (root == NULL) {
            root = temp;
            updatePositions();
            updateEdges();
            copyTree();
        }
        else {
            updatePositions();
            updateEdges();
            copyTree();
        }
        return temp;
    }

    node->alphavisiting = 1.0f;
    node->visiting = true;
    copyTree();


    if (value < node->value) {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        node->left = insertRecursive(node->left, value, inserted);
        if (node->left) node->left->parent = node;
    }
    else if (value > node->value) {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        node->right = insertRecursive(node->right, value, inserted);
        if (node->right) node->right->parent = node;
    }
    else {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        return node; // Duplicate values are not allowed
    }

    /*node->alphavisiting = 0.0f;
    node->visiting = false;
    copyTree();*/
    updateHeightAndBalance(node);
    updatePositions();
    updateEdges();

    if (!inserted) {
        copyTree();
        inserted = true;
    }

    
    return balanceTree(node);

}


void avlTree::updateHeight(avlNode* node) {
    if (node == nullptr) return;
    int leftHeight = (node->left != nullptr) ? node->left->height : 0;
    int rightHeight = (node->right != nullptr) ? node->right->height : 0;
    node->height = std::max(leftHeight, rightHeight) + 1;
}

void avlTree::updateBalance(avlNode* node) {
    if (node == nullptr) return;
    int leftHeight = (node->left != nullptr) ? node->left->height : 0;
    int rightHeight = (node->right != nullptr) ? node->right->height : 0;
    node->balance = leftHeight - rightHeight;
}

avlNode* avlTree::rotateLeft(avlNode* &node) {

    avlNode* newRoot = node->right;
    node->right = newRoot->left;
    if (newRoot->left != nullptr) {
        newRoot->left->parent = node;
    }
    newRoot->left = node;
    newRoot->parent = node->parent;
    node->parent = newRoot;

    // Update heights and balance factors
    updateHeight(node);
    updateHeight(newRoot);
    updateBalance(node);
    updateBalance(newRoot);

    updatePositions();
    updateEdges();

    return newRoot;
}

avlNode* avlTree::rotateRight(avlNode* &node) {
    avlNode* newRoot = node->left;
    node->left = newRoot->right;
    if (newRoot->right != nullptr) {
        newRoot->right->parent = node;
    }
    newRoot->right = node;
    newRoot->parent = node->parent;
    node->parent = newRoot;

    // Update heights and balance factors
    updateHeight(node);
    updateHeight(newRoot);
    updateBalance(node);
    updateBalance(newRoot);
    updatePositions();
    updateEdges();


    return newRoot;
}

avlNode* avlTree::balanceTree(avlNode* &node) {
    if (node == nullptr) return nullptr;

    node->alphavisiting = 1.0f;
    node->balanceVisit = true;
    copyTree();

    // Left heavy subtree
    if (node->balance > 1) {
        node->alphavisiting = 0.0f;
        node->balanceVisit = false;
        if (node->left != nullptr && node->left->balance < 0) {
            node->left = rotateLeft(node->left);
            copyTree();
        }
        node = rotateRight(node);
    }

    // Right heavy subtree
    else if (node->balance < -1) {
        node->alphavisiting = 0.0f;
        node->balanceVisit = false;
        if (node->right != nullptr && node->right->balance > 0) {
            node->right = rotateRight(node->right);
            copyTree();
        }
        node = rotateLeft(node);
    }

    updatePositions();
    updateEdges();

    node->alphavisiting = 0.0f;
    node->balanceVisit = false;
    // Already balanced
    return node;
}


void avlTree::updateHeightAndBalance(avlNode* node) {
    if (node == nullptr) return;
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    node->balance = getHeight(node->left) - getHeight(node->right);
}

int avlTree::getHeight(avlNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

int avlTree::getBalance(avlNode* node) {
    return (node == nullptr) ? 0 : node->balance;
}



void avlTree::updatePositions() {
    if (size == 0) return; // If the tree is empty, do nothing

    // Initial settings: root node is centered at the top
    float initialX = constants::scene_width; // X position
    float levelSpacing = 50.0f;      // Vertical distance between levels
    updateNodePosition(root, 0, 0, initialX, levelSpacing);
}

void avlTree::updateNodePosition(avlNode* root, int level, float minX, float maxX, float levelSpacing) {
    if (root == NULL) return;

    // Set the position of the current node
    root->position.x = (minX + maxX) / 2;
    root->position.y = 120.0f + level * levelSpacing;

    // Recursively set positions for left and right children
    if (root->left != NULL) {
        updateNodePosition(root->left, level + 1, minX, root->position.x, levelSpacing);
    }
    if (root->right != NULL) {
        updateNodePosition(root->right, level + 1, root->position.x, maxX, levelSpacing);
    }
}


avlNode* avlTree::deepCopy(avlNode* root) {
    if (!root) return nullptr;

    avlNode* newNode = new avlNode(root->value, root->pos);
    newNode->left = deepCopy(root->left);
    if (newNode->left) newNode->left->parent = newNode;

    newNode->right = deepCopy(root->right);
    if (newNode->right) newNode->right->parent = newNode;

    newNode->parent = nullptr; // You might want to handle parent linkage appropriately
    newNode->height = root->height;
    newNode->balance = root->balance;
    newNode->position = root->position;
    newNode->radius = root->radius;
    newNode->alpha = root->alpha;
    newNode->visiting = root->visiting; // Copy visiting status
    newNode->alphavisiting = root->alphavisiting;
    newNode->balanceVisit = root->balanceVisit;
    newNode->searchVisit = root->searchVisit;

    return newNode;
}



void avlTree::treeToArray(avlNode* root, std::vector<avlNode*>& list){
    if (root == nullptr) return;

    // Ensure the list can accommodate the position index
    if (root->pos >= 0 && root->pos < list.size()) {
        list[root->pos] = root;
    }

    // Recursively call for left and right subtrees
    treeToArray(root->left, list);
    treeToArray(root->right, list);
}

void avlTree::copyTree() {
    std::vector<avlNode*> temp(size);
    avlNode* step = deepCopy(root);

    stepRoots.push_back(step);
    treeToArray(step, temp);
    steps.push_back(temp);

    stepEdges.push_back(copyEdges());
}


void avlTree::printInOrder(avlNode* node) {
    if (node == nullptr) return;

    printInOrder(node->left);

    std::cout << "Value: " << node->value << ", Pos: " << node->pos
        << ", Height: " << node->height << ", Balance: " << node->balance
        << ", Visiting: " << (node->visiting ? "True" : "False") << ", BalanceVisiting: " << (node->balanceVisit ? "True" : "False") << " PositionXY:" << node->position.x << " " << node->position.y << std::endl;

    printInOrder(node->right);
}

void avlTree::updateState(int& stateIndex, float& elapsedTime, float deltaTime) {
    if (stateIndex < 0 || stateIndex >= steps.size() - 1) return;

    float G = elapsedTime / deltaTime;
    if (G > 1.0f) G = 1.0f; 

    std::vector<avlNode*>& currentStep = steps[stateIndex];
    std::vector<avlNode*>& nextStep = steps[stateIndex + 1];
    std::vector<edge>& currentEdges = stepEdges[stateIndex];
    std::vector<edge>& nextEdges = stepEdges[stateIndex + 1];

    curList.clear();
    edgeList.clear();

    size_t maxSize = std::max(currentStep.size(), nextStep.size()); 
    for (size_t i = 0; i < maxSize; ++i) {
        avlNode* mixedNode = new avlNode(0);
        avlNode* startNode = (i < currentStep.size()) ? currentStep[i] : nullptr;
        avlNode* endNode = (i < nextStep.size()) ? nextStep[i] : nullptr;

        mixNodes(mixedNode, startNode, endNode, G);
        if (i < curList.size()) curList[i] = mixedNode;
        else curList.push_back(mixedNode);
    }


    size_t maxEdgeSize = std::max(currentEdges.size(), nextEdges.size());
    for (size_t i = 0; i < maxEdgeSize; ++i) {
        edge mixedEdge = (i < nextEdges.size()) ? nextEdges[i] : currentEdges[i];
        if (i < curEdge.size()) { 
            mixedEdge.alpha = 1.0f;
            curEdge[i] = mixedEdge; }
        else {
            mixedEdge.alpha = G;
            curEdge.push_back(mixedEdge);
        }
    }
}

void avlTree::updateTree(float deltaTime, float &elapsedTime, int& stateIndex, float stepTime) {
    elapsedTime += deltaTime;

    if (elapsedTime >= stepTime) {
        elapsedTime = 0.0f;
        stateIndex++;
        if (stateIndex >= steps.size() - 1) {
            stateIndex = steps.size() - 1;
        }
    }

    if (steps.size() > 0) updateState(stateIndex, elapsedTime, stepTime);
}


void avlTree::updateEdges() {
    edgeList.clear();
    updateEdgesFromNode(root);

}


void avlTree::updateEdgesFromNode(avlNode* node) {
    if (node == nullptr) return;

    if (node->left != nullptr) {
        edgeList.emplace_back(node->pos, node->left->pos);
        updateEdgesFromNode(node->left);
    }
    if (node->right != nullptr) {
        edgeList.emplace_back(node->pos, node->right->pos);
        updateEdgesFromNode(node->right);
    }
}

std::vector<edge> avlTree::copyEdges() {
    std::vector<edge> copiedEdges;
    for (const edge& e : edgeList) {
        copiedEdges.push_back(e);
    }
    return copiedEdges;
}

avlNode* avlTree::deleteRecursive(avlNode* node, int value) {
    if (node == nullptr) return node;

    node->alphavisiting = 1.0f;
    node->visiting = true;
    updatePositions();
    updateEdges();
    copyTree();


    if (value < node->value) {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        //printInOrder(root);
        //copyTree();
        node->left = deleteRecursive(node->left, value);
    }
    else if (value > node->value) {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        //printInOrder(root);
        //copyTree();
        node->right = deleteRecursive(node->right, value);
    }
    else {
        if (node->left == nullptr || node->right == nullptr) {
            avlNode* temp = node->left ? node->left : node->right;
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else {
                *node = *temp;
            }
            delete temp;
        }
        else {
            avlNode* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->value = temp->value;
            node->right = deleteRecursive(node->right, temp->value);
        }
    }

    if (node == nullptr) return node;

    node->alphavisiting = 0.0f;
    node->visiting = false;
    updatePositions();
    updateEdges();
    copyTree();

    updateHeightAndBalance(node);
    return balanceTree(node);
}

void avlTree::remove(int value) {
    updateEdges();
    copyTree();
    root = deleteRecursive(root, value);
    if (root != nullptr) {
        updatePositions();
        updateEdges();
        copyTree();
    }
}


void avlTree::drawEdges() {
    for (edge& e : curEdge) {
        const avlNode* startNode = (e.startIndex >= 0 && e.startIndex < curList.size()) ? curList[e.startIndex] : nullptr;
        const avlNode* endNode = (e.endIndex >= 0 && e.endIndex < curList.size()) ? curList[e.endIndex] : nullptr;
        if (!(startNode == NULL || endNode == NULL)) e.draw(startNode, endNode);
    }
}


void avlTree::draw() {
    if (root == NULL) return;
    drawEdges();
    for (avlNode* nodes : curList) {
        nodes->draw();
    }
}

void avlTree::mixNodes(avlNode* target, avlNode* start, avlNode* end, float mixCoeff) {
    if (!start && !end) return;

    if (start && end) {
        // Interpolate position
        if (fabs(start->position.x - end->position.x) > 1.0f) target->position.x = start->position.x * (1.0f - mixCoeff) + end->position.x * mixCoeff;
        else  target->position.x = end->position.x;

        if (fabs(start->position.y - end->position.y) > 1.0f) target->position.y = start->position.y * (1.0f - mixCoeff) + end->position.y * mixCoeff;
        else  target->position.y = end->position.y;

        // Interpolate alpha
        target->alpha = start->alpha * (1.0f - mixCoeff) + end->alpha * mixCoeff;
        target->alphavisiting = start->alphavisiting * (1.0f - mixCoeff) + end->alphavisiting * mixCoeff;

        // Interpolate radius (if needed)
        target->visiting = end->visiting;
        target->balanceVisit = end->balanceVisit;
        target->searchVisit = end->searchVisit;
        
        target->value = start->value;
        target->balance = start->balance;
    }
    else if (start) {
        // Fading out effect for deleted nodes
        target->position = start->position;
        if (fabs(mixCoeff - 1.0f) < 0.05f) target->alpha = 0;
        else target->alpha = start->alpha * (1.0f - mixCoeff);

        target->value = start->value;
        target->balance = start->balance;
    }
    else if (end) {
        // Fading in effect for new nodes
        target->position = end->position;
        target->alpha = end->alpha * mixCoeff;
        target->radius = end->radius;
        target->value = end->value;
        target->balance = end->balance;
    }
}

void avlTree::finalDelete(int value, int& stateIndex, bool& pause) {
    pause = true;

    steps.clear();
    stepEdges.clear();
    copyTree();
    stateIndex = 0;
    remove(value);
    pause = false;

}

void avlTree::finalInsert(int value, int& stateIndex, bool& pause) {
    pause = true;

    steps.clear();
    stepEdges.clear();
    copyTree();
    stateIndex = 0;
    insert(value);
    pause = false;

}

void avlTree::finalCreate(int value, int& stateIndex, bool& pause) {
    pause = true;

    clearTree();
    root = nullptr;
    stateIndex = 0;
    for (int i = 0; i < value;i++) {
        int s = rand() % 1000;
        if (s == 0) s++;
        insert(s);
    }
    pause = false;

}

void avlTree::finalFile(std::vector<int>& input, int& stateIndex, bool& pause) {
    pause = true;

    clearTree();
    root = nullptr;
    stateIndex = 0;
    for (auto i : input) insert(i);
    pause = false;
}


bool avlTree::isInteracting(int state) {
    if (steps.size() == 0) return false;
    return (state < steps.size() - 1);
}

bool avlTree::search(int value, int& stateIndex, bool& pause) {
    pause = true;

    steps.clear();
    stepEdges.clear();
    copyTree();
    stateIndex = 0;
    avlNode* searchNode = searchRecursive(root, value);
    if (!searchNode) return false;
    searchNode->searchVisit = true;
    searchNode->alphavisiting = 1.0f;
    copyTree();
    searchNode->searchVisit = false;
    searchNode->alphavisiting = 0.0f;
    copyTree();
    searchNode->searchVisit = true;
    searchNode->alphavisiting = 1.0f;
    copyTree();
    searchNode->searchVisit = false;
    searchNode->alphavisiting = 0.0f;
    copyTree();
    pause = false;
    return true;
}

avlNode* avlTree::searchRecursive(avlNode* node, int value) {
    if (node == nullptr || node->value == value) {       
        return node;
    }
    
    node->alphavisiting = 1.0f;
    node->visiting = true;
    copyTree();


    if (value < node->value) {
        node->alphavisiting = 0.0f;
        node->visiting =    false;
        return searchRecursive(node->left, value);
    }
    else
    {
        node->alphavisiting = 0.0f;
        node->visiting = false;
        searchRecursive(node->right, value);
    } 
}

void avlTree::clearTree() {
    // Recursively delete all nodes in the tree
    clearTreeRecursive(root);

    // Reset the root to nullptr after clearing the tree
    root = nullptr;

    // Delete all dynamically allocated nodes in nodeList
    nodeList.clear(); // Clear the list of nodes

    edgeList.clear(); // Clear the list of edges

    // Clear the history of steps and dynamically allocated nodes in steps
    for (auto& step : steps) {
        for (auto& node : step) {
            if (node != nullptr) {
                delete node;
                node = nullptr;
            }
        }
    }
    steps.clear();    // Clear the history of steps

    stepRoots.clear(); // Clear the history of root steps
    stepEdges.clear(); // Clear the history of edges steps
    curList.clear();  // Clear the current list of nodes
    size = 0;         // Reset the size of the tree
}


void avlTree::clearTreeRecursive(avlNode* node) {
    if (node == nullptr) return;

    // Recursively clear left and right subtrees
    clearTreeRecursive(node->left);
    clearTreeRecursive(node->right);

    // Delete the current node
    delete node;
}

int avlTree::getStepsSize() {
    return steps.size();
}




char AVLinputNumber[4] = "\0";
int AVLnumCount = 0;

//char inputRandom[3] = "\0";
//int randomCount = 0;


bool avlcheck = false;
bool avlinteracting = false;
Interact avlcurInteract = REST;
bool AVLinputClick = false;
bool AVLrandomClick = false;
avlTree avl;
float AVLtimePassed = 0.0f;



void initializeAVL() {
    avl.insert(1);
    avl.insert(2);
    avl.insert(3);
    avl.insert(4);
}

float deltaTime = 0.0f;
float elapsedTime = 0.0f;
float stepTime = 1.0f;
int stateIndex = 0;
bool pause = false;
bool isDragging = false;
bool getFile = false;
bool doubleSpeed = false;


void renderAVLtree(Screen& currentScreen) {
	DrawTexture(avlBG, 0, 0, WHITE);
    deltaTime = GetFrameTime();
    if (checkClick(changeSpeed)) doubleSpeed = !doubleSpeed;
    if (!doubleSpeed) {
        stepTime = 1.0f;
        DrawTexture(speed1x, changeSpeed.x, changeSpeed.y, WHITE);
    }
    else {
        stepTime = 0.5f;
        DrawTexture(speed2x, changeSpeed.x, changeSpeed.y, WHITE);
    }

    if (!pause) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pause = !pause;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });
   
    if (checkClick(backWard)) {
        pause = true;
        if (stateIndex > 0) {
            stateIndex--;
            elapsedTime = 0.8f*stepTime;
            avl.updateTree(deltaTime, elapsedTime, stateIndex, stepTime);
        }
    }

    if (checkClick(forWard)) {
        pause = true;
        if (stateIndex < (avl.getStepsSize() - 2)) {
            stateIndex++;
            elapsedTime = 0.8f*stepTime;
            avl.updateTree(deltaTime, elapsedTime, stateIndex, stepTime);
        }
    }


    if (!pause) avl.updateTree(deltaTime, elapsedTime, stateIndex, stepTime);

    if (checkClick(slidingButton)) {
        isDragging = true; 
    }

    // If the mouse button is released, stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDragging = false;
    }

    // Update the position of the slidingButton if it is being dragged
    if (isDragging) {
        float mouseX = GetMouseX(); // Get the current mouse X position

        // Constrain the slidingButton within the slider bar's range
        float minX = 800.0f; // Left bound of the slider
        float maxX = 1205.0f; // Right bound of the slider

        slidingButton.x = mouseX;

        // Ensure slidingButton doesn't go out of bounds
        if (slidingButton.x < minX) slidingButton.x = minX;
        if (slidingButton.x > maxX) {
            slidingButton.x = maxX;
            stateIndex = avl.getStepsSize() - 1;
        }


        // Update the stateIndex based on the slidingButton's position
        if (avl.getStepsSize() > 0) {
            float relativePosition = (slidingButton.x - minX) / (maxX - minX);
            stateIndex = static_cast<int>(relativePosition * (avl.getStepsSize() - 1));
            elapsedTime = 0.8f; // Reset elapsed time when manually adjusting the state
            pause = true;  // Pause the animation when dragging the slider
            avl.updateTree(deltaTime, elapsedTime, stateIndex, stepTime);  // Update the tree state
        }
    }
    else {
        // If not dragging, keep the button in sync with the stateIndex
        if (avl.getStepsSize() > 0)
            slidingButton.x = 800.0f + static_cast<float>(stateIndex) / static_cast<float>(avl.getStepsSize() - 1) * 405.0f;
        else
            slidingButton.x = 800.0f;
    }

    // Draw the sliding button
    DrawTexture(slidingBut, slidingButton.x, slidingButton.y, WHITE);

    avl.draw();

    for (int i = 0; i < 5; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;


    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) {
            if (!avlinteracting) {
                avlcurInteract = constants::hashInter[i];
                avlinteracting = true;
                getFile = false;
            }
            else {
                avlinteracting = false;
                avlcurInteract = REST;
            }
        }
    }
    avlInteracting(avlcurInteract);
}

void avlInteracting(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        avlCreate(state);
    } break;
    case INSERT:
    {
        avlInsert(state);
    } break;
    case DELETE:
    {
        avlDelete(state);
    } break;
    case SEARCH:
    {
        avlSearch(state);
    } break;
    case FILEINTER:
    {
        avlFile(state);
    }
    default: break;
    }
};

void avlFile(Interact& state) {
    if (!getFile) {
        std::string selectedFilePath = FileSelectDialog();
        std::vector<int> numbers = ReadNumbersFromFile(selectedFilePath);
        avl.finalFile(numbers, stateIndex, pause);
        getFile = true;
    }
}

void avlInsert(Interact& state) {
    DrawTexture(insertSection, hashtableOptions[1].x + 90.0f, hashtableOptions[1].y, WHITE);

    if (checkCollision(randomInsert)) DrawRectangleRec(randomInsert, Color{ 0, 255, 0, 32 });
    if (checkCollision(okInput)) DrawRectangleRec(okInput, Color{ 0, 255, 0, 32 });
    if (checkCollision(inputSection)) {
        DrawRectangleRec(inputSection, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(inputSection)) AVLinputClick = true;
    else if (!checkClick(inputSection) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) AVLinputClick = false;

    if (AVLinputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (AVLnumCount < 3))
            {
                AVLinputNumber[AVLnumCount] = (char)key;
                AVLinputNumber[AVLnumCount + 1] = '\0'; // Add null terminator at the end of the string.
                AVLnumCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            AVLnumCount--;
            if (AVLnumCount < 0) AVLnumCount = 0;
            AVLinputNumber[AVLnumCount] = '\0';
        }

        if (fmod(AVLtimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)inputSection.x + 10 + MeasureText(AVLinputNumber, 20), (int)inputSection.y + 8, 20, DARKGREEN);
        }
    }

    AVLtimePassed = GetTime();

    DrawText(AVLinputNumber, (int)inputSection.x + 7, (int)inputSection.y + 4, 20, DARKGREEN);

    if ((checkClick(okInput) || IsKeyPressed(KEY_ENTER)) && !avl.isInteracting(stateIndex)) {
        if (AVLnumCount > 0) {
            int input = std::stoi(AVLinputNumber);
            avl.finalInsert(input, stateIndex,pause);
            while (AVLnumCount > 0) {
                AVLnumCount--;
                AVLinputNumber[AVLnumCount] = '\0';
            }
        }
    }



    if (checkClick(randomInsert) && !avl.isInteracting(stateIndex)) {
        int number = rand() % 1000;
        if (number == 0) number++;
        std::string str = std::to_string(number);
        for (int i = 0; i < str.size(); i++) {
            AVLinputNumber[i] = str[i];
        }
        AVLnumCount = str.size();
        AVLinputNumber[AVLnumCount] = '\0';
    }


};


void avlDelete(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[2].x + 90.0f, hashtableOptions[2].y, WHITE);

    if (checkCollision(okDelete)) DrawRectangleRec(okDelete, Color{ 0, 255, 0, 32 });
    if (checkCollision(deleteSectionBox)) {
        DrawRectangleRec(deleteSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(deleteSectionBox)) AVLinputClick = true;
    else if (!checkClick(deleteSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) AVLinputClick = false;

    if (AVLinputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (AVLnumCount < 3))
            {
                AVLinputNumber[AVLnumCount] = (char)key;
                AVLinputNumber[AVLnumCount + 1] = '\0'; // Add null terminator at the end of the string.
                AVLnumCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            AVLnumCount--;
            if (AVLnumCount < 0) AVLnumCount = 0;
            AVLinputNumber[AVLnumCount] = '\0';
        }

        if (fmod(AVLtimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)deleteSectionBox.x + 10 + MeasureText(AVLinputNumber, 20), (int)deleteSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    AVLtimePassed = GetTime();

    DrawText(AVLinputNumber, (int)deleteSectionBox.x + 7, (int)deleteSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okDelete) || IsKeyPressed(KEY_ENTER)) && !avl.isInteracting(stateIndex)) {
        if (AVLnumCount > 0) {
            int input = std::stoi(AVLinputNumber);
            avl.finalDelete(input, stateIndex, pause);
            while (AVLnumCount > 0) {
                AVLnumCount--;
                AVLinputNumber[AVLnumCount] = '\0';
            }
        }
    }

};


void avlSearch(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[3].x + 90.0f, hashtableOptions[3].y, WHITE);

    if (checkCollision(okSearch)) DrawRectangleRec(okSearch, Color{ 0, 255, 0, 32 });
    if (checkCollision(searchSectionBox)) {
        DrawRectangleRec(searchSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(searchSectionBox)) AVLinputClick = true;
    else if (!checkClick(searchSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) AVLinputClick = false;

    if (AVLinputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (AVLnumCount < 2))
            {
                AVLinputNumber[AVLnumCount] = (char)key;
                AVLinputNumber[AVLnumCount + 1] = '\0'; // Add null terminator at the end of the string.
                AVLnumCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            AVLnumCount--;
            if (AVLnumCount < 0) AVLnumCount = 0;
            AVLinputNumber[AVLnumCount] = '\0';
        }

        if (fmod(AVLtimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)searchSectionBox.x + 10 + MeasureText(AVLinputNumber, 20), (int)searchSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    AVLtimePassed = GetTime();

    DrawText(AVLinputNumber, (int)searchSectionBox.x + 7, (int)searchSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okSearch) || IsKeyPressed(KEY_ENTER)) && !avl.isInteracting(stateIndex)) {
        if (AVLnumCount > 0) {
            int input = std::stoi(AVLinputNumber);
            avl.search(input, stateIndex, pause);
            while (AVLnumCount > 0) {
                AVLnumCount--;
                AVLinputNumber[AVLnumCount] = '\0';
            }
        }
    }

}


void avlCreate(Interact& state) {
    DrawTexture(insertSection, hashtableOptions[0].x + 90.0f, hashtableOptions[0].y, WHITE);

    if (checkCollision(randomCreate)) DrawRectangleRec(randomCreate, Color{ 0, 255, 0, 32 });
    if (checkCollision(ok1random)) DrawRectangleRec(ok1random, Color{ 0, 255, 0, 32 });
    if (checkCollision(random1Section)) {
        DrawRectangleRec(random1Section, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(random1Section)) AVLinputClick = true;
    else if (!checkClick(random1Section) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) AVLinputClick = false;

    if (AVLinputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (AVLnumCount < 2))
            {
                AVLinputNumber[AVLnumCount] = (char)key;
                AVLinputNumber[AVLnumCount + 1] = '\0'; // Add null terminator at the end of the string.
                AVLnumCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            AVLnumCount--;
            if (AVLnumCount < 0) AVLnumCount = 0;
            AVLinputNumber[AVLnumCount] = '\0';
        }

        if (fmod(AVLtimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)random1Section.x + 10 + MeasureText(AVLinputNumber, 20), (int)random1Section.y + 8, 20, DARKGREEN);
        }
    }

    AVLtimePassed = GetTime();

    DrawText(AVLinputNumber, (int)random1Section.x + 7, (int)random1Section.y + 4, 20, DARKGREEN);


    if (AVLnumCount > 0) {
        int inputValue = std::stoi(AVLinputNumber);  // Convert the string to an integer

        if (inputValue > 50) {
            // Reset the input to "30"
            AVLinputNumber[0] = '5';
            AVLinputNumber[1] = '0';
            AVLinputNumber[2] = '\0';
            AVLnumCount = 2;  // Update the count to reflect the new length of the input
        }
    }

    if ((checkClick(okInput) || IsKeyPressed(KEY_ENTER)) && !avl.isInteracting(stateIndex)) {
        if (AVLnumCount > 0) {
            int input = std::stoi(AVLinputNumber);
            avl.finalCreate(input, stateIndex, pause);
            while (AVLnumCount > 0) {
                AVLnumCount--;
                AVLinputNumber[AVLnumCount] = '\0';
            }
        }
    }
    
 

    if (checkClick(randomCreate) && !avl.isInteracting(stateIndex)) {
        int number = rand()%51;
        if (number == 0) number++;
        std::string str = std::to_string(number);
        for (int i = 0; i < str.size(); i++) {
            AVLinputNumber[i] = str[i];
        }
        AVLnumCount = str.size();
        AVLinputNumber[AVLnumCount] = '\0';
    }



}