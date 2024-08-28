#include "hash_table.h"
#define cDarkGreen (Color){ 50, 98, 14, 255 } 

//Node constructor
Node::Node(int ckey, int cvalue, Vector2 cposition, float cscale)
    : key(ckey), value(cvalue), position(cposition), scale(cscale){}

void Node::drawNode() const {
    // Adjust the color using alpha
    Color colorWithAlpha = Fade(WHITE, alpha);
    DrawTextureEx(honeydrop, position, 0, scale, colorWithAlpha);

    const char* valueStr = TextFormat("%d", value);
    Vector2 posText = CalculateCenteredTextPosition(position, constants::honeydropW * scale, constants::honeydropH * scale, valueStr, fontsize * scale);
    DrawText(valueStr, posText.x, posText.y, fontsize * scale, Fade(BLACK, alpha));
}

 bool Node::fadeout(float fadeSpeed) {
    alpha = fmax(alpha - fadeSpeed, 0.0f); // Decrease alpha, ensuring it doesn't go below 0
    if (alpha < 0.01) return true;
    return false;
}

//Key constructor
Key::Key(int x, Vector2 cposition, float cscale)
    : key(x), position(cposition), scale(cscale) {}

void Key::drawKey() const {
    DrawTextureEx(honeycomb, position, 0, scale, WHITE);
    const char* valueStr = TextFormat("%d", key);
    Vector2 posText = CalculateCenteredTextPosition(position, constants::honeycombW*scale, constants::honeycombH*scale, valueStr, fontsize*scale);
    DrawText(valueStr, posText.x,posText.y, fontsize * scale, BLACK);
};


//Hash Table Class
// Constructor
HashTable::HashTable(int size) : tableSize(size) {
    if (size > 12) scale = 12.0f / static_cast<float>(size);
    else scale = 1.0;
    for (int i = 0; i < size; i++) {
        Key key = { i, {constants::hashX + scale*i * constants::hashOffsetX + scale*i*constants::honeycombW, constants::hashY} , scale};
        keyList.push_back(key);
    }
    bee = Bee{size};
    bee.updatePosition(keyList[0]);
    bee.updateTargetPosition(keyList[0]);
}

// Destructor
HashTable::~HashTable() {
}

//Size getter
int HashTable::size() {
    return tableSize;
}

void HashTable::resize(int newSize) {
    // Clear existing values
    for (auto& key : keyList) {
        key.valueList.clear();
    }

    // Update table size
    tableSize = newSize;

    // Calculate new scale
    if (newSize > 12) {
        scale = 12.0f / static_cast<float>(newSize);
    }
    else {
        scale = 1.0;
    }

    // Clear existing keys and reinitialize with new size
    keyList.clear();
    for (int i = 0; i < newSize; i++) {
        Key key = { i, {constants::hashX + scale * i * constants::hashOffsetX + scale * i * constants::honeycombW, constants::hashY}, scale };
        keyList.push_back(key);
    }

    // Update the bee's position and target with the new keyList
    if (!keyList.empty()) {
        bee.updatePosition(keyList[0]);
        bee.updateTargetPosition(keyList[0]);
    }
}




// Hash function to map keys to bucket index
int HashTable::hashFunction(int key) const {
    return key % tableSize;
}

// Insert a key-value pair into the hash table
void HashTable::insert( int value) {
    int index = hashFunction(value);

    waitingKey.push(keyList[index]);
    waitingValue.push(value);
    record.push_back({ 1, value });
    curState++;
}

void HashTable::searchVal(int value) {
    int index = hashFunction(value);

    searchKey.push(keyList[index]);
    if (search(value)) {
        searchNode.push(keyList[index].valueList[searchN(value).second]);
        searchState = 1;
  
    }
    else {
        searchNode.push(keyList[index].valueList.back());
        searchState = -1;
    }
    
}

