#include "trie.h"


void TrieEdge::draw(std::vector<TrieNode*>& list) {
    if (startNode == -1 || endNode == -1) return;
    if (list[startNode]->ch == '0' || list[endNode]->ch == '0') return;
    DrawLineEx(list[startNode]->position, list[endNode]->position, 2.0f, Fade(BLACK, alpha));
}

void TrieNode::draw() {
    if (ch == '0') return;

    // Determine the color with fading effect using the alpha value
    Color gradientStart = ColorAlpha(YELLOW, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);


    if (isEndOfWord) {
        gradientEnd = ColorAlpha(YELLOW, alpha);
    }


    if (visiting) {
        outlineColor = ColorAlpha(RED, alpha);
    }
    

    DrawCircleV(position, radius, outlineColor);

    DrawCircleGradient(position.x, position.y, radius - 5.0f, WHITE, gradientEnd);

    int valueFontSize = 18; // Adjust font size as needed

    // Draw value text at the center of the node
    std::string valueStr;
    valueStr.push_back(ch);
    if (ch == '1') { valueStr.resize(0); valueStr.push_back('r'); valueStr.push_back('o');
    valueStr.push_back('o');
    valueStr.push_back('t');
    }
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);
}

void Trie::insert(const std::string& word) {

    TrieNode* current = root;
    for (char ch : word) {
        current->visiting = true;
        copyTree();
        if (current->children.find(ch) == current->children.end()) {
            TrieNode* temp = new TrieNode(size, { 0,0 }, ch);
            current->children[ch] = temp;
            nodeList.push_back(temp);
            size++;
        }
        current->visiting = false;
        copyTree();
        current = current->children[ch];
    }
    current->isEndOfWord = true;
    copyTree();
}

void Trie::updatePositions() {
    if (size == 0) return; // If the tree is empty, do nothing

    // Initial settings: root node is centered at the top
    float initialX = constants::scene_width-100; // X position
    float levelSpacing = 60.0f;      // Vertical distance between levels
    updateNodePosition(root, 0, 100, initialX, levelSpacing);
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
        float childMaxX = childMinX + (maxX - minX) * (static_cast<float>(childLeaves) / totalLeaves);

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
    node->visiting = true;
    copyTree();
    // Base case: if we've reached the end of the word
    if (depth == word.size()) {
        // If this node is the end of the word, unmark it
        if (!node->isEndOfWord) {
            node->visiting = false;

            return false;  // The word was not found
        }

        node->isEndOfWord = false;
        node->visiting = false;

        // If the node has no children, it can be deleted
        return node->children.empty();
    }

    char ch = word[depth];
    TrieNode* childNode = node->children[ch];

    if (childNode == nullptr) {
        node->visiting = false;
        copyTree();
        return false;  // The word was not found
    }

    // Recursively delete the child node
    bool shouldDeleteChild = removeHelper(childNode, word, depth + 1);

    // If the child should be deleted, remove it from the map
    if (shouldDeleteChild) {
        childNode->ch = '0';
        node->children.erase(ch);
        node->visiting = false;
        copyTree();

        // If the current node has no other children and is not the end of another word, it can be deleted
        return !node->isEndOfWord && node->children.empty();
    }

    if (node) node->visiting = false;
    copyTree();

    return false;
}


bool Trie::remove(const std::string& word) {
    if (root == nullptr || word.empty()) {
        return false;
    }

    return removeHelper(root, word, 0);
}


bool Trie::search(const std::string& word) {
    TrieNode* current = root;
    std::vector<TrieNode*> visitedNodes;

    for (char ch : word) {
        current->visiting = true;
        copyTree();  // Copy the current state of the Trie for visualization
        visitedNodes.push_back(current);  // Keep track of visited nodes

        // Check if the current character exists in the current node's children
        if (current->children.find(ch) == current->children.end()) {
            // Unvisit all nodes before returning false
            for (TrieNode* node : visitedNodes) {
                node->visiting = false;
            }
            copyTree();
            return false;  // The character was not found, so the word is not in the Trie
        }

        // Move to the child node corresponding to the current character
        current = current->children[ch];
    }

    // Unvisit all nodes after traversing the word
    for (TrieNode* node : visitedNodes) {
        node->visiting = false;
    }
    copyTree();
    return current->isEndOfWord;
}



