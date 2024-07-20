#include "CMakeProject1.h"
#include "constant.h"
#include "Render.h"

using namespace std;
int main() {
    InitWindow(constants::scene_width, constants::scene_height,
        "Data");
    SetTargetFPS(constants::frames_per_second);

    initImages();

    Screen currentScreen = MENU;

    while (!WindowShouldClose()) {

        BeginDrawing();
            ClearBackground(RAYWHITE);
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
                    renderHashTable(currentScreen);
                } break;
                case TREE234:
                {
                    renderHashTable(currentScreen);
                } break;
                case MINHEAP:
                {
                    renderHashTable(currentScreen);
                } break;
                case TRIE:
                {
                    renderHashTable(currentScreen);
                } break;
                case GRAPH:
                {
                    renderHashTable(currentScreen);
                } break;
                default: break;
                }

        EndDrawing();
    }

    unloadImages();
 }