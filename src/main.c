#include "../include/emulator.h"

void main_loop(t_architecture *architecture) {
	InitWindow(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, "Chip 8 Emulator");
	SetTargetFPS(5);
	while (!WindowShouldClose()) {
		//Timers

		//Logic
		cpu_cyle(architecture);
		//Drawing loop
		BeginDrawing();
		draw_grid(architecture);
		EndDrawing();
	}
	CloseWindow();
}

int main (int argc, char **argv) {
	t_architecture	architecture;
	t_game_loaded	game_loaded;

	//game_loaded.game_file = argv[1];
	init(&architecture, &game_loaded);
	for (size_t i = 0x200; i < 0x200 + game_loaded.file_size; i++) {
		if (i % 2 == 0) {
			printf (" ");
		}
		printf("%02X", architecture.memory[i]);
	}
	architecture.registre_v[0xF] = (uint8_t)12;
	cpu_cyle(&architecture);
	cpu_cyle(&architecture);
	//main_loop(&architecture);
	return (EXIT_SUCCESS);
}