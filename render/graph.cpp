#include "graph.h"

void GraphNode::draw() {
    if (value == -1) return;

    // Determine the color with fading effect using the alpha value
    //Color gradientStart = ColorAlpha(WHITE, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);

    if (colour > 0) {
        Color temp = graphColour[colour%7];
        outlineColor = ColorAlpha(temp, alpha);
        gradientEnd = ColorAlpha(temp, alpha);
        //outlineColor = ColorAlpha(temp, alpha);
    }

    DrawCircleV(position, radius, outlineColor);

    DrawCircleGradient(position.x, position.y, radius - 5.0f, WHITE, gradientEnd);
    // Draw ring outline

    // Calculate positions for the value and balance text
    int valueFontSize = 20; // Adjust font size as needed
    int balanceFontSize = 15; // Adjust font size as needed

    // Draw value text at the center of the node
    std::string valueStr = std::to_string(ind);
    Vector2 valuePos = { position.x - MeasureText(valueStr.c_str(), valueFontSize) / 2,
                         position.y - valueFontSize / 2 };
    DrawText(valueStr.c_str(), valuePos.x, valuePos.y, valueFontSize, textColor);

    std::string balanceStr = "val: " + std::to_string(value);
    Vector2 balancePos = { position.x - MeasureText(balanceStr.c_str(), balanceFontSize) / 2,
                          position.y - radius
        - balanceFontSize };
    DrawText(balanceStr.c_str(), balancePos.x, balancePos.y, balanceFontSize, textColor);
}

void GraphNode::update(const Vector2& mousePosition, bool isMouseDown, bool isMouseUp, const std::vector<GraphNode*>& nodeList) {
    if (isMouseDown) {
        float distance = Vector2Distance(mousePosition, position);
        if (distance <= radius-5.0f) {
            isDragging = true;
        }
    }

    if (isDragging) {
        bool canMove = true;
        Vector2 newPosition = mousePosition;

        // Check if the new position would cause an overlap
        for (const auto& otherNode : nodeList) {
            if (otherNode != this) {
                float distanceToOther = Vector2Distance(newPosition, otherNode->position);
                if (distanceToOther < radius + otherNode->radius) {
                    canMove = false;
                    break;
                }
            }
        }

        if (canMove) {
            position = newPosition;
        }
    }

    if (isMouseUp) {
        isDragging = false;
    }
}



void GraphEdge::draw(std::vector<GraphNode*>& nodeList) {
    Vector2 start = nodeList[startIndex]->position;
    Vector2 end = nodeList[endIndex]->position;
    if (start.x == 0 || end.x == 0) return;
    Color edgeColor = BLACK;

    if (colour > 0) {
        Color temp = graphColour[colour%7];

        edgeColor = ColorAlpha(temp, alpha);
    }

    DrawLineEx(start, end, 2.0f, edgeColor);

    // Draw weight of the edge in the middle of the edge line
    Vector2 midPoint = { (start.x + end.x) / 2, (start.y + end.y) / 2 };
    DrawCircleGradient(midPoint.x, midPoint.y, 10.0f, WHITE, WHITE);
    std::string weightStr = std::to_string(weight);
    DrawText(weightStr.c_str(), midPoint.x - MeasureText(weightStr.c_str(), 15) / 2, midPoint.y - 15 / 2, 15, GOLD);
}


void Graph::addNode(int val) {
    Vector2 position;
    do {
        position = generateRandomPosition();
    } while (doesOverlap(position, nodeList));

	GraphNode* n = new GraphNode(V+1, val, position);
	nodeList.push_back(n);
    // Create new rows with zero-initialized values
    std::vector<int> newN(V + 1, 0); // New row for Mat
    std::vector<int> newC(V + 1, 0); // New row for ColourMat

    // Add the new row to Mat and ColourMat
    Mat.push_back(newN);
    ColourMat.push_back(newC);

    // Update existing rows in Mat and ColourMat to add the new column
    for (int i = 0; i < V; i++) {
        Mat[i].push_back(0);
        ColourMat[i].push_back(0);
    }

    // Increment the number of vertices
    V++;
    //std::cout << Mat.size() << " " << Mat[0].size() << std::endl;
}

