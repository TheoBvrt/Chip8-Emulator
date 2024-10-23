#include "../include/emulator.h"

void clear_screen(t_architecture *architecture) {
	for (int index = 0; index < DISPLAY_HEIGHT; index++) {
		if (!memset(&architecture->display_ptr[index], 0, DISPLAY_WIDTH)) {
			exit(EXIT_FAILURE);
		}
	}
}

void draw_grid(t_architecture *architecture) {
	int displayPosX = 0;
	int displayPosY = 0;
	int y = 0;
	int x = 0;

	while (y < DISPLAY_HEIGHT) {
		while (x < DISPLAY_WIDTH) {
			DrawRectangle(displayPosX, displayPosY, SCALE, SCALE,
				(architecture->display_ptr[y][x] == 0) ? BLACK : WHITE);
			displayPosX += SCALE;
			x ++;
		}
		x = 0;
		y ++;
		displayPosX = 0;
		displayPosY += SCALE;
	}
}