#include "minheap.h"

void MinHeapNode::draw() {
    Color gradientStart = ColorAlpha(YELLOW, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);

    if (visiting) {
        outlineColor = ColorAlpha(BLUE, alpha);
    }


    if (value == 0) return;
    if (value == -1) return;

    DrawCircleV(position, radius, outlineColor);

    DrawCircleGradient(position.x, position.y, radius - 5.0f, WHITE, gradientEnd);
    // Draw ring outline

    // Calculate positions for the value and balance text
    int valueFontSize = 20; // Adjust font size as needed

    // Draw value text at the center of the node
    std::string valueStr = std::to_string(value);
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);

}

void MinHeap::insert(int value) {
    copyHeap();
    // Create a new node and add it to nodeList
    MinHeapNode* newNode = new MinHeapNode(value);
    newNode->ind = nodeList.size();
    nodeList.push_back(newNode);

    // Add the new node to the heapArray
    heapArray.push_back(newNode);

    // Calculate its initial position (before heapifying)
    newNode->position = calculateNodePosition(heapArray.size() - 1);

    //Add edge
    if (heapArray.size() > 1) {
        int parentIndex = (heapArray.size() - 2) / 2;
        addEdge(parentIndex, heapArray.size() - 1);
    }

    copyHeap();

    // Bubble up to maintain the heap property
    bubbleUp(heapArray.size() - 1);

    copyHeap();

    //printEdges();
}

void MinHeap::bubbleUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heapArray[index]->value < heapArray[parentIndex]->value) {
            copyHeap();
            heapArray[index]->visiting = true;
            heapArray[parentIndex]->visiting = true;
            std::swap(heapArray[index], heapArray[parentIndex]);


            copyHeap2();
            heapArray[index]->visiting = false;
            heapArray[parentIndex]->visiting = false;
            index = parentIndex;

        }
        else {
            break;
        }
    }
}

Vector2 MinHeap::calculateNodePosition(int index) {
    int level = std::floor(std::log2(index + 1));
    int posInLevel = index - (1 << level) + 1;
    float xSpacing = constants::scene_width / (1 << (level + 1));
    float x = xSpacing * (posInLevel * 2 + 1);
    float y = 100.0f + level * 80.0f;
    return Vector2{ x, y };
}


void MinHeap::addEdge(int parentIndex, int childIndex) {
    Vector2 startPos = heapArray[parentIndex]->position;
    Vector2 endPos = heapArray[childIndex]->position;
    EdgeHeap* newEdge = new EdgeHeap(startPos, endPos);
    edges.push_back(newEdge);
}


void MinHeap::updateEdges() {
    // Clear existing edges to avoid redundancy
    edges.clear();

    // Recalculate edges based on the current heapArray
    for (size_t i = 0; i < heapArray.size(); ++i) {
        int leftChildIndex = 2 * i + 1;
        int rightChildIndex = 2 * i + 2;

        // Add an edge from the current node to its left child (if it exists)
        if (leftChildIndex < heapArray.size()) {
            //edges.emplace_back(i, leftChildIndex);
            addEdge(i, leftChildIndex);
        }

        // Add an edge from the current node to its right child (if it exists)
        if (rightChildIndex < heapArray.size()) {
            //edges.emplace_back(i, rightChildIndex);
            addEdge(i, rightChildIndex);
        }
    }
}

void MinHeap::deleteElement(int value) {
    // Find the index of the element to delete
    int indexToDelete = -1;
    int listIndex= -1;
    for (int i = 0; i < heapArray.size(); ++i) {
        if (heapArray[i]->value == value) {
            indexToDelete = i;
            break;
        }
    }


    if (indexToDelete == -1) {
        std::cout << "Value not found in the heap." << std::endl;
        return;
    }

    copyHeap();
    heapArray[indexToDelete]->visiting = true;
    copyHeap();
    heapArray[indexToDelete]->visiting = false;
    std::swap(heapArray[indexToDelete], heapArray.back());
    MinHeapNode* temp = heapArray.back();

    copyHeap();

    heapArray.pop_back();
    temp->alpha = 0.0f;
    copyHeap();
    delete temp;
    copyHeap();


    // Restore the heap property by heapifying down or bubbling up
    if (indexToDelete < heapArray.size()) {
        int parentIndex = (indexToDelete - 1) / 2;
        if (heapArray[indexToDelete]->value < heapArray[parentIndex]->value) {
            bubbleUp(indexToDelete);
            copyHeap();
        }
        else {
            heapifyDown(indexToDelete);
            copyHeap();
        }
    }

}