Vector2 generateRandomPosition() {
    float x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
    float y = minY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxY - minY)));
    return Vector2{ x, y };
}

bool Graph::doesOverlap(const Vector2& pos, const std::vector<GraphNode*>& nodeList) {
    for (const auto& node : nodeList) {
        float distance = sqrt(pow(pos.x - node->position.x, 2) + pow(pos.y - node->position.y, 2));
        if (distance < minDistance) {
            return true;
        }
    }
    return false;
}


void Graph::addEdge(int u, int v, int weight) {
	if (u >= V || v >= V) return;

	Mat[u][v] = weight;

	Mat[v][u] = weight;

    //std::cout << "Add edge: " << Mat[u][v] << std::endl;

}


void Graph::dfsConnected(std::vector<bool>& visited, int u) {
    visited[u] = true;
    for (int i = 0; i < V; i++) {
        if (Mat[u][i] && !visited[i]) {
            dfsConnected( visited, i);
        }
    }
}

bool Graph::isConnected() {
    std::vector<bool> visited;
    for (int i = 0; i < V; i++) {
        visited[i] = false;
    }

    dfsConnected(visited, 0);

    for (int i = 0; i < V; i++) {
        if (visited[i] == false) {
            return false; 
        }
    }
    return true;

}

void Graph::initializeFromMatrix(const std::vector<std::vector<int>>& adjacencyMatrix) {
    clearGraph(); // Clear any existing graph data

    int matrixSize = adjacencyMatrix.size();

    // Add nodes
    for (int i = 0; i < matrixSize; ++i) {
        addNode(); // Adds a node with a default value
    }

    // Add edges based on the adjacency matrix
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            if (adjacencyMatrix[i][j] > 0) {
                addEdge(i, j, adjacencyMatrix[i][j]); // Adds an edge with the given weight
            }
        }
    }

    // Update the edges to reflect the new graph structure
    updateEdges();
}



int Graph::minKey(std::vector<int>& key, std::vector<bool>& mstSet) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

void Graph::prim() {
    std::vector<int> key(V), parent(V); std::vector<bool> mstSet(V);

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V; count++) {
        int u = minKey(key, mstSet);
        if (u == -1) continue;
        mstSet[u] = true;
        nodeList[u]->colour = 1;
        if (parent[u] >= 0) {
            ColourMat[parent[u]][u] = 1;
            ColourMat[u][parent[u]] = 1;
        }
        copyList();
        for (int v = 0; v < V; v++) {
            if (Mat[u][v] && !mstSet[v] && Mat[u][v] < key[v]) {
                parent[v] = u;
                key[v] = Mat[u][v];
            }
        }
    }

    copyList();
    /*printf("Edges in MST:\n");
    for (int i = 1; i < V; i++) {
        printf("%d - %d: %d\n", parent[i], i, Mat[i][parent[i]]);
    }*/
}

void Graph::clearColour() {
    for (auto node : nodeList) node->colour = 0;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            ColourMat[i][j] = 0;
        }
    }
}

int Graph::connectedComp() {
    copyList();
    std::vector<bool> visited(V, false);
    int componentIndex = 0;

    for (int u = 0; u < V; ++u) {
        if (!visited[u]) {
            componentIndex++;
            dfs(u, componentIndex, visited);
        }
    }
    copyList();
    return componentIndex; 
}

void Graph::dfs(int u, int componentIndex, std::vector<bool>& visited) {
    visited[u] = true;
    nodeList[u]->colour = componentIndex;
    copyList();

    for (int v = 0; v < V; ++v) {
        if (Mat[u][v] > 0 && !visited[v]) {
            ColourMat[u][v] = componentIndex;
            ColourMat[v][u] = componentIndex;
            dfs(v, componentIndex, visited);
        }
    }
}

void Graph::updateEdges() {
    edgeList.clear(); 
    for (int u = 0; u < V; ++u) {
        for (int v = u + 1; v < V; ++v) {
            if (Mat[u][v] > 0) {
                GraphEdge* edge = new GraphEdge(u, v, Mat[u][v], ColourMat[u][v]);
                edgeList.push_back(edge);
            }
        }
    }
    //std::cout << "Update edges: " << edgeList.size() << std::endl;

}

