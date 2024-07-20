#include "Render.h"

bool checkClick(Rectangle& button) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, button)) {
            return true;
        }
    }
    return false;
}

// Function to check if the mouse cursor is over the button (Rectangle)
bool checkCollision(Rectangle& button) {
    // Get the current mouse position
    Vector2 mousePoint = GetMousePosition();

    // Check if the mouse position is within the button rectangle
    if (CheckCollisionPointRec(mousePoint, button)) {
        return true;  // Mouse cursor is within the button rectangle
    }
    return false;  // Mouse cursor is outside the button rectangle
}