// Function to heapify down from the given index
void MinHeap::heapifyDown(int index) {
    int size = heapArray.size();
    int smallest = index;
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;

    // Find the smallest among root, left child, and right child
    if (leftChildIndex < size && heapArray[leftChildIndex]->value < heapArray[smallest]->value) {
        smallest = leftChildIndex;
    }
    if (rightChildIndex < size && heapArray[rightChildIndex]->value < heapArray[smallest]->value) {
        smallest = rightChildIndex;
    }

    // Swap and continue heapifying down if the smallest is not the current index
    if (smallest != index) {
        std::swap(heapArray[index], heapArray[smallest]);

        // Recalculate positions after swapping
        heapArray[index]->position = calculateNodePosition(index);
        heapArray[smallest]->position = calculateNodePosition(smallest);
        copyHeap();
        heapifyDown(smallest);
    }
}



void MinHeap::updatePositions() {
    for (size_t i = 0; i < heapArray.size(); ++i) {
        heapArray[i]->position = calculateNodePosition(i);
    }
}

void MinHeap::draw() {
    for (auto& edge : curEdges) {
        edge->draw();
    }

    for (auto& node : curList) {
        node->draw();
    }
}


void MinHeap::printHeapArray() {
    std::cout << "Heap Array: ";
    for (const auto& node : heapArray) {
        std::cout << node->value << " ";
    }
    std::cout << std::endl;
}
void MinHeap::debug() {
    //printEdges();
    printHeapArray();
}

void MinHeap::copyHeap() {
    updatePositions();
    updateEdges();
    std::vector<MinHeapNode*> tempNodes;
    std::vector<EdgeHeap*> tempEdges;

    for (auto& node : nodeList) {
        MinHeapNode* tempN = new MinHeapNode;
        if (node) {
            tempN->value = node->value;
            tempN->position = node->position;
            tempN->visiting = node->visiting;
            tempN->alpha = node->alpha;
            tempNodes.push_back(tempN);
        }
        else tempNodes.push_back(nullptr);
    }

    for (auto& e : edges) {
        EdgeHeap* newE = new EdgeHeap;
        if (e) {
            newE->startPos = e->startPos;
            newE->endPos = e->endPos;
            tempEdges.push_back(newE);
        }
    }

    steps.push_back(tempNodes);
    stepsEdge.push_back(tempEdges);
}

void MinHeap::copyHeap2() {

    std::vector<MinHeapNode*> tempNodes;
    std::vector<EdgeHeap*> tempEdges;

    for (auto& node : nodeList) {
        MinHeapNode* tempN = new MinHeapNode;
        if (node) {
            tempN->value = node->value;
            tempN->position = node->position;
            tempN->visiting = node->visiting;
            tempN->alpha = node->alpha;
            tempNodes.push_back(tempN);
        }
        else tempNodes.push_back(nullptr);
    }

    for (auto& e : edges) {
        EdgeHeap* newE = new EdgeHeap;
        if (e) {
            newE->startPos = e->startPos;
            newE->endPos = e->endPos;
            tempEdges.push_back(newE);
        }
    }

    steps.push_back(tempNodes);
    stepsEdge.push_back(tempEdges);
}