void Trie::updateEdgesHelper(TrieNode* node, std::vector<TrieEdge*>& edges) {
    if (!node) return;

    for (auto& pair : node->children) {
        if (!pair.second) continue;
        TrieEdge* edge = new TrieEdge(node->index, pair.second->index);
        edges.push_back(edge);
        updateEdgesHelper(pair.second, edges);
    }
   

}

void Trie::updateEdges(TrieNode* node) {
    curEdge.clear();
    updateEdgesHelper(node, curEdge);
}

void Trie::copyTree() {
    // Update positions and edges before copying the tree
    updatePositions();
    updateEdges(root);

    // Copy the nodes into a new vector
    std::vector<TrieNode*> nodeListCopy;
    for (TrieNode* node : nodeList) {
        // Create a deep copy of each TrieNode
        TrieNode* copyNode = new TrieNode(node->index, node->position, node->ch);
        copyNode->alpha = node->alpha;
        copyNode->visiting = node->visiting;
        copyNode->isEndOfWord = node->isEndOfWord;

        nodeListCopy.push_back(copyNode);
    }

    // Store the copied nodes in the steps vector for visualization
    steps.push_back(nodeListCopy);

    // Copy the edges into a new vector
    std::vector<TrieEdge*> edgeListCopy;
    for (TrieEdge* edge : curEdge) {
        TrieEdge* copyEdge = new TrieEdge(edge->startNode, edge->endNode, edge->alpha);
        edgeListCopy.push_back(copyEdge);
    }

    // Store the copied edges in the stepsEdge vector for visualization
    stepsEdge.push_back(edgeListCopy);
}

void Trie::mixNodes(TrieNode* target, TrieNode* start, TrieNode* end, float mixCoeff) {
    if (!start && !end) return;

    if (start && end) {
        // Interpolate position
        if (fabs(start->position.x - end->position.x) > 1.0f)
            target->position.x = start->position.x * (1.0f - mixCoeff) + end->position.x * mixCoeff;
        else
            target->position.x = end->position.x;

        if (fabs(start->position.y - end->position.y) > 1.0f)
            target->position.y = start->position.y * (1.0f - mixCoeff) + end->position.y * mixCoeff;
        else
            target->position.y = end->position.y;

        // Interpolate alpha
        target->alpha = start->alpha * (1.0f - mixCoeff) + end->alpha * mixCoeff;

        // Update visiting state
        target->visiting = end->visiting;

        // Update character (assuming the character does not change)
        target->ch = start->ch;

        // Maintain the end-of-word status
        target->isEndOfWord = end->isEndOfWord;
    }
    else if (start) {
        // Fading out effect for deleted nodes
        target->position = start->position;
        if (fabs(mixCoeff - 1.0f) < 0.05f)
            target->alpha = 0;
        else
            target->alpha = start->alpha * (1.0f - mixCoeff);

        target->ch = start->ch;
        target->isEndOfWord = start->isEndOfWord;
    }
    else if (end) {
        // Fading in effect for new nodes
        target->position = end->position;
        target->alpha = end->alpha * mixCoeff;
        target->ch = end->ch;
        target->isEndOfWord = end->isEndOfWord;
    }
}