void HashTable::deleteVal(int value) {
    int index = hashFunction(value);

    deleteKey.push(keyList[index]);
    if (search(value)) {
        deleteNode.push(keyList[index].valueList[searchN(value).second]);
        deleteValue.push(value);
        deleteRecord.push_back({ value, keyList[index].valueList.size() });
        curDelete++;
        record.push_back({ 2, value });
        curState++;
        std::cout << searchN(value).second << "\n" << deleteNode.size()  << "\n";
    }
    else {
        deleteNode.push(keyList[index].valueList.back());
        deleteValue.push(-1);
    }


    
}

void HashTable::interact() {
    if (waitingKey.size() > 0 && permission == 0) {
        bee.updateMoving(true);
        if (bee.updateTargetKey(waitingKey.front())) {
            permission = 1;
            waitingKey.pop();
        }
    }
    else if (waitingValue.size() > 0 && permission == 1) {
        hiddenInsert(waitingValue.front());
        waitingValue.pop();
        permission = 0;
        bee.updateMoving(false);

    }
    else if (deleteKey.size() > 0 && perDelete == 0) {
        bee.updateMoving(true);
        if (bee.updateTargetKey(deleteKey.front())) {
            perDelete = 1;
            deleteKey.pop();
        }
    }
    else if (deleteNode.size() > 0 && perDelete == 1) {
        bee.updateMoving(true);
        if (bee.updateTargetNode(deleteNode.front())) {
            int value = deleteNode.front().value;
            int index = hashFunction(value);
            if (deleteValue.front() != -1) {
                if (keyList[index].valueList[searchN(value).second].fadeout(0.02f)) {
                    perDelete = 2;
                    deleteNode.pop();
                }
            }
            else {
                perDelete = 2;
                deleteNode.pop();
            }
           
        }
    }
    else if (deleteValue.size() > 0 && perDelete == 2) {
        hiddenDelete(deleteValue.front());
        deleteValue.pop();
        perDelete = 0;
        bee.updateMoving(false);
    }
    else if (searchKey.size() > 0 && perSearch == 0) {
        bee.updateMoving(true);
        if (bee.updateTargetKey(searchKey.front())) {
            perSearch = 1;
            searchKey.pop();
        }
    }
    else if (searchNode.size() > 0 && perSearch == 1) {
        bee.updateMoving(true);
        if (bee.updateTargetNode(searchNode.front())) {
            if (searchState == 1) {
                int value = searchNode.front().value;
                int index = hashFunction(value);
                if (keyList[index].valueList[searchN(value).second].fadeout(0.02f)) {
                    keyList[index].valueList[searchN(value).second].alpha = 1.0f;
                    perSearch = 0;
                    searchNode.pop();
                }
            }
            else { 
                searchNode.pop();
                searchState = 0; 
                perSearch = 0;
            }

        }
    }

}

void HashTable::hiddenInsert(int value) {
    int index = hashFunction(value);
    auto& list = keyList[index].valueList;

    Vector2 pos;
    pos.x = keyList[index].position.x + scale * (constants::honeycombW - constants::honeydropW) / 2;
    pos.y = keyList[index].position.y + scale * (keyList[index].valueList.size() + 1) * constants::hashOffsetY + constants::honeycombH * scale + scale * (keyList[index].valueList.size()) * constants::honeydropH;
    Node newNode = { index, value, pos, scale };
    list.push_back(newNode); // Insert new key-value pair
}

void HashTable::addRandom(int n) {
    while (n--) {
        int s = rand() % 100;
        insert(s);
    }
}


void HashTable::hiddenDelete(int value) {
    if (value == -1) return;
    int index = hashFunction(value);
    auto& list = keyList[index].valueList;

    // Use iterator for safe removal
    auto it = std::find_if(list.begin(), list.end(),
        [value](const Node& node) { return node.value == value; });



    if (it != list.end()) {
        list.erase(it);
    }

    for (int i = 0; i < list.size(); i++) {
        list[i].position.y = keyList[index].position.y + scale * (i + 1) * constants::hashOffsetY + constants::honeycombH * scale + scale * (i) * constants::honeydropH;
    }
}


std::pair<int,int> HashTable::searchN(int value) {
    int index = hashFunction(value);
    auto& list = keyList[index].valueList;

    for (int i = 0; i < list.size(); i++) {
        if (list[i].value == value) return {index, i};
    }
    return { index, -1 };
}

