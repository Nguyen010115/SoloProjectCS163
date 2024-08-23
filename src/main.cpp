#include "CMakeProject1.h"
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
    
    while (!WindowShouldClose()) {
        if (!init) {
            initAll(currentScreen);
            init = true;
        }
        BeginDrawing();
            ClearBackground(RAYWHITE);
            render(currentScreen);
        EndDrawing();
    }
    unloadAll;
 }