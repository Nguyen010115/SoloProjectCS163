#include "Render.h"
#include "constant.h"
#include "raylib.h"

void renderHashTable(Screen& currentScreen) {
    constexpr Rectangle button = { 350, 200, 200, 100 };
    const char* buttonText = "Menu";
    Vector2 mousePoint = GetMousePosition();
    bool buttonHover = CheckCollisionPointRec(mousePoint, button);

    if (buttonHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::cout << "Button clicked!";
        currentScreen = MENU;
    }
    if (buttonHover) {
        DrawRectangleRec(button, LIGHTGRAY);
    }
    else {
        DrawRectangleRec(button, DARKGRAY);
    }

    DrawText(buttonText, button.x + 10, button.y + 10, 20, RAYWHITE);
}