#include "../include/emulator.h"

	void debug_nible(uint8_t nible) {
		printf("\n Nible : [%01X]\n", nible);
	}

void go_to(uint16_t opcode, t_architecture *architecture) {
	uint16_t address = opcode & 0x0FFF;
	architecture->pc_ptr = architecture->memory + address;
}

void skip_if_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t registre_number = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (architecture->registre_v[registre_number] == value) {
		architecture->pc_ptr += 2;
	}
}

void skip_if_not_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t registre_number = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (architecture->registre_v[registre_number] != value) {
		architecture->pc_ptr += 2;
	}
}

void skip_if_registre_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t nible1 = (opcode & 0x0F00) >> 8;
	uint8_t nible2 = (opcode & 0x00F0) >> 4;
	if (architecture->registre_v[nible1] == architecture->registre_v[nible2])
		architecture->pc_ptr += 2;
}


void cpu_cyle(t_architecture *architecture, int cycle_id) {
	uint8_t octet1 = *architecture->pc_ptr++;
	uint8_t octet2 = *architecture->pc_ptr++;
	uint16_t current_opcode;
	//ptr on next opcode

	current_opcode = (octet1 << 8 | octet2);
	printf("[c:%d | %04X]", cycle_id, current_opcode);
	switch (current_opcode & 0xF000) {
		case (0x0000) :
			switch (current_opcode & 0x00FF) {
				case (0x00E0):
					clear_screen(architecture);
					break;
				case (0x00EE):
					printf("Return to sub program");
					break;
				default:
					break;
			}
			break;
		case (0x1000) :
			go_to(current_opcode, architecture);
			break;
		case (0x3000) :
			skip_if_equal(current_opcode, architecture);
			break;
		case (0x4000) :
			skip_if_not_equal(current_opcode, architecture);
			break;
		case (0x5000) :
			skip_if_registre_equal(current_opcode, architecture);
			break;
		default:
			break;
	}
	//printf("\n[%02X]\n", current_opcode);*/
}