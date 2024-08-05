#pragma once

#include "Render.h"

struct Node {
    int key;
    int value;
    Vector2 position;
    float scale;
    int fontsize = 20;
    float alpha = 1.0f;

    Node(int ckey = -1, int cvalue = -1, Vector2 cposition= { 0, 0 }, float cscale = 1);
    void drawNode() const;
    bool fadeout(float fadeSpeed);
};

//void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
//void DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);

struct Key {
    int key;
    std::vector<Node> valueList;
    Vector2 position;
    float scale;
    int fontsize = 30;

    Key(int x = 0, Vector2 cposition = { 0, 0 }, float cscale = 1);    
    void drawKey() const;
};

class Bee {
public:
    //Constructor
    Bee(int size = 10);

    //Destructor
    ~Bee();

    //Moving function
    void update(Vector2, float speed);

    //Move to key
    void moveToKey(Key& destination);

    //Move to node
    void moveToNode(Node& destination);

    //Update position
    void updatePosition(Key& key);

    //Update targetPosition
    void updateTargetPosition(Key& key);

    void updateTargetPositionNode(Node& key);


    bool updateTargetNode(Node& node);

    // Check if bee has reached the target
    bool hasReachedTarget(Vector2 target) const;

    //Draw bee
    void drawBee() const;

    //Update moving
    void updateMoving(bool b);

    //Moving checking
    bool isMoving();

    //Reached Key
    bool hasReachedKey(Key& Key);

    bool hasReachedNode(Node& Node);

    //Update target key
    bool updateTargetKey(Key& Key);

    //Target key getter
    Key getTargetKey();

    Node getTargetNode();

    Vector2 getTargetPosition();

    void moveToPos(Vector2 destination);


private:
    Vector2 position = { 0,0 };
    Vector2 targetPosition = { 0,0 };
    Key targetKey;
    Node targetNode;
    float beeHeight = 41.0f;
    float beeWidth = 41.0f;
    float scale = 1;
    float speed = 0.0f;
    float maxSpeed = 400.0f;
    float acceleration = 800.0f;
    bool moving = false;
    float zigzagAmplitude = 10.0f;
    float zigzagFrequency = 10.0f;
    float elapsedTime = 0.0f;

};

class HashTable {
public:
    // Constructor
    HashTable(int size);

    // Destructor
    ~HashTable();

    //Size getter
    int size();

    void resize(int newSize);

    void addRandom(int n);

    // Insert a key-value pair into the hash table
    void insert(int value);

    // Move bee
    void moveBee();
    void moveBeeToNode();


    // Bee reached
    bool beeHasReached(int key);


    // Search for a value by key
    bool search(int& value) const;

    // Draw the contents of the hash table
    void draw();


    //Check moving
    bool beeIsMoving();

    void updateBeeMoving(bool b);

    void interact();

    void hiddenInsert(int value);

    void revert();

    void revertInsert(int value);

    void goUp();


    void replay(int signal) {
        if (signal == 1) {
            revert();
        }
        if (signal == 2) {
            goUp();
        }
    }

    //Delete
    void deleteVal(int val);
    void hiddenDelete(int value);

    //Search
    std::pair<int, int> HashTable::searchN(int value);

    void searchVal(int value);


private:
    // Hash function to map keys to bucket index
    int hashFunction(int key) const;

    // Size of the hash table
    int tableSize;

    Bee bee;

    float scale;

    const int lowerBound = 1;
    const int upperBound = 99;

    std::queue<int> waitingValue;
    std::queue<Key> waitingKey;
    std::queue<int> deleteValue;
    std::queue<Key> deleteKey;
    std::queue<Node> deleteNode;

    std::queue<Key> searchKey;
    std::queue<Node> searchNode;

    std::vector< std::pair<int, int> > record; //<type, val>
    std::vector<std::pair<int, int>> deleteRecord; //<value, pos>
    int curState = 0;
    int curDelete = 0;

    int permission = 0;
    int perDelete = 0;
    int perSearch = 0;
    int searchState = 0;
   

    // Array of lists to handle collisions via chaining
    std::vector<Key> keyList;

};