//void Trie::mixEdge(TrieEdge& target, const TrieEdge* start, const TrieEdge* end, float mixCoeff) {
//    if (!start && !end) return;
//
//    if (start && end) {
//        // Interpolate start position
//        target.startPos.x = start->startPos.x * (1.0f - mixCoeff) + end->startPos.x * mixCoeff;
//        target.startPos.y = start->startPos.y * (1.0f - mixCoeff) + end->startPos.y * mixCoeff;
//
//        // Interpolate end position
//        target.endPos.x = start->endPos.x * (1.0f - mixCoeff) + end->endPos.x * mixCoeff;
//        target.endPos.y = start->endPos.y * (1.0f - mixCoeff) + end->endPos.y * mixCoeff;
//
//        // Interpolate alpha
//        target.alpha = start->alpha * (1.0f - mixCoeff) + end->alpha * mixCoeff;
//    }
//    else if (start) {
//        // Fading out effect for deleted edges
//        target.startPos = start->startPos;
//        target.endPos = start->endPos;
//        if (fabs(mixCoeff - 1.0f) < 0.05f)
//            target.alpha = 0;
//        else
//            target.alpha = start->alpha * (1.0f - mixCoeff);
//    }
//    else if (end) {
//        // Fading in effect for new edges
//        target.startPos = end->startPos;
//        target.endPos = end->endPos;
//        target.alpha = end->alpha * mixCoeff;
//    }
//}


void Trie::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step) {
    if (steps.size() == 0 || stateIndex < 0 || stateIndex >= steps.size() - 1) return; // Ensure stateIndex is within valid range

    elapsedTime += deltaTime;
    float G = elapsedTime / step;
    if (G > step) G = step;

    const std::vector<TrieNode*>& startNodeList = steps[stateIndex];
    const std::vector<TrieNode*>& endNodeList = steps[stateIndex + 1];

    const std::vector<TrieEdge*>& currentEdges = stepsEdge[stateIndex];
    const std::vector<TrieEdge*>& nextEdges = stepsEdge[stateIndex + 1];

    curList.clear();  // Clear current list of mixed nodes
    //curEdge.clear();  // Clear current list of mixed edges

    size_t maxSize = std::max(startNodeList.size(), endNodeList.size());
    curList.resize(maxSize);

    for (size_t i = 0; i < maxSize; ++i) {
        TrieNode* startNode = (i < startNodeList.size()) ? startNodeList[i] : nullptr;
        TrieNode* endNode = (i < endNodeList.size()) ? endNodeList[i] : nullptr;

        TrieNode* mixedNode = new TrieNode(); // Create a new mixed node
        mixNodes(mixedNode, startNode, endNode, G);

        curList[i] = mixedNode;  // Add the mixed node to the current list
    }

    // Mix edges
    size_t maxEdgeSize = std::max(currentEdges.size(), nextEdges.size());
    curEdge.resize(maxEdgeSize);

    for (size_t i = 0; i < maxEdgeSize; ++i) {
        TrieEdge* mixedEdge = (i < nextEdges.size()) ? nextEdges[i] : currentEdges[i];
        if (i < curEdge.size()) {
            mixedEdge->alpha = 1.0f;
            curEdge[i] = mixedEdge;
        }
        else {
            mixedEdge->alpha = G;
            curEdge.push_back(mixedEdge);
        }
    }

    if (G >= step) {
        stateIndex++;  // Move to the next state
        elapsedTime = 0.0f;  // Reset the elapsed time
    }
}

void Trie::draw() {
    // Draw edges first so that they appear below the nodes
    for (auto& edge : curEdge) {
        edge->draw(curList);
    }

    // Draw nodes
    for (auto& node : curList) {
        node->draw();
    }
}

int Trie::getStepsSize() {
    return steps.size();
}

bool Trie::isInteracting(int s) {
    if (steps.size() ==0) return false;
    return s < (steps.size()-1);
}

void Trie::finalDelete(const std::string& word, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdge.clear();
    copyTree();
    remove(word);  
    pause = false;
}

void Trie::finalInsert(const std::string& word, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdge.clear();
    copyTree();
    insert(word);  
    pause = false;
}

void Trie::finalCreate(int count, int& stateIndex, bool& pause) {
    pause = true;
    clearTree();
    stateIndex = 0;
    for (int i = 0; i < count; i++) {
        std::string s = GenerateRandomString(10);  // Generate a random string of up to 10 characters
        insert(s);
    }
    pause = false;
}

void Trie::finalFile(const std::vector<std::string>& input, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;
    
    for (const auto& word : input) {
        insert(word);
    }
    pause = false;
}