// Search for a value by key
bool HashTable::search(int &value) const {
    int index = hashFunction(value);
    const auto& list = keyList[index].valueList;

    for (const auto& node : list) {
        if (node.value == value) {
            return true;
        }
    }

    return false; 
}

// Draw the contents of the hash table
void HashTable::draw() {
    for (int i = 0; i < tableSize; i++)
    {
        keyList[i].drawKey();
        for (auto& node : keyList[i].valueList) node.drawNode();
    }
    bee.drawBee();
    moveBee();
}

void HashTable::moveBee() {
    bee.moveToPos(bee.getTargetPosition());
}

void HashTable::moveBeeToNode() {
    bee.moveToNode(bee.getTargetNode());
}



bool HashTable::beeHasReached(int key) {
    Vector2 target = keyList[5].position;
    target.x -= 41.0f * scale - 15.0f * scale;
    target.y -= 41.0f * scale - 15.0f * scale;

    return bee.hasReachedTarget(target);
}


bool HashTable::beeIsMoving() {
    return bee.isMoving();
}

void HashTable::updateBeeMoving(bool b) {
    bee.updateMoving(true);
}

void HashTable::revertInsert(int value) {
    int index = hashFunction(value);
    keyList[index].valueList.pop_back();
};

void HashTable::revert() {
    if (curState > 0) {
        if (record[curState-1].first == 1) {
            revertInsert(record[curState-1].second);
            curState--;
        }
        if (record[curState].first == 2)
        {
           
        }
    }
}

void HashTable::goUp() {
    if (curState < record.size()) {
        if (record[curState + 1].first == 1) {
            insert(record[curState + 1].second);
            curState++;
        }
        if (record[curState].first == 2)
        {

        }
    }
}



///////////////////////////////////////////////////////////////////////

Rectangle hashtableOptions[6];
Rectangle returnBar;
Rectangle returnButton;
Rectangle randomInsert;
Rectangle inputSection;
Rectangle okInput;

Rectangle deleteSectionBox;
Rectangle okDelete;

Rectangle searchSectionBox;
Rectangle okSearch;

Rectangle sizeSectionBox;
Rectangle randomSectionBox;
Rectangle okRandom;

Rectangle random1Section;
Rectangle ok1random;
Rectangle randomCreate;
Rectangle heapTop;
Rectangle heapSize;

 Rectangle TrieInsertBox;
 Rectangle TrieInsertOK;
 Rectangle TrieInsertRandom;
 Rectangle TrieDeleteBox;
 Rectangle TrieDeleteOK;
 Rectangle TrieSearchBox;
 Rectangle TrieSearchOk;



extern HashTable hashTable(12);
char inputNumber[3] = "\0";
int numCount = 0;
char inputRandom[3] = "\0";
int randomCount = 0;
float timePassed = 0.0f;


Rectangle backWard;
Rectangle forWard;
Rectangle pauseButton;
Rectangle slidingButton;
Rectangle changeSpeed;

