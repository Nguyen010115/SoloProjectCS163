#pragma once
#include <iostream>
#include "raylib.h"
#include "constant.h"
#include <cmath>
#include <vector>
#include <list>
#include <utility>
#include <time.h> 
#include <queue>
#include <string>
#include <optional>
#include <thread>
#include <chrono>



extern Rectangle hashtableOptions[4];
extern Rectangle returnBar;
extern Rectangle returnButton;

extern Texture2D menuBG;
extern Texture2D hashtableBG;
extern Texture2D honeycomb;
extern Texture2D honeydrop;
extern Texture2D beeImage;
extern Texture2D insertSection;
extern Texture2D deleteSection;
extern Texture2D createSection;

extern Font font;

extern Color darkGreen;



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
void renderAVLtree(Screen& currentScreen);