void Graph::clearGraph() {
    // Delete all dynamically allocated nodes
    for (auto node : nodeList) {
        delete node;
    }
    nodeList.clear(); // Clear the list of nodes

    // Delete all dynamically allocated edges
    for (auto edge : edgeList) {
        delete edge;
    }
    edgeList.clear(); // Clear the list of edges

    // Clear the adjacency matrices
    Mat.clear();
    ColourMat.clear();

    // Clear other data structures
    curList.clear();
    steps.clear();
    curEdge.clear();
    stepsEdge.clear();

    // Reset the number of vertices
    V = 0;
}


void Graph::copyList() {


    curList.clear();
    curEdge.clear();

    updateEdges();



    // Copy nodes
    for (auto& node : nodeList) {
        GraphNode* newNode = new GraphNode(node->ind, node->value);
        newNode->colour = node->colour;
        newNode->visiting = node->visiting;
        newNode->position = node->position;

        newNode->isDragging = node->isDragging;
        newNode->alpha = node->alpha;
        curList.push_back(newNode);
    }

    // Copy edges
    //std::cout << "Edge size: " << edgeList.size() << std::endl;

    for (auto& edge : edgeList) {
        GraphEdge* newEdge = new GraphEdge(edge->startIndex, edge->endIndex, edge->weight, edge->colour);
        //std::cout << edge->startIndex << " " << edge->endIndex << std::endl;
        newEdge->colour = edge->colour;
        curEdge.push_back(newEdge);
    }

    // Save the current list and edges into steps for potential animation or undo/redo
    steps.push_back(curList);
    stepsEdge.push_back(curEdge);
}

int Graph::getStepsSize() {
    return steps.size();
}

void Graph::updateState(float deltaTime, float& elapsedTime, int& stateIndex, float step) {
    if (stateIndex < 0 || stateIndex >= steps.size() - 1) return;

    if (steps.size() == 0) return;

    elapsedTime += deltaTime;
    float G = elapsedTime / step;
    if (G > 1.0f) G = 1.0f;

    const std::vector<GraphNode*>& startList = steps[stateIndex];
    const std::vector<GraphNode*>& endList = steps[stateIndex + 1];

    const std::vector<GraphEdge*>& startEdges = stepsEdge[stateIndex];
    const std::vector<GraphEdge*>& endEdges = stepsEdge[stateIndex + 1];

    curList.clear();
    curEdge.clear();

    // Interpolate node positions
    for (size_t i = 0; i < startList.size(); ++i) {
        Vector2 mixedPosition = {
            nodeList[i]->position.x,
            nodeList[i]->position.y
        };
        GraphNode* mixedNode = new GraphNode(startList[i]->ind,startList[i]->value, mixedPosition);
        mixedNode->colour = startList[i]->colour;
        mixedNode->visiting = startList[i]->visiting;
        mixedNode->alpha = startList[i]->alpha;

        curList.push_back(mixedNode);
    }

    // Interpolate edge positions and colors
    for (size_t i = 0; i < startEdges.size(); ++i) {
        

        GraphEdge* mixedEdge = new GraphEdge(
            startEdges[i]->startIndex,  // Since indices don't change, use the same index
            startEdges[i]->endIndex,
            startEdges[i]->weight,
            startEdges[i]->colour
        );
        mixedEdge->colour = startEdges[i]->colour;
        curEdge.push_back(mixedEdge);
    }

    if (G >= 1.0f) {
        stateIndex++;
        elapsedTime = 0.0f;
    }
}


void Graph::draw() {
  
    // Draw edges first so they appear below the nodes
    for (const auto& edge : curEdge) {
        edge->draw(curList);
    }

    // Draw nodes
    for (const auto& node : curList) {
        node->draw();
    }

}