void initializeHash() {


    for (int i = 0; i < 4; i++)
    {
        setupButton(hashtableOptions[i], constants::optionX, constants::optionY + i * constants::optionHeight + i * constants::optionOffsetY, constants::optionWidth, constants::optionHeight);
    }
    setupButton(hashtableOptions[4], constants::optionX, constants::optionY - constants::optionHeight - constants::optionOffsetY, constants::optionWidth, constants::optionHeight);
    setupButton(hashtableOptions[5], constants::optionX, constants::optionY - 2*constants::optionHeight - 2*constants::optionOffsetY, constants::optionWidth, constants::optionHeight);

    setupButton(returnBar, constants::returnBarX, constants::returnBarY, constants::returnBarWidth, constants::returnBarHeight);
    setupButton(returnButton, constants::returnButtonX, constants::returnButtonY, constants::returnButtonWidth, constants::returnButtonHeight);
    setupButton(randomInsert,241, 613 , 28.0f, 28.0f);
    setupButtonScale(inputSection, 190.0f, 865.0f, 68.2f, 37.2f);
    setupButtonScale(okInput, 281.4f, 865.4f, 36.6f, 36.6f);

    setupButtonScale(deleteSectionBox, 190.0f, 865.0f + 21.0f+ 51.8f, 68.2f, 37.2f);
    setupButtonScale(okDelete, 281.4f, 865.4f + 21.0f+ 51.8f, 36.6f, 36.6f);

    setupButtonScale(searchSectionBox, 190.0f, 865.0f + 21.0f + 51.8f + 21.0f + 51.8f, 68.2f, 37.2f);
    setupButtonScale(okSearch, 281.4f, 865.4f + 21.0f + 51.8f + 21.0f + 51.8f, 36.6f, 36.6f);

    setupButtonScale(sizeSectionBox, 278.1f, 791.7f, 68.2f, 37.2f);
    setupButtonScale(randomSectionBox, 495.8f, 792.7f, 68.2f, 37.2f);

    setupButtonScale(okRandom, 575.0f, 792.3f, 36.6f, 36.6f);

    setupButtonScale(random1Section, 192.3f, 792.7f, 68.2f, 37.2f);
    setupButtonScale(ok1random, 281.4f, 792.6f, 36.6f, 36.6f);
    setupButtonScale(randomCreate, 339.1f, 790.7f, 40.3f, 40.6f);

    setupButtonScale(backWard, 830.7f, 987.2f, 65.5f, 56.8f);
    setupButtonScale(forWard, 1006.3f, 987.2f, 65.5, 56.8);
    setupButtonScale(pauseButton, 916.4f, 980.7f, 69.7f, 69.7f);
    setupButtonScale(slidingButton, 1123.9f, 984.6f, 55.0f, 48.0f);
    setupButtonScale(changeSpeed, 1801.4f, 964.8f, 80.0f, 80.0f);
    setupButtonScale(heapTop, 399.3f, 1004.8f, 110.3f, 51.8f);
    setupButtonScale(heapSize, 536.4f, 1005.4f, 151.3f, 51.8f);

    setupButtonScale(TrieInsertBox, 187.3f, 865.5f, 212.3f, 38.5f);
    setupButtonScale(TrieInsertOK, 420.6f, 865.4f, 36.6f, 36.6f);
    setupButtonScale(TrieInsertRandom, 470.2f, 863.5f, 40.6f, 40.6f);

    setupButtonScale(TrieDeleteBox, 187.3f, 938.3f, 212.3f, 38.5f);
    setupButtonScale(TrieDeleteOK, 420.6f, 938.3f, 36.6f, 36.6f);

    setupButtonScale(TrieSearchBox, 187.3f, 1011.5f, 212.3f, 38.5f);
    setupButtonScale(TrieSearchOk, 420.6f, 1011.4f, 36.6f, 36.6f);

};

 bool check = false;
 bool interacting = false;
 Interact curInteract = REST;
 bool inputClick = false;
 bool randomClick = false;

void renderHashTable(Screen& currentScreen) {
    DrawTexture(hashtableBG, 0, 0, WHITE);
    hashTable.interact();
    hashTable.draw();
    
    if (checkClick(hashtableOptions[5])) {
        hashTable.resize(0);
    }

    for (int i = 0; i < 6; i++) {
        if (checkCollision(hashtableOptions[i])) DrawRectangleRec(hashtableOptions[i], Color{ 0, 255, 0, 32 });
    }
    if (checkCollision(returnButton)) DrawRectangleRec(returnButton, Color{ 0, 255, 0, 32 });
    if (checkClick(returnButton)) currentScreen = MENU;
   
   
    for (int i = 0; i < 5; i++) {
        if (checkClick(hashtableOptions[i])) { 
            if (!interacting) {
                curInteract = constants::hashInter[i];
                interacting = true;
            }
            else {
                interacting = false;
                curInteract = REST;
            }
        }
    }
    hashInteracting(curInteract);
};