void MinHeap::mixedNode(MinHeapNode* target, MinHeapNode* start, MinHeapNode* end, float mixCoeff) {
    if (!start && !end) return;

    if (start && end) {
        // Interpolate position
        if (fabs(start->position.x - end->position.x) > 1.0f) target->position.x = start->position.x * (1.0f - mixCoeff) + end->position.x * mixCoeff;
        else  target->position.x = end->position.x;

        if (fabs(start->position.y - end->position.y) > 1.0f) target->position.y = start->position.y * (1.0f - mixCoeff) + end->position.y * mixCoeff;
        else  target->position.y = end->position.y;
        target->alpha = start->alpha * (1.0f - mixCoeff) + end->alpha * mixCoeff;
        
        target->visiting = end->visiting;
        target->value = start->value;
    }
    else if (start) {
        target->position = start->position;
        if (fabs(mixCoeff - 1.0f) < 0.05f) target->alpha = 0;
        else target->alpha = start->alpha * (1.0f - mixCoeff);

        target->value = start->value;
    }
    else if (end) {
        target->position = end->position;
        target->alpha = end->alpha * mixCoeff;
        target->value = end->value;
    }
}

void MinHeap::updateState(int& stateIndex, float& elapsedTime, float deltaTime, float step) {
    if (stateIndex < 0 || stateIndex >= steps.size() - 1) return; // Ensure stateIndex is within valid range
    elapsedTime += deltaTime;
    float G = elapsedTime / step;  // Assuming transition over 1 second
    if (G > step) G = step;

    const std::vector<MinHeapNode*>& startCombList = steps[stateIndex];
    const std::vector<MinHeapNode*>& endCombList = steps[stateIndex + 1];

    const std::vector<EdgeHeap*>& currentEdges = stepsEdge[stateIndex];
    const std::vector<EdgeHeap*>& nextEdges = stepsEdge[stateIndex + 1];


    curList.clear();  // Clear current list of mixed combs
    curEdges.clear(); // Clear current list of mixed edges

    size_t maxSize = std::max(startCombList.size(), endCombList.size());
    curList.resize(maxSize);


    for (size_t i = 0; i < maxSize; ++i) {
        MinHeapNode* startComb = (i < startCombList.size()) ? startCombList[i] : nullptr;
        MinHeapNode* endComb = (i < endCombList.size()) ? endCombList[i] : nullptr;

        MinHeapNode* newmixedComb = new MinHeapNode; // Create a new mixed comb node
        mixedNode(newmixedComb, startComb, endComb, G);

        curList[i] = newmixedComb;  // Add the mixed comb to the current list
    }

    // Mix edges
    size_t maxEdgeSize = std::max(currentEdges.size(), nextEdges.size());
    curEdges.resize(maxEdgeSize);

    for (size_t i = 0; i < maxEdgeSize; ++i) {
        EdgeHeap* mixedEdge = (i < nextEdges.size()) ? nextEdges[i] : currentEdges[i];
        if (i < curEdges.size()) {
            mixedEdge->alpha = 1.0f;
            curEdges[i] = mixedEdge;
        }
        else {
            if (fabs(G - 1.0f) < 0.05f) mixedEdge->alpha = 0;
            else mixedEdge->alpha = 1.0f * (1.0f - G);
        }
        curEdges[i] = mixedEdge;
    }
    if (G >= step) {
        stateIndex++;  // Move to the next state
        elapsedTime = 0.0f;  // Reset the elapsed time
    }
}

int MinHeap::getStepsSize() {
    return steps.size();
}


/////////////////////////////////////

MinHeap minHeap;
float deltaTimeHeap = 0.0f;
float elapsedTimeHeap = 0.0f;
float stepHeap = 1.0f;
int stateIndexHeap = 0;
bool pauseHeap = false;
bool isDraggingHeap = false;
bool heapCurInteracting = false;
Interact heapCurInteract = REST;

void initializeHeap() {
    for (int i = 10; i >= 1; i--) minHeap.insert(i);

}


