#include "../include/emulator.h"

void parse_game(t_architecture *architecture, t_game_loaded *game_loaded) {
	if (!memset(architecture->memory, 0, MEMORY_SIZE)) 
		exit(EXIT_FAILURE);

	game_loaded->game_file = fopen("/Users/theo/Documents/Perso/C/Chip8/Cave.ch8", "rb");
	if (!game_loaded->game_file) {
		printf("Error opening file");
		exit(EXIT_FAILURE);
	}

	fseek(game_loaded->game_file, 0, SEEK_END);
	game_loaded->file_size = ftell(game_loaded->game_file);
	fseek(game_loaded->game_file, 0, SEEK_SET);

	fread (architecture->pc_ptr, sizeof(uint8_t), game_loaded->file_size, game_loaded->game_file);
	fclose(game_loaded->game_file);
}

void init(t_architecture *architecture, t_game_loaded *game_loaded) {
	architecture->pc_ptr = (architecture->memory + 0x200);
	architecture->addr_ptr = 0x0;
	srand(time(NULL));

	parse_game(architecture, game_loaded);
	for (size_t i = 0; i < 16; i++) {
		architecture->stack[i] = 0;
	}
	
	for (int index = 0; index < DISPLAY_HEIGHT; index++) {
		if (!memset(&architecture->display_ptr[index], 0, DISPLAY_WIDTH)) {
			exit(EXIT_FAILURE);
		}
	}
	for (int i = 0; i < 16; i++) {
		memset(&architecture->registre_v[i], 0, 1);
	}
	
	architecture->game_loaded = *game_loaded;
}