//////////////////////////////////////////////////////////////
void hashInteracting(Interact& state) {
    switch (state)
    {
    case CREATE:
    {
        hashCreate(state);
    } break;
    case INSERT:
    {
        hashInsert(state);
    } break;
    case DELETE:
    {
        hashDelete(state);
    } break;
    case SEARCH:
    {
        hashSearch(state);
    } break;
    case FILEINTER:
    {
        hashFile(state);
    } break;
    default: break;
    }
};

bool getFileHash = false;
void hashFile(Interact& state) {

        std::string selectedFilePath = FileSelectDialog();
        std::vector<int> numbers = ReadNumbersFromFile(selectedFilePath);
        if (numbers.size() == 0) {
            state = REST;

            return;
        }
        hashTable.resize(numbers[0]);
        for (int i = 1; i < numbers.size(); i++) {
            hashTable.insert(numbers[i]);
        }
        state = REST;

    
}

void hashInsert(Interact& state) {
    DrawTexture(insertSection, hashtableOptions[1].x + 90.0f, hashtableOptions[1].y, WHITE);

    if (checkCollision(randomInsert)) DrawRectangleRec(randomInsert, Color{ 0, 255, 0, 32 });
    if (checkCollision(okInput)) DrawRectangleRec(okInput, Color{ 0, 255, 0, 32 });
    if (checkCollision(inputSection)) {
        DrawRectangleRec(inputSection, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    
    if (checkClick(inputSection)) inputClick = true;
    else if (!checkClick(inputSection) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inputClick = false;

    if (inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (numCount < 2))
            {
                inputNumber[numCount] = (char)key;
                inputNumber[numCount + 1] = '\0'; // Add null terminator at the end of the string.
                numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            numCount--;
            if (numCount < 0) numCount = 0;
            inputNumber[numCount] = '\0';
        }

        if (fmod(timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)inputSection.x + 10 + MeasureText(inputNumber, 20), (int)inputSection.y + 8, 20, DARKGREEN);
        }
    }

    timePassed = GetTime();
    
    DrawText(inputNumber, (int)inputSection.x + 7, (int)inputSection.y + 4, 20, DARKGREEN);

    if ((checkClick(okInput) || IsKeyPressed(KEY_ENTER)) && !hashTable.beeIsMoving()) {
        if (numCount > 0) {
            int input = std::stoi(inputNumber);
            hashTable.insert(input);
            while (numCount > 0) {
                numCount--;
                inputNumber[numCount] = '\0';
            }
        }
    }



    if (checkClick(randomInsert)) {
        int s = rand() % 100;
        hashTable.insert(s);
    }

    


};


void hashDelete(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[2].x + 90.0f, hashtableOptions[2].y, WHITE);

    if (checkCollision(okDelete)) DrawRectangleRec(okDelete, Color{ 0, 255, 0, 32 });
    if (checkCollision(deleteSectionBox)) {
        DrawRectangleRec(deleteSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(deleteSectionBox)) inputClick = true;
    else if (!checkClick(deleteSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inputClick = false;

    if (inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (numCount < 2))
            {
                inputNumber[numCount] = (char)key;
                inputNumber[numCount + 1] = '\0'; // Add null terminator at the end of the string.
                numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            numCount--;
            if (numCount < 0) numCount = 0;
            inputNumber[numCount] = '\0';
        }

        if (fmod(timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)deleteSectionBox.x + 10 + MeasureText(inputNumber, 20), (int)deleteSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    timePassed = GetTime();

    DrawText(inputNumber, (int)deleteSectionBox.x + 7, (int)deleteSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okDelete) || IsKeyPressed(KEY_ENTER)) && !hashTable.beeIsMoving()) {
        if (numCount > 0) {
            int input = std::stoi(inputNumber);
            hashTable.deleteVal(input);
            while (numCount > 0) {
                numCount--;
                inputNumber[numCount] = '\0';
            }
        }
    }

};


void hashSearch(Interact& state) {
    DrawTexture(deleteSection, hashtableOptions[3].x + 90.0f, hashtableOptions[3].y, WHITE);

    if (checkCollision(okSearch)) DrawRectangleRec(okSearch, Color{ 0, 255, 0, 32 });
    if (checkCollision(searchSectionBox)) {
        DrawRectangleRec(searchSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(searchSectionBox)) inputClick = true;
    else if (!checkClick(searchSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inputClick = false;

    if (inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (numCount < 2))
            {
                inputNumber[numCount] = (char)key;
                inputNumber[numCount + 1] = '\0'; // Add null terminator at the end of the string.
                numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            numCount--;
            if (numCount < 0) numCount = 0;
            inputNumber[numCount] = '\0';
        }

        if (fmod(timePassed, 1.0f) < 0.5f)
        {
            DrawText("_", (int)searchSectionBox.x + 10 + MeasureText(inputNumber, 20), (int)searchSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    timePassed = GetTime();

    DrawText(inputNumber, (int)searchSectionBox.x + 7, (int)searchSectionBox.y + 4, 20, DARKGREEN);

    if ((checkClick(okSearch) || IsKeyPressed(KEY_ENTER)) && !hashTable.beeIsMoving()) {
        if (numCount > 0) {
            int input = std::stoi(inputNumber);
            hashTable.searchVal(input);
            while (numCount > 0) {
                numCount--;
                inputNumber[numCount] = '\0';
            }
        }
    }

}


void hashCreate(Interact& state) {
    DrawTexture(createSection, hashtableOptions[0].x + 90.0f, hashtableOptions[0].y, WHITE);
    if (checkCollision(okRandom)) DrawRectangleRec(okRandom, Color{ 0, 255, 0, 32 });
    if (checkCollision(sizeSectionBox)) {
        DrawRectangleRec(sizeSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }

    else if (checkCollision(randomSectionBox)) {
        DrawRectangleRec(randomSectionBox, Color{ 0, 255, 0, 32 });
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


    if (checkClick(sizeSectionBox)) inputClick = true;
    else if (!checkClick(sizeSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) inputClick = false;

    if (inputClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (numCount < 2)) {
                inputNumber[numCount] = (char)key;
                inputNumber[numCount + 1] = '\0'; // Add null terminator at the end of the string.
                numCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            numCount--;
            if (numCount < 0) numCount = 0;
            inputNumber[numCount] = '\0';
        }

        if (fmod(timePassed, 1.0f) < 0.5f) {
            DrawText("_", (int)sizeSectionBox.x + 10 + MeasureText(inputNumber, 20), (int)sizeSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    timePassed = GetTime();

    DrawText(inputNumber, (int)sizeSectionBox.x + 7, (int)sizeSectionBox.y + 4, 20, DARKGREEN);

   

    if (checkClick(randomSectionBox)) randomClick = true;
    else if (!checkClick(randomSectionBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) randomClick = false;

    if (randomClick) {
        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0) {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 48) && (key <= 57) && (randomCount < 2)) {
                inputRandom[randomCount] = (char)key;
                inputRandom[randomCount + 1] = '\0'; // Add null terminator at the end of the string.
                randomCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            randomCount--;
            if (randomCount < 0) randomCount = 0;
            inputRandom[randomCount] = '\0';
        }

        if (fmod(timePassed, 1.0f) < 0.5f) {
            DrawText("_", (int)randomSectionBox.x + 10 + MeasureText(inputRandom, 20), (int)randomSectionBox.y + 8, 20, DARKGREEN);
        }
    }

    timePassed = GetTime();

    DrawText(inputRandom, (int)randomSectionBox.x + 7, (int)randomSectionBox.y + 4, 20, DARKGREEN);


    if ((checkClick(okRandom) || IsKeyPressed(KEY_ENTER)) && !hashTable.beeIsMoving()) {
        if (randomCount > 0 && numCount > 0) {
            int input = std::stoi(inputNumber);
            hashTable.resize(input);
            while (numCount > 0) {
                numCount--;
                inputNumber[numCount] = '\0';
            }
            int inputRan = std::stoi(inputRandom);
            hashTable.addRandom(input);
            while (randomCount > 0) {
                randomCount--;
                inputRandom[randomCount] = '\0';
            }
        }
    }


    
}
