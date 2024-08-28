#pragma once
#include <iostream>
#include "raylib.h"
#include "constant.h"
#include <math.h>
#include <vector>
#include <queue>
#include <string>
#include "file_select.h"
#include <cstdlib>
#include <unordered_map>


extern Texture2D menuBG;
extern Texture2D hashtableBG;
extern Texture2D avlBG;
extern Texture2D tree234BG;
extern Texture2D trieBG;
extern Texture2D heapBG;
extern Texture2D graphBG;


extern Texture2D pauseButImg;
extern Texture2D playButImg;
extern Texture2D slidingBut;
extern Texture2D loadingScreen;



extern Texture2D honeycomb;
extern Texture2D honeydrop;
extern Texture2D beeImage;
extern Texture2D insertSection;
extern Texture2D deleteSection;
extern Texture2D createSection;

extern Texture2D TrieInsertSection;
extern Texture2D TrieDeleteSection;

extern Texture2D speed1x;
extern Texture2D speed2x;


extern Font font;

extern Color darkGreen;


//Components


extern Rectangle hashtableOptions[6];
extern Rectangle returnBar;
extern Rectangle returnButton;
extern Rectangle randomInsert;
extern Rectangle inputSection;
extern Rectangle okInput;

extern Rectangle deleteSectionBox;
extern Rectangle okDelete;

extern Rectangle searchSectionBox;
extern Rectangle okSearch;

extern Rectangle sizeSectionBox;
extern Rectangle randomSectionBox;
extern Rectangle okRandom;


extern Rectangle random1Section;
extern Rectangle ok1random;
extern Rectangle randomCreate;

extern Rectangle backWard;
extern Rectangle forWard;
extern Rectangle pauseButton;
extern Rectangle slidingButton;

extern Rectangle changeSpeed;
extern Rectangle heapTop;
extern Rectangle heapSize;

extern Rectangle TrieInsertBox;
extern Rectangle TrieInsertOK;
extern Rectangle TrieInsertRandom;
extern Rectangle TrieDeleteBox;
extern Rectangle TrieDeleteOK;
extern Rectangle TrieSearchBox;
extern Rectangle TrieSearchOk;
extern Rectangle ccGraph;


constexpr const Color graphColour[7] = { YELLOW, MAROON, DARKBLUE, DARKBROWN, MAGENTA, DARKGREEN, GRAY};
constexpr float minX = 200.0f;
constexpr float maxX = 1300.0f;
constexpr float minY = 200.0f;
constexpr float maxY = 450.0f;
constexpr float nodeRadius = 25.0f; 
constexpr float minDistance = 2 * nodeRadius + 10.0f; 


//Menu
void initImages();

void initAll(Screen& cur);

void render(Screen& cur);

void initializeMenu();

void renderMenu(Screen& currentScreen);

void unloadAll();


//Vector2 helpers
Vector2 addVector2(Vector2 a, Vector2 b);

Vector2 subtractVector2(Vector2 a, Vector2 b);

Vector2 multiplyVector2(Vector2 v, float scalar);

Vector2 divideVector2(Vector2 v, float scalar);

float magnitudeVector2(Vector2 v);

Vector2 normalizeVector2(Vector2 v);

Vector2 Vector2Scale(const Vector2& v, float scalar);

Vector2 CalculateCenteredTextPosition(Vector2 texturePos, float textureWidth, float textureHeight, const char* text, int fontSize);

bool checkEqual(Vector2 u, Vector2 v);

float Vector2Distance(const Vector2& v1, const Vector2& v2);

//Button
bool checkClick(Rectangle& button);

bool checkCollision(Rectangle& button);

void setupButton(Rectangle& button, float x, float y, float wide, float height);

void setupButtonScale(Rectangle& button, float x, float y, float wide, float height);



//Hash Table
void initializeHash();

void renderHashTable(Screen& currentScreen);

void hashInteracting(Interact& state);

void hashInsert(Interact& state);

void hashDelete(Interact& state);

void hashSearch(Interact& state);

void hashCreate(Interact& state);

void hashFile(Interact& state);

//AVL Tree

void initializeAVL();

void renderAVLtree(Screen& currentScreen);

void avlInteracting(Interact& state);

void avlInsert(Interact& state);

void avlDelete(Interact& state);

void avlSearch(Interact& state);

void avlCreate(Interact& state);

void avlFile(Interact& state);


//234 Tree

void initialize234Tree();

void render234Tree(Screen &currentScreen);

void tree234Interacting(Interact& state);

void tree234Insert(Interact& state);

void tree234Delete(Interact& state);

void tree234Search(Interact& state);

void tree234Create(Interact& state);

void tree234File(Interact& state);

///Heap

void initializeHeap();

void renderHeap(Screen& currentScreen);

void HeapInteracting(Interact& state);

void HeapInsert(Interact& state);

void HeapDelete(Interact& state);

void HeapSearch(Interact& state);

void HeapCreate(Interact& state);

void HeapFile(Interact& state);


//Trie

void initializeTrie();

void renderTrie(Screen& currentScreen);

void TrieInteracting(Interact& state);

void TrieInsert(Interact& state);

void TrieDelete(Interact& state);

void TrieSearch(Interact& state);

void TrieCreate(Interact& state);

void TrieFile(Interact& state);

std::string GenerateRandomString(int maxLength);

//Graph

Vector2 generateRandomPosition();

void initializeGraph();

void renderGraph(Screen& currentScreen);

void graphInteractingFunction(Interact& graphCurInteract);

void GraphClear(Interact& state);

void GraphFile(Interact& state);

void GraphMSTPrim(Interact& state);

void GraphConnectedComponents(Interact& state);

void GraphRandom(Interact& state);

std::vector<std::vector<int>> generateRandomAdjacencyMatrix(int numNodes, int minValue, int maxValue, float density = 0.3f, bool allowSelfLoops = false);
