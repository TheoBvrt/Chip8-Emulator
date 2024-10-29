#include "../include/emulator.h"

//Dxyn
void draw_sprite(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	uint8_t *sprite_ptr = architecture->addr_ptr;

	int height = (int)(opcode & 0x000F);
	int posX = (int)architecture->registre_v[register_x];
	int posY = (int)architecture->registre_v[register_y];

	uint8_t sprite;
	int bits[8];

	int y = 1;
	while (y <= height) {
		sprite = *sprite_ptr;
		int i = 0;
		int x = 0;
		while (i < 8) {
			bits[7 - i] = (sprite >> i) & 1;
			i ++;
		}
		while (x < 8) {
			if (bits[x] == 1) {
				if (architecture->display_ptr[posY + y - 1][posX + x] == 1) {
					architecture->registre_v[0xF] = 1;
				}
				architecture->display_ptr[posY + y - 1][posX + x] ^= 1;
			}
			x ++;
		}
		sprite_ptr++;
		y ++;
	}

	printf("draw sprite");
}

//00E0
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