void Graph::applyForceDirectedLayout(int iterations, float areaWidth, float areaHeight ) {
    copyList();
    float k = std::sqrt((areaWidth * areaHeight) / V);  // Optimal distance between nodes
    float repulsiveForceMultiplier = 0.1f;
    float attractiveForceMultiplier = 0.5f;
    float maxDisplacement = 50.0f;

    for (int iter = 0; iter < iterations; iter++) {
        std::vector<Vector2> displacement(V, Vector2{ 0.0f, 0.0f });

        // Repulsive forces between all pairs of nodes
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (i != j) {
                    Vector2 delta = subtractVector2(nodeList[i]->position, nodeList[j]->position);
                    float distance = magnitudeVector2(delta);
                    if (distance > 0) {
                        Vector2 repulsiveForce = multiplyVector2(normalizeVector2(delta), (k * k / distance));
                        displacement[i] = addVector2(displacement[i], multiplyVector2(repulsiveForce, repulsiveForceMultiplier));
                    }
                }
            }
        }

        // Attractive forces between connected nodes
        for (const auto& edge : edgeList) {
            Vector2 delta = subtractVector2(nodeList[edge->startIndex]->position, nodeList[edge->endIndex]->position);
            float distance = magnitudeVector2(delta);
            Vector2 attractiveForce = multiplyVector2(normalizeVector2(delta), (distance * distance / k));
            displacement[edge->startIndex] = subtractVector2(displacement[edge->startIndex], multiplyVector2(attractiveForce, attractiveForceMultiplier));
            displacement[edge->endIndex] = addVector2(displacement[edge->endIndex], multiplyVector2(attractiveForce, attractiveForceMultiplier));
        }

        // Apply displacement to node positions
        for (int i = 0; i < V; i++) {
            float displacementLength = magnitudeVector2(displacement[i]);
            if (displacementLength > 0) {
                nodeList[i]->position = addVector2(nodeList[i]->position, multiplyVector2(normalizeVector2(displacement[i]), std::min(displacementLength, maxDisplacement)));

                // Ensure nodes stay within the defined area
                nodeList[i]->position.x = std::min(areaWidth - nodeRadius, std::max(nodeRadius, nodeList[i]->position.x));
                nodeList[i]->position.y = std::min(areaHeight - nodeRadius, std::max(nodeRadius, nodeList[i]->position.y));
            }
        }
    }

    // After the layout is applied, update the edges to reflect new node positions
    copyList();
}

void Graph::dragNodes() {
    Vector2 mousePosition = GetMousePosition();
    bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool isMouseUp = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    bool updateNL = false;
    bool updateCL = false;

    for (auto node : nodeList) {
        if (node) {
            node->update(mousePosition, isMouseDown, isMouseUp, nodeList);
           
        }
    }

    for (auto node : curList) {
        if (node ) {
            node->update(mousePosition, isMouseDown, isMouseUp, curList);
            
        }
    }


}

bool Graph::isInteracting(int s) {
    if (steps.size() == 0) return false;
    else return (s < steps.size() - 1);
}

void Graph::finalMSTPrim(int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;
    curList.clear();
    curEdge.clear();
    steps.clear();
    stepsEdge.clear();
    prim();
    copyList();
    pause = false;
}

int Graph::finalConnectedComponents(int& stateIndex, bool& pause) {
    pause = true;
    stateIndex = 0;
    curList.clear();
    curEdge.clear();
    steps.clear();
    stepsEdge.clear();
    int res = connectedComp();
    pause = false;
    return res;
}


///////////////////////////////////////////////////////////////////////////////////

Graph graph;

void initializeGraph() {
    for (int i = 0; i < 7; i++) graph.addNode();
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 8);
    graph.addEdge(2, 0, 7);

    // Component 2: Nodes 3, 4
    //graph.addEdge(3, 4, 9);
    graph.addEdge(2, 3, 5);
    // Component 3: Nodes 5, 6
    graph.addEdge(5, 6, 51);
    graph.addEdge(4, 5, 19);
    // Additional edges to complete the 3 connected components
    graph.addEdge(0, 2, 1); // Another connection in Component 1
    graph.addEdge(3, 3, 3); // Self-loop in Component 2 for variety
    graph.prim();
}

float stepGraph = 1.0f;
float deltaTimeGraph = 0.0f;
float elapsedTimeGraph = 0.0f;
int stateIndexGraph = 0;
bool pauseGraph = false;
bool isDraggingGraph = false;
bool getFileGraph = false;
bool doubleSpeedGraph = false;
bool graphInteracting = false;
Interact graphCurInteract = REST;
int size = 0;

