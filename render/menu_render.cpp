#include "Render.h"
#include "constant.h"
#include "raylib.h"

Texture2D menuBG;
Texture2D hashtableBG;
Texture2D avlBG;
Texture2D pauseButImg;
Texture2D playButImg;
Texture2D slidingBut;

Texture2D honeycomb;
Texture2D honeydrop;
Texture2D beeImage;
Texture2D insertSection;
Texture2D deleteSection;
Texture2D createSection;

Font font;

Color darkGreen;

Rectangle menuOptions[constants::MAX_DS_COUNT];

void initImages() {
    Color darkGreen = {50, 98, 14, 255 };

    font = LoadFont("resources/fonts/ComicSansMS3.ttf");

    menuBG = LoadTexture("resources/images/menu_demo.png");
    hashtableBG = LoadTexture("resources/images/hashtable_BG.png");
    avlBG = LoadTexture("resources/images/avl_BG.png");

    honeycomb = LoadTexture("resources/images/honeycomb.png");
    honeydrop = LoadTexture("resources/images/honeydrop.png");
    beeImage = LoadTexture("resources/images/flippedBee.png");
    insertSection = LoadTexture("resources/images/insertSection.png");
    deleteSection = LoadTexture("resources/images/deleteSection.png");
    createSection = LoadTexture("resources/images/createSection.png");
    pauseButImg = LoadTexture("resources/images/pausebutton.png");
    playButImg = LoadTexture("resources/images/playbutton.png");
    slidingBut = LoadTexture("resources/images/slidingbar-button.png");





};

void unloadAll() {
    UnloadFont(font);

    UnloadTexture(menuBG);
    UnloadTexture(hashtableBG);
    UnloadTexture(honeycomb);
    UnloadTexture(honeydrop);
    UnloadTexture(beeImage);
    UnloadTexture(insertSection);
    UnloadTexture(deleteSection);
    UnloadTexture(createSection);

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

void initAll() {
    initImages();
    initializeMenu();
    initializeHash();
    initializeAVL();
    initialize234Tree();
};

void renderMenu(Screen& currentScreen) {
    DrawTexture(menuBG, 0, 0, WHITE);

    for (int i = 0; i < constants::MAX_DS_COUNT; i++)  
    {
        if (checkCollision(menuOptions[i])) DrawRectangleRec(menuOptions[i], Color{ 255, 255, 0, 64 });
    }

    for (int i = 0; i < constants::MAX_DS_COUNT; i++)
    {
        if (checkClick(menuOptions[i])) currentScreen = constants::dataName[i];
    }
};

Vector2 CalculateCenteredTextPosition(Vector2 texturePos, float textureWidth, float textureHeight, const char* text, int fontSize) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize; // Height is roughly equal to the font size

    Vector2 centerPos;
    centerPos.x = texturePos.x + (textureWidth / 2.0f) - (textWidth / 2.0f);
    centerPos.y = texturePos.y + (textureHeight / 2.0f) - (textHeight / 2.0f);

    return centerPos;
};

void render(Screen& currentScreen) {
    switch (currentScreen)
    {
    case MENU:
    {
        renderMenu(currentScreen);
    } break;
    case HASHTABLE:
    {
        renderHashTable(currentScreen);
    } break;
    case AVLTREE:
    {
        renderAVLtree(currentScreen);
    } break;
    case TREE234:
    {
        render234Tree(currentScreen);
    } break;
    case MINHEAP:
    {
        //renderHashTable(currentScreen);
    } break;
    case TRIE:
    {
        //renderHashTable(currentScreen);
    } break;
    case GRAPH:
    {
        //renderHashTable(currentScreen);
    } break;
    default: break;
    }
};