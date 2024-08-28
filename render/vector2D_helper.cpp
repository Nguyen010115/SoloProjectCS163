#include "Render.h"


Vector2 CalculateCenteredTextPosition(Vector2 texturePos, float textureWidth, float textureHeight, const char* text, int fontSize) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize; // Height is roughly equal to the font size

    Vector2 centerPos;
    centerPos.x = texturePos.x + (textureWidth / 2.0f) - (textWidth / 2.0f);
    centerPos.y = texturePos.y + (textureHeight / 2.0f) - (textHeight / 2.0f);

    return centerPos;
};

Vector2 addVector2(Vector2 a, Vector2 b) {
    return Vector2{ a.x + b.x, a.y + b.y };
}

Vector2 subtractVector2(Vector2 a, Vector2 b) {
    return Vector2{ a.x - b.x, a.y - b.y };
}

Vector2 multiplyVector2(Vector2 v, float scalar) {
    return Vector2{ v.x * scalar, v.y * scalar };
}

Vector2 divideVector2(Vector2 v, float scalar) {
    return Vector2{ v.x / scalar, v.y / scalar };
}

float magnitudeVector2(Vector2 v) {
    return std::sqrt(v.x * v.y);
}

Vector2 normalizeVector2(Vector2 v) {
    float mag = magnitudeVector2(v);
    return mag != 0 ? Vector2{ v.x / mag, v.y / mag } : Vector2{ 0, 0 };
}

bool checkEqual(Vector2 u, Vector2 v) {
    return (u.x == v.x && u.y == v.y);
}

float Vector2Distance(const Vector2& v1, const Vector2& v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;

    return std::sqrt(dx * dx + dy * dy);
}

Vector2 Vector2Scale(const Vector2& v, float scalar) {
    return { v.x * scalar, v.y * scalar };
}
