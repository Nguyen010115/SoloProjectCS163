#pragma once
#include <iostream>
#include "raylib.h"
#include "constant.h"

extern Texture2D buttonImage;
extern Texture2D menuBG;

//Menu
void initImages();

bool checkClick(Rectangle& button);

bool checkCollision(Rectangle& button);

void initializeMenu();

void renderMenu(Screen& currentScreen);

void unloadImages();
//Hash Table
void renderHashTable(Screen& currentScreen);