bool Trie::finalSearch(const std::string& word, int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;

    steps.clear();
    stepsEdge.clear();
    copyTree();

    TrieNode* current = root;
    std::vector<TrieNode*> visitedNodes;

    for (char ch : word) {
        // If the character is not found, return false
        current->visiting = true;
        copyTree();  // Copy the current state of the Trie for visualization
        visitedNodes.push_back(current);  // Keep track of visited nodes

        // Check if the current character exists in the current node's children
        if (current->children.find(ch) == current->children.end()) {
            // Unvisit all nodes before returning false
            for (TrieNode* node : visitedNodes) {
                node->visiting = false;
            }
            copyTree();
            return false;  // The character was not found, so the word is not in the Trie
        }

        current = current->children[ch];
        visitedNodes.push_back(current);
    }

    // Blink the nodes that form the word
    for (int i = 0; i < 3; ++i) {
        for (auto node : visitedNodes) {
            node->visiting = true;
        }
        copyTree();  

        for (auto node : visitedNodes) {
            node->visiting = false;
        }
        copyTree(); 
    }

    pause = false;
    return true;
}

void Trie::clearTree() {
    for (auto node : curList) {
        if (node) delete node;
        node = NULL;
    }
    /*for (auto node : nodeList) {
        if (node) delete node;
        node = NULL;
    }*/
    for (auto node : curEdge) {
        if (node) delete node;
        node = NULL;
    }
    for (auto ve : steps) {
        for (auto n : ve) {
            if (n) delete n;
            n = NULL;
        }
    }

    steps.clear();
    curEdge.clear();
    curList.clear();
    nodeList.clear();
    steps.clear();
    stepsEdge.clear();
    size = 1;
    root  = new TrieNode(0, { 683,120 }, '1');
    nodeList.push_back(root);
}

std::string GenerateRandomString(int maxLength) {
    std::string characters = "abcdefghijklmnopqrstuvwxyz";
    std::string randomString;

    int length = rand() % maxLength + 1;  // Random length between 1 and maxLength

    for (int i = 0; i < length; ++i) {
        randomString += characters[rand() % characters.length()];
    }

    return randomString;
}




//////////////////////////////////////////////////////////////


Trie testTrie;

void initializeTrie() {
    // Example inputs
    /*std::vector<std::string> words = { "apple", "banana", "grape", "cherry", "mango","bathroom", "grass"};
    for (const std::string& word : words) {
        testTrie.insert(word);
    }*/
   
}

Interact trieCurInteract = REST;
bool trieCurInteracting = false;

float deltaTimeTrie = 0.0f;
float elapsedTimeTrie = 0.0f;
float stepTimeTrie = 1.0f;
int stateIndexTrie = 0;
bool pauseTrie = false;
bool isDraggingTrie = false;
bool doubleSpeedTrie = false;



