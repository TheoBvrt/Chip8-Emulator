#include "../include/emulator.h"

void main_loop(t_architecture *architecture) {
	int cycle_id = 0;

	InitWindow(DISPLAY_WIDTH * SCALE, DISPLAY_HEIGHT * SCALE, "Chip 8 Emulator");
	SetTargetFPS(5);
	while (!WindowShouldClose()) {
		//Timers

		//Logic
		cpu_cycle(architecture, cycle_id);
		cycle_id ++;
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

	architecture.registre_v[0x1] = (uint8_t)0xFF;
	architecture.registre_v[0xA] = (uint8_t)0x00;

	//Debug
	int nb_of_cycle = 1;
	printf("\n\n=============================================\n\n");
	for (int cycle_id = 0; cycle_id < nb_of_cycle; cycle_id++) {
		cpu_cycle(&architecture, cycle_id);
		printf("\n");
	}
	printf("\n\n=============================================\n\n");
	//main_loop(&architecture);
	return (EXIT_SUCCESS);
}