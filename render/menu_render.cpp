#include "Render.h"
#include "constant.h"
#include "raylib.h"

Texture2D menuBG;
Texture2D hashtableBG;
Texture2D avlBG;
Texture2D tree234BG;
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
    tree234BG = LoadTexture("resources/images/234_BG.png");


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
    UnloadTexture(tree234BG);
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

void initAll(Screen &cur) {
    initImages();
    initializeMenu();
    initializeHash();
    initializeAVL();
    initialize234Tree();
    initializeHeap();
    initializeGraph();

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
        renderHeap(currentScreen);
    } break;
    case TRIE:
    {
        //renderHashTable(currentScreen);
    } break;
    case GRAPH:
    {
        renderGraph(currentScreen);
    } break;
    default: break;
    }
};