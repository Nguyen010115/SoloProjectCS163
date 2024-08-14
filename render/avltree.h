#pragma once
#include "Render.h"

struct avlNode
{
	int value;
	avlNode* left;
	avlNode* right;
	avlNode* parent;
	int height;
	int balance;
	Vector2 position;
	int pos;

	float radius = 25.0f;
	float alpha = 1.0f;

	bool visiting = false;
	bool balanceVisit = false;
	float alphavisiting = 0.0f;

	bool searchVisit = false;
	
	avlNode(int val, int pos = -1) : value(val), left(nullptr), right(nullptr), parent(nullptr), height(1), balance(0), position(Vector2{ 100,100 }), pos(pos), visiting(false) {}


	void draw();
	void fadein(float deltatime);
};


struct edge {
	int startIndex;
	int endIndex;
	float alpha = 1.0f;
	edge(int st = -1, int en = -1);
	void draw(const avlNode* startNode, const avlNode* endNode);
};



class avlTree
{
public:
	avlTree();
	~avlTree();

	avlNode* insertRecursive(avlNode* &node, int value, bool& inserted);
		void updateHeightAndBalance(avlNode* node);
	int getHeight(avlNode* node);
	int getBalance(avlNode* node);
	avlNode* rotateLeft(avlNode* &node);
	avlNode* rotateRight(avlNode* &node);
	avlNode* balanceTree(avlNode* &node);
	void updateHeight(avlNode* node);
	void updateBalance(avlNode* node);
	avlNode* deleteRecursive(avlNode* node, int value);
	void remove(int value);

	void deleteNode(avlNode* node);
	void clearTree(avlNode* node);


	void updatePositions();
	void updateNodePosition(avlNode* root, int level, float minX, float maxX, float levelSpacing);


	void insert(int value);

	void draw();

	void copyTree();
	avlNode* deepCopy(avlNode* root);
	
	void treeToArray(avlNode* root, std::vector<avlNode*> & list);

	void update();
	void printInOrder(avlNode* node);
	void updateEdges();
	void avlTree::updateEdgesFromNode(avlNode* node);
	void drawEdges();
	std::vector<edge> copyEdges();
	void clearTree();  
	void clearTreeRecursive(avlNode* node);

	void updateState(int& stateIndex, float& elapsedTime, float deltaTime);
	void mixNodes(avlNode* target, avlNode* start, avlNode* end, float mixCoeff);
	void avlTree::updateTree(float deltaTime, float& elapseTime, int& stateIndex);

	void finalDelete(int value, int& stateIndex,bool& pause);
	void finalInsert(int value, int& stateIndex,bool& pause);
	void finalCreate(int value, int& stateIndex,bool& pause);
		
	bool isInteracting(int t);
	avlNode* searchRecursive(avlNode* node, int value);
	bool search(int value, int& stateIndex, bool& pause);
	int getStepsSize();

private:
	avlNode* root;
	std::vector<avlNode*> nodeList;
	std::vector<edge> edgeList;
	std::vector<edge> curEdge;
	std::vector<std::vector<edge>> stepEdges;

	int size;
	int pos_steps = 0;
	bool rootRotate = false;
	std::vector < std::vector<avlNode*> > steps;
	std::vector < avlNode* > stepRoots;
	std::vector<avlNode*> curList;




	float timeStep = 1.0f;
	float curPeriod = 0.0f;
	float lastTime = 0.0f;
};

avlTree::avlTree() : size(0), root(NULL) {
}

avlTree::~avlTree() {
	// Clear the main tree
	if (root) clearTree(root);
	root = nullptr; // Avoid dangling pointer


	// Clear all steps
	for (auto& step : steps) {
		if (step[0]) clearTree(step[0]);
	}
	steps.clear();

	// If nodeList or curList have any dynamically allocated nodes, delete them as well
	nodeList.clear();

	for (auto& step : steps) {
		if (step[0]) clearTree(step[0]);
	}
	stepRoots.clear();

	for (auto* node : curList) {
		if (node) delete node;
	}
	curList.clear();
}

// Helper function to delete a single node
void avlTree::deleteNode(avlNode* node) {
	if (node != nullptr) {
		delete node;
	}
}

// Helper function to clear the entire tree
void avlTree::clearTree(avlNode* node) {
	if (node != nullptr) {
		if (node->left) clearTree(node->left);
		if (node->right) clearTree(node->right);
		deleteNode(node);
	}
}



