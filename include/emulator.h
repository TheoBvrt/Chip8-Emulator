#ifndef EMULATOR
# define EMULATOR

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "raylib.h"
#include "config.h"

// Struct
typedef struct game_loaded {
	char *name;
	FILE *game_file;
	long file_size;
} t_game_loaded;

typedef struct architecture {
	uint8_t memory[MEMORY_SIZE];
	uint16_t stack[STACK_SIZE];
	uint8_t	display_ptr[DISPLAY_HEIGHT][DISPLAY_WIDTH];
	uint8_t registre_v[16];
	uint8_t *pc_ptr;
	uint16_t *addr_ptr;

	t_game_loaded game_loaded;
} t_architecture;

void draw_grid(t_architecture *architecture);
void init(t_architecture *architecture, t_game_loaded *game_loaded);
void cpu_cyle(t_architecture *architecture, int cycle_id);

//CPU Command
void clear_screen(t_architecture *architecture);
void skip_if_equal(uint16_t opcode, t_architecture *architecture);

#endif