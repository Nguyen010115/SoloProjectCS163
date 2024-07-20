#include "Render.h"
#include "constant.h"
#include "raylib.h"

Texture2D buttonImage;
Texture2D menuBG;
Rectangle menuOptions[constants::MAX_DS_COUNT];

void initImages() {
    buttonImage = LoadTexture("resources/images/hashtable_demo.png");
    menuBG = LoadTexture("resources/images/menu_demo.png");
};

void unloadImages() {
    UnloadTexture(buttonImage);
    UnloadTexture(menuBG);
};

void initializeMenu() {
    for (int i = 0; i < constants::MAX_DS_COUNT; i++)
    {
        menuOptions[i].x = 71.2f + 431.2f * (i % 3);
        menuOptions[i].y = 177.9f + 300.3f * (i / 3);
        menuOptions[i].width = 355.8f;
        menuOptions[i].height = 213.5f;
    }
};

void renderMenu(Screen& currentScreen) {
    initializeMenu();

    DrawTexture(menuBG, 0, 0, WHITE);

    for (int i = 0; i < constants::MAX_DS_COUNT; i++)  
    {
        if (checkCollision(menuOptions[i])) DrawRectangleRec(menuOptions[i], Color{ 0, 0, 255, 32 });
    }

    for (int i = 0; i < constants::MAX_DS_COUNT; i++)
    {
        if (checkClick(menuOptions[i])) currentScreen = constants::dataName[i];
    }
};