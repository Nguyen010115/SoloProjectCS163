#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

typedef enum Screen { MENU, HASHTABLE, AVLTREE, TREE234, MINHEAP, TRIE, GRAPH } GameScreen;


namespace constants {
	constexpr int MAX_DS_COUNT = 6;
	constexpr int scene_width = 1366;
	constexpr int scene_height = 768;
	constexpr int frames_per_second = 30;

	constexpr int sidebar_width = 256;
	constexpr int ani_speed = 8;

	constexpr int text_buffer_size = 512;

	constexpr int min_val = 0;
	constexpr int max_val = 999;

	constexpr int default_font_size = 60;
	
	constexpr const GameScreen dataName[6] = {HASHTABLE, AVLTREE, TREE234, MINHEAP, TRIE, GRAPH};
	
	constexpr const char* default_color_path = "data/color.bin";

}  // namespace constants

#endif  // CONSTANTS_HPP_