void renderGraph(Screen& currentScreen) {
    DrawTexture(graphBG, 0, 0, WHITE);
    deltaTimeGraph = GetFrameTime();

    if (checkClick(changeSpeed)) doubleSpeedGraph = !doubleSpeedGraph;
    if (!doubleSpeedGraph) {
        stepGraph = 1.0f;
        DrawTexture(speed1x, changeSpeed.x, changeSpeed.y, WHITE);
    }
    else {
        stepGraph = 0.5f;
        DrawTexture(speed2x, changeSpeed.x, changeSpeed.y, WHITE);
    }

    graph.dragNodes();

    if (!pauseGraph) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pauseGraph = !pauseGraph;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });


    if (checkClick(backWard)) {
        pauseGraph = true;
        if (stateIndexGraph > 0) {
            stateIndexGraph--;
            elapsedTimeGraph = 0.8f*stepGraph;
            graph.updateState(deltaTimeGraph, elapsedTimeGraph, stateIndexGraph, stepGraph);
        }
    }

    if (checkClick(forWard)) {
        pauseGraph = true;
        if (stateIndexGraph < (graph.getStepsSize() - 2)) {
            stateIndexGraph++;
            elapsedTimeGraph = 0.8f*stepGraph;
            graph.updateState(deltaTimeGraph, elapsedTimeGraph, stateIndexGraph, stepGraph);
        }
    }


    if (!pauseGraph) graph.updateState(deltaTimeGraph, elapsedTimeGraph, stateIndexGraph, stepGraph);


    // Detect if the mouse is clicking on the slidingButton
    if (checkClick(slidingButton)) {
        isDraggingGraph = true;  // Start dragging when the button is clicked
    }

    // If the mouse button is released, stop dragging
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isDraggingGraph = false;
    }

    // Update the position of the slidingButton if it is being dragged
    if (isDraggingGraph) {
        float mouseX = GetMouseX(); // Get the current mouse X position

        // Constrain the slidingButton within the slider bar's range
        float minX = 800.0f; // Left bound of the slider
        float maxX = 1205.0f; // Right bound of the slider

        slidingButton.x = mouseX;

        // Ensure slidingButton doesn't go out of bounds
        if (slidingButton.x < minX) slidingButton.x = minX;
        if (slidingButton.x > maxX) {
            slidingButton.x = maxX;
            stateIndexGraph = graph.getStepsSize() - 1;
        }


        // Update the stateIndex based on the slidingButton's position
        if (graph.getStepsSize() > 0) {
            float relativePosition = (slidingButton.x - minX) / (maxX - minX);
            stateIndexGraph = static_cast<int>(relativePosition * (graph.getStepsSize() - 1));
            elapsedTimeGraph = 0.8f; // Reset elapsed time when manually adjusting the state
            pauseGraph = true;  // Pause the animation when dragging the slider
            graph.updateState(deltaTimeGraph, elapsedTimeGraph, stateIndexGraph, stepGraph);
        }
    }
    else {
        // If not dragging, keep the button in sync with the stateIndex
        if (graph.getStepsSize() > 0)
            slidingButton.x = 800.0f + static_cast<float>(stateIndexGraph) / static_cast<float>(graph.getStepsSize() - 1) * 405.0f;
        else
            slidingButton.x = 800.0f;
    }

    // Draw the sliding button
    DrawTexture(slidingBut, slidingButton.x, slidingButton.y, WHITE);

    graph.draw();

    int fontSize = 25;
    float textWidth = MeasureText(TextFormat("%d", size), fontSize);
    float textPosX = ccGraph.x + ccGraph.width * 3 / 4 - textWidth / 2+ 42.0f;
    float textPosY = ccGraph.y + ccGraph.height / 2 - fontSize / 2;
    DrawTextEx(font, TextFormat("%d", size), { textPosX - 10, textPosY }, fontSize, 1, WHITE);
    for (int i = 0; i < 5; i++) {
        if (i != 3) if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(ccGraph)) DrawRectangleRec(ccGraph, Color{ 0, 255, 0, 32 });
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;


    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) {
            if (!graphInteracting) {

                graphCurInteract = constants::hashInter[i];
                graphInteracting = true;
            }
            else {
                graphInteracting = false;
                graphCurInteract = REST;
            }
        }
    }
    if (checkClick(ccGraph)) {
        if (!graphInteracting) {
            graphCurInteract = constants::hashInter[3];
           
            graphInteracting = true;
        }
        else {
            graphInteracting = false;
            graphCurInteract = REST;
        }
    }
    graphInteractingFunction(graphCurInteract);
}