void renderTrie(Screen& currentScreen) {
    DrawTexture(trieBG, 0, 0, WHITE);
    deltaTimeTrie = GetFrameTime();
    if (checkClick(hashtableOptions[5])) {
        stateIndexTrie = 0;
        pauseTrie = true;
        testTrie.clearTree();
        testTrie = Trie();
    }
    if (checkClick(changeSpeed)) doubleSpeedTrie = !doubleSpeedTrie;
    if (!doubleSpeedTrie) {
        stepTimeTrie = 1.0f;
        DrawTexture(speed1x, changeSpeed.x, changeSpeed.y, WHITE);
    }
    else {
        stepTimeTrie = 0.5f;
        DrawTexture(speed2x, changeSpeed.x, changeSpeed.y, WHITE);
    }

    if (!pauseTrie) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pauseTrie = !pauseTrie;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });

    if (checkClick(backWard)) {
        pauseTrie = true;
        if (stateIndexTrie > 0) {
            stateIndexTrie--;
            elapsedTimeTrie = 0.8f * deltaTimeTrie;
            testTrie.updateState(stateIndexTrie, elapsedTimeTrie, deltaTimeTrie, stepTimeTrie);
        }
    }

    if (checkClick(forWard)) {
        pauseTrie = true;
        if (stateIndexTrie < (testTrie.getStepsSize() - 2)) {
            stateIndexTrie++;
            elapsedTimeTrie = 0.8f * deltaTimeTrie;
            testTrie.updateState(stateIndexTrie, elapsedTimeTrie, deltaTimeTrie, stepTimeTrie);
        }
    }

    if (!pauseTrie) testTrie.updateState(stateIndexTrie, elapsedTimeTrie, deltaTimeTrie, stepTimeTrie);

    // Detect if the mouse is clicking on the slidingButton
    if (checkClick(slidingButton)) {
        isDraggingTrie = true;  // Start dragging when the button is clicked
    }

    // If the mouse button is released, stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDraggingTrie = false;
    }

    // Update the position of the slidingButton if it is being dragged
    if (isDraggingTrie) {
        float mouseX = GetMouseX(); // Get the current mouse X position

        // Constrain the slidingButton within the slider bar's range
        float minX = 800.0f; // Left bound of the slider
        float maxX = 1205.0f; // Right bound of the slider

        slidingButton.x = mouseX;

        // Ensure slidingButton doesn't go out of bounds
        if (slidingButton.x < minX) slidingButton.x = minX;
        if (slidingButton.x > maxX) {
            slidingButton.x = maxX;
            stateIndexTrie = testTrie.getStepsSize() - 1;
        }

        // Update the stateIndex based on the slidingButton's position
        if (testTrie.getStepsSize() > 0) {
            float relativePosition = (slidingButton.x - minX) / (maxX - minX);
            stateIndexTrie = static_cast<int>(relativePosition * (testTrie.getStepsSize() - 1));
            elapsedTimeTrie = 0.8f; // Reset elapsed time when manually adjusting the state
            pauseTrie = true;  // Pause the animation when dragging the slider
            testTrie.updateState(stateIndexTrie, elapsedTimeTrie, deltaTimeTrie, stepTimeTrie);
        }
    }
    else {
        // If not dragging, keep the button in sync with the stateIndex
        if (testTrie.getStepsSize() > 0)
            slidingButton.x = 800.0f + static_cast<float>(stateIndexTrie) / static_cast<float>(testTrie.getStepsSize() - 1) * 405.0f;
        else
            slidingButton.x = 800.0f;
    }

    // Draw the sliding button
    DrawTexture(slidingBut, slidingButton.x, slidingButton.y, WHITE);

    testTrie.draw();

    for (int i = 0; i < 6; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;

    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) {
            if (!trieCurInteracting) {
                trieCurInteract = constants::hashInter[i];
                trieCurInteracting = true;
            }
            else {
                trieCurInteracting = false;
                trieCurInteract = REST;
            }
        }
    }
    TrieInteracting(trieCurInteract);
}

void TrieInteracting(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        TrieCreate(state);
    } break;
    case INSERT:
    {
        TrieInsert(state);
    } break;
    case DELETE:
    {
        TrieDelete(state);
    } break;
    case SEARCH:
    {
        TrieSearch(state);
    } break;
    case FILEINTER:
    {
        TrieFile(state);
    }

    default: break;
    }
}

char trieInputString[11] = "\0"; // 10 characters + null terminator
int trieCharCount = 0;
bool trieInputClick = false;
bool trieRandomClick = false;
float trieTimePassed = 0.0f;
bool trieGetFile = false;


void TrieFile(Interact& state) {
    
        std::string selectedFilePath = FileSelectDialog();
        std::vector<std::string> words = ReadWordsFromFile(selectedFilePath);
        if (words.size() == 0) {
            state = REST;
            return;
        }
        testTrie.clearTree();
        testTrie = Trie();
        testTrie.finalFile(words, stateIndexTrie, pauseTrie);
        state = REST;

}