void renderHeap(Screen& currentScreen) {
    DrawTexture(avlBG, 0, 0, WHITE);
    deltaTimeHeap = GetFrameTime();

    if (!pauseHeap) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pauseHeap = !pauseHeap;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });
    if (checkCollision(forWard)) DrawRectangleRec(forWard, Color{ 0, 255, 0, 32 });
    if (checkCollision(backWard)) DrawRectangleRec(backWard, Color{ 0, 255, 0, 32 });

    if (checkClick(backWard)) {
        pauseHeap = true;
        if (stateIndexHeap > 0) {
            stateIndexHeap--;
            elapsedTimeHeap = 0.8f*deltaTimeHeap;
            minHeap.updateState(stateIndexHeap, elapsedTimeHeap, deltaTimeHeap, stepHeap);
        }
    }

    if (checkClick(forWard)) {
        pauseHeap = true;
        if (stateIndexHeap < (minHeap.getStepsSize() - 2)) {
            stateIndexHeap++;
            elapsedTimeHeap = 0.8f * deltaTimeHeap;
            minHeap.updateState(stateIndexHeap, elapsedTimeHeap, deltaTimeHeap, stepHeap);
        }
    }


    if (!pauseHeap) minHeap.updateState(stateIndexHeap, elapsedTimeHeap, deltaTimeHeap, stepHeap);


    // Detect if the mouse is clicking on the slidingButton
    if (checkClick(slidingButton)) {
        isDraggingHeap = true;  // Start dragging when the button is clicked
    }

    // If the mouse button is released, stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDraggingHeap = false;
    }

    // Update the position of the slidingButton if it is being dragged
    if (isDraggingHeap) {
        float mouseX = GetMouseX(); // Get the current mouse X position

        // Constrain the slidingButton within the slider bar's range
        float minX = 800.0f; // Left bound of the slider
        float maxX = 1205.0f; // Right bound of the slider

        slidingButton.x = mouseX;

        // Ensure slidingButton doesn't go out of bounds
        if (slidingButton.x < minX) slidingButton.x = minX;
        if (slidingButton.x > maxX) slidingButton.x = maxX;

        // Update the stateIndex based on the slidingButton's position
        if (minHeap.getStepsSize() > 0) {
            float relativePosition = (slidingButton.x - minX) / (maxX - minX);
            stateIndexHeap = static_cast<int>(relativePosition * (minHeap.getStepsSize() - 1));
            elapsedTimeHeap = 0.8f; // Reset elapsed time when manually adjusting the state
            pauseHeap = true;  // Pause the animation when dragging the slider
            minHeap.updateState(stateIndexHeap, elapsedTimeHeap, deltaTimeHeap, stepHeap);
        }
    }
    else {
        // If not dragging, keep the button in sync with the stateIndex
        if (minHeap.getStepsSize() > 0)
            slidingButton.x = 800.0f + static_cast<float>(stateIndexHeap) / static_cast<float>(minHeap.getStepsSize() - 1) * 405.0f;
        else
            slidingButton.x = 800.0f;
    }

    
    DrawTexture(slidingBut, slidingButton.x, slidingButton.y, WHITE);

    minHeap.draw();


    for (int i = 0; i < 4; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;
    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) {
            if (!heapCurInteracting) {
                heapCurInteract = constants::hashInter[i];
                heapCurInteracting = true;
            }
            else {
                heapCurInteracting = false;
                heapCurInteract = REST;
            }
        }
    }
    tree234Interacting(heapCurInteract);
}

void HeapInteracting(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        HeapCreate(state);
    } break;
    case INSERT:
    {
        HeapInsert(state);
    } break;
    case DELETE:
    {
        HeapDelete(state);
    } break;
    case SEARCH:
    {
        HeapSearch(state);
    } break;
    case FILEINTER:
    {
        HeapFile(state);
    }

    default: break;
    }
};

char HeapinputNumber[4] = "\0";
int HeapnumCount = 0;
bool HeapinputClick = false;
bool HeaprandomClick = false;
float HeaptimePassed = 0.0f;
bool HeapgetFile = false;

