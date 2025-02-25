#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

typedef enum Screen { MENU, HASHTABLE, AVLTREE, TREE234, MINHEAP, TRIE, GRAPH } Screen;
typedef enum Interact {REST,CREATE, INSERT, DELETE, SEARCH, FILEINTER};


namespace constants {
	constexpr int MAX_DS_COUNT = 6;
	constexpr int scene_width = 1366;
	constexpr int scene_height = 768;
	constexpr int frames_per_second = 60;

	constexpr float optionX = 25.0f;
	constexpr float optionY = 558.5f;
	constexpr float optionWidth = 78.4f;
	constexpr float optionHeight = 36.8f;
	constexpr float optionOffsetY = 14.9f;

	constexpr float returnBarX = 22.1f;
	constexpr float returnBarY = 7.0f;
	constexpr float returnBarWidth = 54.7f;
	constexpr float returnBarHeight = 57.0f;

	constexpr float returnButtonX = 1311.0f;
	constexpr float returnButtonY = 8.1f;
	constexpr float returnButtonWidth = 55.0f;
	constexpr float returnButtonHeight = 55.0f;

	constexpr float hashX = 213.4f;
	constexpr float hashY = 99.2f;
	constexpr float hashOffsetX = 28.4f;
	constexpr float hashOffsetY = 14.2f;

	constexpr float honeydropW = 41.0f;
	constexpr float honeydropH = 41.0f;

	constexpr float honeycombW = 51.0f;
	constexpr float honeycombH = 57.0f;

	constexpr float scaling = 1366.0f / 1920.0f;
	
	
	constexpr const Screen dataName[6] = {HASHTABLE, AVLTREE, TREE234, MINHEAP, TRIE, GRAPH};
	constexpr const Interact hashInter[5] = { CREATE, INSERT, DELETE, SEARCH, FILEINTER };
	


}  

#endif  // CONSTANTS_HPP_