void graphInteractingFunction(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        GraphRandom(state);
    } break;
    case INSERT:
    {
        GraphFile(state);
    } break;
    case DELETE:
    {
        GraphMSTPrim(state);
    } break;
    case SEARCH:
    {
        GraphConnectedComponents(state);
    } break;
    case FILEINTER:
    {
        GraphClear(state);
    } break;

    default: break;
    }
};

void GraphClear(Interact& state) {
    graph.clearGraph();
    graph = Graph();
    state = REST;
}

void GraphFile(Interact& state) {
    std::string selectedFilePath = FileSelectDialog();
    std::vector<int> numbers = ReadNumbersFromFile(selectedFilePath);
    if (numbers.size() == 0) {
        state = REST;
        return;
    }

    int N = numbers[0], ind = 1;

    std::vector<std::vector<int>> adjacencyMatrix(N, std::vector<int>(N, 0));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (ind < numbers.size()-1) {
                adjacencyMatrix[i][j] = numbers[ind];
                ind++;
            }
        }
    }

    graph.clearGraph();
    graph = Graph();

    graph.initializeFromMatrix(adjacencyMatrix);

    graph.copyList();
    graph.copyList();

    state = REST;

}

void GraphMSTPrim(Interact& state) {
    graph.clearColour();
    graph.finalMSTPrim(stateIndexGraph, pauseGraph);
    state = REST;
}

void GraphConnectedComponents(Interact& state) {
    graph.clearColour();

    size = graph.finalConnectedComponents(stateIndexGraph, pauseGraph);
    state = REST;
}

void GraphRandom(Interact& state) {
    graph.clearGraph();
    graph = Graph();
    int s = rand() % 15;
    std::vector<std::vector<int>> randomMatrix = generateRandomAdjacencyMatrix(s, 1, 99);
    graph.initializeFromMatrix(randomMatrix);
    graph.copyList();
    state = REST;
}

std::vector<std::vector<int>> generateRandomAdjacencyMatrix(int numNodes, int minValue, int maxValue, float density, bool allowSelfLoops) {
    // Initialize the adjacency matrix with all zeros
    std::vector<std::vector<int>> adjacencyMatrix(numNodes, std::vector<int>(numNodes, 0));

    // Seed the random number generator with the current time
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Calculate the maximum possible number of edges in an undirected graph
    int maxEdges = numNodes * (numNodes - 1) / (allowSelfLoops ? 1 : 2);

    // Calculate the number of edges based on the desired density
    int desiredEdges = static_cast<int>(density * maxEdges);

    // Generate a list of all possible edges
    std::vector<std::pair<int, int>> potentialEdges;
    for (int i = 0; i < numNodes; ++i) {
        for (int j = (allowSelfLoops ? 0 : i + 1); j < numNodes; ++j) {
            potentialEdges.emplace_back(i, j);
            if (!allowSelfLoops && i != j) {
                potentialEdges.emplace_back(j, i); // Ensure symmetry
            }
        }
    }

    // Shuffle the list of potential edges
    std::random_shuffle(potentialEdges.begin(), potentialEdges.end());

    // Add edges to the adjacency matrix based on the desired number of edges
    for (int i = 0; i < desiredEdges && i < potentialEdges.size(); ++i) {
        int u = potentialEdges[i].first;
        int v = potentialEdges[i].second;
        int weight = minValue + std::rand() % (maxValue - minValue + 1);

        adjacencyMatrix[u][v] = weight;
        adjacencyMatrix[v][u] = weight; // Ensure symmetry
    }

    return adjacencyMatrix;
}