void TrieInsert(Interact& state) {
    DrawTexture(TrieInsertSection, hashtableOptions[1].x + 90.0f, hashtableOptions[1].y, WHITE);

    if (checkCollision(TrieInsertRandom)) DrawRectangleRec(TrieInsertRandom, Color{ 0, 255, 0, 32 });
    if (checkCollision(TrieInsertOK)) DrawRectangleRec(TrieInsertOK, Color{ 0, 255, 0, 32 });
    if (checkCollision(TrieInsertBox)) {
        DrawRectangleRec(TrieInsertBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (checkClick(TrieInsertBox)) trieInputClick = true;
    else if (!checkClick(TrieInsertBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) trieInputClick = false;

    if (trieInputClick) {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (trieCharCount < 10))
            {
                trieInputString[trieCharCount] = (char)key;
                trieInputString[trieCharCount + 1] = '\0'; // Add null terminator
                trieCharCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            trieCharCount--;
            if (trieCharCount < 0) trieCharCount = 0;
            trieInputString[trieCharCount] = '\0';
        }

        if (fmod(trieTimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)TrieInsertBox.x + 10 + MeasureText(trieInputString, 20), (int)TrieInsertBox.y + 8, 20, DARKGREEN);
        }
    }

    trieTimePassed = GetTime();

    DrawText(trieInputString, (int)TrieInsertBox.x + 7, (int)TrieInsertBox.y + 4, 20, DARKGREEN);

    if ((checkClick(TrieInsertOK) || IsKeyPressed(KEY_ENTER)) && !testTrie.isInteracting(stateIndexTrie)) {
        if (trieCharCount > 0) {
            std::string input = std::string(trieInputString);
            testTrie.finalInsert(input, stateIndexTrie, pauseTrie);
            while (trieCharCount > 0) {
                trieCharCount--;
                trieInputString[trieCharCount] = '\0';
            }
        }
    }

    if (checkClick(TrieInsertRandom)) {
        std::string str = GenerateRandomString(10); // Generate random string of up to 10 characters
        for (int i = 0; i < str.size(); i++) {
            trieInputString[i] = str[i];
        }
        trieCharCount = str.size();
        trieInputString[trieCharCount] = '\0';
    }
}


void TrieDelete(Interact& state) {
    DrawTexture(TrieDeleteSection, hashtableOptions[2].x + 90.0f, hashtableOptions[2].y, WHITE);

    if (checkCollision(TrieDeleteOK)) DrawRectangleRec(okDelete, Color{ 0, 255, 0, 32 });
    if (checkCollision(TrieDeleteBox)) {
        DrawRectangleRec(TrieDeleteBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (checkClick(TrieDeleteBox)) trieInputClick = true;
    else if (!checkClick(TrieDeleteBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) trieInputClick = false;

    if (trieInputClick) {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (trieCharCount < 10))
            {
                trieInputString[trieCharCount] = (char)key;
                trieInputString[trieCharCount + 1] = '\0';
                trieCharCount++;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            trieCharCount--;
            if (trieCharCount < 0) trieCharCount = 0;
            trieInputString[trieCharCount] = '\0';
        }

        if (fmod(trieTimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)TrieDeleteBox.x + 10 + MeasureText(trieInputString, 20), (int)TrieDeleteBox.y + 8, 20, DARKGREEN);
        }
    }

    trieTimePassed = GetTime();

    DrawText(trieInputString, (int)TrieDeleteBox.x + 7, (int)TrieDeleteBox.y + 4, 20, DARKGREEN);

    if ((checkClick(TrieDeleteOK) || IsKeyPressed(KEY_ENTER)) && !testTrie.isInteracting(stateIndexTrie)) {
        if (trieCharCount > 0) {
            std::string input = std::string(trieInputString);
            testTrie.finalDelete(input, stateIndexTrie, pauseTrie);
            while (trieCharCount > 0) {
                trieCharCount--;
                trieInputString[trieCharCount] = '\0';
            }
        }
    }
}


void TrieSearch(Interact& state) {
    DrawTexture(TrieDeleteSection, hashtableOptions[3].x + 90.0f, hashtableOptions[3].y, WHITE);

    if (checkCollision(TrieSearchOk)) DrawRectangleRec(TrieSearchOk, Color{ 0, 255, 0, 32 });
    if (checkCollision(TrieSearchBox)) {
        DrawRectangleRec(TrieSearchBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (checkClick(TrieSearchBox)) trieInputClick = true;
    else if (!checkClick(TrieSearchBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) trieInputClick = false;

    if (trieInputClick) {
        int key = GetCharPressed();

        while (key > 0)
        {
            if ((key >= 32) && (key <= 125) && (trieCharCount < 10))
            {
                trieInputString[trieCharCount] = (char)key;
                trieInputString[trieCharCount + 1] = '\0';
                trieCharCount++;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            trieCharCount--;
            if (trieCharCount < 0) trieCharCount = 0;
            trieInputString[trieCharCount] = '\0';
        }

        if (fmod(trieTimePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)TrieSearchBox.x + 10 + MeasureText(trieInputString, 20), (int)TrieSearchBox.y + 8, 20, DARKGREEN);
        }
    }

    trieTimePassed = GetTime();

    DrawText(trieInputString, (int)TrieSearchBox.x + 7, (int)TrieSearchBox.y + 4, 20, DARKGREEN);

    if ((checkClick(TrieSearchOk) || IsKeyPressed(KEY_ENTER)) && !testTrie.isInteracting(stateIndexTrie)) {
        if (trieCharCount > 0) {
            std::string input = std::string(trieInputString);
            testTrie.finalSearch(input, stateIndexTrie, pauseTrie);
            while (trieCharCount > 0) {
                trieCharCount--;
                trieInputString[trieCharCount] = '\0';
            }
        }
    }
}


int trieCreateAmount = 0;  // Global variable to store the number of strings to generate
char trieCreateInputNumber[4] = "\0";  // For storing the number input as a string
int trieCreateNumCount = 0;

void TrieCreate(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[0].x + 90.0f, hashtableOptions[0].y, WHITE);

    if (checkCollision(randomCreate)) DrawRectangleRec(randomCreate, Color{ 0, 255, 0, 32 });
    if (checkCollision(ok1random)) DrawRectangleRec(ok1random, Color{ 0, 255, 0, 32 });
    if (checkCollision(random1Section)) {
        DrawRectangleRec(random1Section, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (checkClick(random1Section)) trieInputClick = true;
    else if (!checkClick(random1Section) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) trieInputClick = false;

    if (trieInputClick) {
        int key = GetCharPressed();

        while (key > 0) {
            if ((key >= 48) && (key <= 57) && (trieCreateNumCount < 3))  // Accept only numbers
            {
                trieCreateInputNumber[trieCreateNumCount] = (char)key;
                trieCreateInputNumber[trieCreateNumCount + 1] = '\0';
                trieCreateNumCount++;
            }

            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            trieCreateNumCount--;
            if (trieCreateNumCount < 0) trieCreateNumCount = 0;
            trieCreateInputNumber[trieCreateNumCount] = '\0';
        }

        if (fmod(trieTimePassed, 1.0f) < 0.5f) {
            DrawText("_", (int)random1Section.x + 10 + MeasureText(trieCreateInputNumber, 20), (int)random1Section.y + 8, 20, DARKGREEN);
        }
    }

    trieTimePassed = GetTime();

    DrawText(trieCreateInputNumber, (int)random1Section.x + 7, (int)random1Section.y + 4, 20, DARKGREEN);

    if (trieCreateNumCount > 0) {
        trieCreateAmount = std::stoi(trieCreateInputNumber);
    }

    if ((checkClick(ok1random) || IsKeyPressed(KEY_ENTER)) && !testTrie.isInteracting(stateIndexTrie)) {
        if (trieCreateAmount > 0) {
            testTrie.clearTree();
            testTrie = Trie();
            testTrie.finalCreate(trieCreateAmount, stateIndexTrie, pauseTrie);
            trieCreateAmount = 0;
            trieCreateNumCount = 0;
            trieCreateInputNumber[0] = '\0';
        }
    }

    if (checkClick(randomCreate) && !testTrie.isInteracting(stateIndexTrie)) {
        trieCreateAmount = rand() % 100 + 1;  // Generate a random number of strings to insert
        std::string str = std::to_string(trieCreateAmount);
        for (int i = 0; i < str.size(); i++) {
            trieCreateInputNumber[i] = str[i];
        }
        trieCreateNumCount = str.size();
        trieCreateInputNumber[trieCreateNumCount] = '\0';
    }
}

