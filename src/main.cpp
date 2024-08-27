﻿#include "CMakeProject1.h"
#include "constant.h"
#include "Render.h"
#include "hash_table.h"


using namespace std;
int main() {
    InitWindow(constants::scene_width, constants::scene_height,
        "Data");
    SetTargetFPS(constants::frames_per_second); 
  
    Screen currentScreen = MENU;
    bool init = false;
    loadingScreen = LoadTexture("resources/images/loading_screen.png");
    float etime = 0.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        float dtime = GetFrameTime();
        etime += dtime;
        DrawTexture(loadingScreen, 0, 0, WHITE);
        if (!init) {
            initAll(currentScreen);
            init = true;
        }
        render(currentScreen);
        EndDrawing();
    }
    unloadAll;
 }