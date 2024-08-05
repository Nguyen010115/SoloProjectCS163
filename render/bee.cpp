#include "hash_table.h"

////////////////////////////// Bee Class
//Constructor
Bee::Bee(int size)  {
    if (size > 10) scale = 10.0f / static_cast<float>(size);
    else scale = 1;
    beeWidth = beeHeight = 41.0f * scale;
}

//Destructor
Bee::~Bee() {

}

//DrawBee
void Bee::drawBee() const {
    DrawTextureEx(beeImage, position, 0, scale, WHITE);
}

// Update the bee's position towards the target
void Bee::update(Vector2 target, float deltaTime) {
    if (!moving) return;

    
    // Update elapsed time
    elapsedTime += deltaTime;

    // Increase speed over time (acceleration)
    if (speed < maxSpeed) {
        speed += acceleration * deltaTime;
        if (speed > maxSpeed) speed = maxSpeed;
    }

    // Calculate direction towards the target
    Vector2 direction = { target.x - position.x, target.y - position.y };
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0) {
        direction.x /= distance;
        direction.y /= distance;

        // Zigzag effect
        float zigzagOffset = zigzagAmplitude * sin(zigzagFrequency * elapsedTime);
        Vector2 zigzagDirection = { direction.y, -direction.x }; // Perpendicular to the main direction
        Vector2 movement = {
            direction.x * speed * deltaTime + zigzagDirection.x * zigzagOffset,
            direction.y * speed * deltaTime + zigzagDirection.y * zigzagOffset
        };

        // Clamp movement to avoid overshooting
        if (distance < (speed * deltaTime)) {
            position = target;
            speed = 0.0f;
            moving = false; // Stop moving once the target is reached
        }
        else {
            position.x += movement.x;
            position.y += movement.y;
        }
    }

}


// Check if the bee has reached the target position
bool Bee::hasReachedTarget(Vector2 target) const {
    return (fabs(position.x - target.x) < 1.0f && fabs(position.y - target.y) < 1.0f);
}

//Update position
void Bee::updatePosition(Key& firstKey) {
    position = firstKey.position;
    position.x -= beeWidth - 15.0f * scale;
    position.y -= beeHeight - 15.0f * scale;
}

void Bee::updateTargetPosition(Key& firstKey) {
    targetPosition = firstKey.position;
    targetPosition.x -= beeWidth - 15.0f * scale;
    targetPosition.y -= beeHeight - 15.0f * scale;
}

void Bee::updateTargetPositionNode(Node& firstKey) {
    targetPosition = firstKey.position;
    targetPosition.x -= beeWidth - 15.0f * scale;
    targetPosition.y -= beeHeight - 15.0f * scale;
}

void Bee::moveToKey(Key& destination) {
    Vector2 target = destination.position;
    target.x -= beeWidth - 15.0f * scale;
    target.y -= beeHeight - 15.0f * scale;

    float deltaTime = GetFrameTime();
    if (!hasReachedTarget(target)) {
        update(target, deltaTime);
    }
}

void Bee::moveToPos(Vector2 destination) {
    Vector2 target = destination;

    float deltaTime = GetFrameTime();
    if (!hasReachedTarget(target)) {
        update(target, deltaTime);
    }
}




void Bee::updateMoving(bool b) {
    moving = b;
}

bool Bee::isMoving() {
    return moving;
}

bool Bee::hasReachedKey(Key& Key) {
    Vector2 target = Key.position;
    target.x -= beeWidth - 15.0f * scale;
    target.y -= beeHeight - 15.0f * scale;
    return hasReachedTarget(target);
}

bool Bee::hasReachedNode(Node& Key) {
    Vector2 target = Key.position;
    target.x -= beeWidth - 15.0f * scale;
    target.y -= beeHeight - 15.0f * scale;
    return hasReachedTarget(target);
}

bool Bee::updateTargetKey(Key& Key) {
    targetKey = Key;
    updateTargetPosition(Key);
    if (hasReachedKey(Key)) return true;
    return false;
}

bool Bee::updateTargetNode(Node& node) {
    targetNode = node;
    updateTargetPositionNode(node);
    if (hasReachedNode(node)) return true;
    return false;
}


Key Bee::getTargetKey() {
    return targetKey;
}


void Bee::moveToNode(Node& destination) {
    Vector2 target = destination.position;
    target.x -= beeWidth - 15.0f * scale;
    target.y -= beeHeight - 15.0f * scale;

    float deltaTime = GetFrameTime();
    if (!hasReachedTarget(target)) {
        update(target, deltaTime);
    }
}


Node Bee::getTargetNode() {
    return targetNode;
}

Vector2 Bee::getTargetPosition() {
    return targetPosition;
}