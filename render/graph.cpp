#include "graph.h"

void GraphNode::draw() {
    if (value == -1) return;

    // Determine the color with fading effect using the alpha value
    Color gradientStart = ColorAlpha(WHITE, alpha);
    Color gradientEnd = ColorAlpha(GOLD, alpha);
    Color outlineColor = ColorAlpha(ORANGE, alpha);
    Color textColor = ColorAlpha(BLACK, alpha);

    if (colour > 0 && colour < 16) {
        Color temp = graphColour[colour];
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

void GraphEdge::draw(std::vector<GraphNode*>& nodeList) {
    Vector2 start = nodeList[startIndex]->position;
    Vector2 end = nodeList[endIndex]->position;
    if (start.x == 0 || end.x == 0) return;
    Color edgeColor = BLACK;

    if (colour > 0 && colour < 16) {
        Color temp = graphColour[colour];

        edgeColor = ColorAlpha(temp, alpha);
    }

    DrawLineEx(start, end, 2.0f, edgeColor);

    // Draw weight of the edge in the middle of the edge line
    Vector2 midPoint = { (start.x + end.x) / 2, (start.y + end.y) / 2 };
    std::string weightStr = std::to_string(weight);
    DrawText(weightStr.c_str(), midPoint.x - MeasureText(weightStr.c_str(), 15) / 2, midPoint.y - 15 / 2, 15, BLACK);
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
    std::cout << Mat.size() << " " << Mat[0].size() << std::endl;
    if (V == 6) std::cout << Mat[0][1] << std::endl;
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

    return componentIndex; 
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


void Graph::copyList() {


    curList.clear();
    curEdge.clear();

    updateEdges();



    // Copy nodes
    for (auto& node : nodeList) {
        GraphNode* newNode = new GraphNode(node->ind, node->value, node->position);
        newNode->colour = node->colour;
        newNode->visiting = node->visiting;
        newNode->clicking = node->clicking;
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

    elapsedTime += deltaTime;
    float G = elapsedTime / step;
    if (G > step) G = step;

    const std::vector<GraphNode*>& startList = steps[stateIndex];
    const std::vector<GraphNode*>& endList = steps[stateIndex + 1];

    const std::vector<GraphEdge*>& startEdges = stepsEdge[stateIndex];
    const std::vector<GraphEdge*>& endEdges = stepsEdge[stateIndex + 1];

    curList.clear();
    curEdge.clear();

    // Interpolate node positions
    for (size_t i = 0; i < startList.size(); ++i) {
        Vector2 mixedPosition = {
            startList[i]->position.x + G * (endList[i]->position.x - startList[i]->position.x),
            startList[i]->position.y + G * (endList[i]->position.y - startList[i]->position.y)
        };
        GraphNode* mixedNode = new GraphNode(startList[i]->ind,startList[i]->value, mixedPosition);
        mixedNode->colour = startList[i]->colour;
        mixedNode->visiting = startList[i]->visiting;
        mixedNode->alpha = startList[i]->alpha;

        curList.push_back(mixedNode);
    }

    // Interpolate edge positions and colors
    for (size_t i = 0; i < startEdges.size(); ++i) {
        Vector2 mixedStartPos = {
            startEdges[i]->startIndex + G * (endEdges[i]->startIndex - startEdges[i]->startIndex),
            startEdges[i]->endIndex + G * (endEdges[i]->endIndex - startEdges[i]->endIndex)
        };

        GraphEdge* mixedEdge = new GraphEdge(
            startEdges[i]->startIndex,  // Since indices don't change, use the same index
            startEdges[i]->endIndex,
            startEdges[i]->weight,
            startEdges[i]->colour
        );
        mixedEdge->colour = startEdges[i]->colour;
        curEdge.push_back(mixedEdge);
    }

    if (G >= step) {
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



///////////////////////////////////////////////////////////////////////////////////

Graph graph;

void initializeGraph() {
    for (int i = 0; i < 7; i++) graph.addNode();
    std::cout << "Check 1." << std::endl;
    graph.addEdge(0, 1, 1);
    graph.addEdge(1, 2, 1);
    graph.addEdge(2, 0, 1);

    // Component 2: Nodes 3, 4
    graph.addEdge(3, 4, 1);

    // Component 3: Nodes 5, 6
    graph.addEdge(5, 6, 1);

    // Additional edges to complete the 3 connected components
    graph.addEdge(0, 2, 1); // Another connection in Component 1
    graph.addEdge(3, 3, 1); // Self-loop in Component 2 for variety

    // Now, you have 3 connected components: {0,1,2}, {3,4}, {5,6}
    /*std::cout << "Check 2." << std::endl;
    for (int i = 0; i < 4; i++) graph.addNode();*/


    std::cout << "Check 3." << std::endl;
    //graph.connectedComp();  // Find and color the connected components
    graph.copyList();
}

float stepGraph = 1.0f;
float deltaTimeGraph = 0.0f;
float elapsedTimeGraph = 0.0f;
int stateIndexGraph = 0;
bool pauseGraph = false;
bool isDraggingGraph = false;
bool getFileGraph = false;

void renderGraph(Screen& currentScreen) {
    DrawTexture(avlBG, 0, 0, WHITE);
    deltaTimeGraph = GetFrameTime();

    graph.applyForceDirectedLayout();

    if (!pauseGraph) DrawTexture(pauseButImg, pauseButton.x, pauseButton.y, WHITE);
    else DrawTexture(playButImg, pauseButton.x, pauseButton.y, WHITE);

    if (checkClick(pauseButton)) {
        pauseGraph = !pauseGraph;
    }

    if (checkCollision(pauseButton)) DrawRectangleRec(pauseButton, Color{ 0, 255, 0, 32 });
    //if (checkCollision(forWard)) DrawRectangleRec(forWard, Color{ 0, 255, 0, 32 });
    //if (checkCollision(backWard)) DrawRectangleRec(backWard, Color{ 0, 255, 0, 32 });

    if (checkClick(backWard)) {
        pauseGraph = true;
        if (stateIndexGraph > 0) {
            stateIndexGraph--;
            elapsedTimeGraph = 0.9f;
            graph.updateState(deltaTimeGraph, elapsedTimeGraph, stateIndexGraph, stepGraph);
        }
    }

    if (checkClick(forWard)) {
        pauseGraph = true;
        if (stateIndexGraph < (graph.getStepsSize() - 2)) {
            stateIndexGraph++;
            elapsedTimeGraph = 0.9f;
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
        if (slidingButton.x > maxX) slidingButton.x = maxX;

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

    for (int i = 0; i < 5; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnBar)) DrawRectangleRec(returnBar, Color{ 0, 255, 0, 32 });
    if (checkClick(returnBar) || checkClick(returnButton)) currentScreen = MENU;
}