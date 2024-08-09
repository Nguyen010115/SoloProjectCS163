#pragma once
#include <iostream>
#include "raylib.h"
#include "constant.h"
#include <math.h>
#include <vector>
#include <list>
#include <utility>
#include <time.h> 
#include <queue>
#include <string>
#include <optional>
#include <thread>
#include <chrono>




extern Texture2D menuBG;
extern Texture2D hashtableBG;
extern Texture2D avlBG;
extern Texture2D pauseButImg;
extern Texture2D playButImg;
extern Texture2D slidingBut;




extern Texture2D honeycomb;
extern Texture2D honeydrop;
extern Texture2D beeImage;
extern Texture2D insertSection;
extern Texture2D deleteSection;
extern Texture2D createSection;

extern Font font;

extern Color darkGreen;


//Components


extern Rectangle hashtableOptions[4];
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


extern constexpr int MaxNodes = 257;


//Menu
void initImages();

void initAll();

void render(Screen& cur);

void initializeMenu();

void initializeHash();

void renderMenu(Screen& currentScreen);

void unloadAll();

Vector2 CalculateCenteredTextPosition(Vector2 texturePos, float textureWidth, float textureHeight, const char* text, int fontSize);

//Hash Table
void renderHashTable(Screen& currentScreen);

void hashInteracting(Interact& state);

void hashInsert(Interact& state);

void hashDelete(Interact& state);

void hashSearch(Interact& state);

void hashCreate(Interact& state);


//Button
bool checkClick(Rectangle& button);

bool checkCollision(Rectangle& button);

void setupButton(Rectangle& button, float x, float y, float wide, float height);

void setupButtonScale(Rectangle& button, float x, float y, float wide, float height);


//AVL Tree

void initializeAVL();

void renderAVLtree(Screen& currentScreen);

void avlInteracting(Interact& state);

void avlInsert(Interact& state);

void avlDelete(Interact& state);

void avlSearch(Interact& state);

void avlCreate(Interact& state);
