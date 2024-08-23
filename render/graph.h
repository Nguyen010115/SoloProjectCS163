#pragma once
#include "Render.h"

struct GraphNode {
	int value = -1;
	int ind = -1;
	Vector2 position;
	bool visiting = false;
	bool clicking = false;
	float radius = 25.0f;
	float alpha = 1.0f;
	int colour = 0;

	GraphNode(int index = 0, int val = 0, Vector2 pos = { 0,0 }) :
		ind(index), value(val), position(pos) {}

	void draw();

};


struct GraphEdge
{
	int startIndex = -1;
	int endIndex = -1;
	int weight = 0;
	int colour = 0;
	float alpha = 1.0f;

	GraphEdge(int s, int e, int w, int c):
		startIndex(s), endIndex(e), weight(w), colour(c){}

	void draw(std::vector<GraphNode*>& v);
};


class Graph
{
public:
	Graph(int v = 0);
	~Graph();

	void addNode(int value = 0);
	bool doesOverlap(const Vector2& pos, const std::vector<GraphNode*>& nodeList);
	void addEdge(int u, int v, int weight);
	int connectedComp();
	void updateEdges();
	void copyList();
	void dfs(int u, int componentIndex, std::vector<bool>& visited);

	void draw();
	void updateState(float deltaTime, float& elapsedTime, int& stateIndex, float step);
	int getStepsSize();

	void applyForceDirectedLayout(int iterations = 100, float areaWidth = 1400, float areaHeight = 600);
	void dfsConnected(std::vector<bool>& visited, int u);
	bool isConnected();
	int minKey(std::vector<int>& key, std::vector<bool>& mstSet);
	void prim();


	void mixedNode(GraphNode* target, GraphNode* start, GraphNode* end, float mixCoeff);
	bool isInteracting(int state);
	void finalFile(std::vector<int>& input, int& stateIndex, bool& pause);
	void finalInsert(int input, int& stateIndex, bool& pause);
	void finalDelete(int value, int& stateIndex, bool& pause);
	bool finalSearch(int value, int& stateIndex, bool& pause);
	void finalCreate(int value, int& stateIndex, bool& pause);
	void finalMSTPrim(int& stateIndex, bool& pause);
	void finalConnectedComponents(int& stateIndex, bool& pause);
	void clearGraph();


private:
	int V = 0;
	std::vector<GraphNode*> nodeList;
	std::vector<GraphEdge*> edgeList;

	std::vector < std::vector<int> > Mat;
	std::vector < std::vector<int> > ColourMat;

	//std::vector<std::vector<GraphEdge*>> EdgeMat;

	

	std::vector<GraphNode*> curList;
	std::vector < std::vector<GraphNode*> > steps;
	
	std::vector<GraphEdge*> curEdge;
	std::vector<std::vector<GraphEdge*>> stepsEdge;



};

Graph::Graph(int v)
{
	for (int i = 0; i < v; i++) addNode();
}

Graph::~Graph() {
	for (auto node : nodeList) {
		delete node;
	}
	for (auto edge : edgeList) {
		delete edge;
	}
	for (auto node : curList) {
		delete node;
	}
	for (auto edge : curEdge) {
		delete edge;
	}
}