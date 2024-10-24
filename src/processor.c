#include "../include/emulator.h"

void debug_nible(uint8_t nible) {
	printf("Nible : [%01X]\n", nible);
}

void debug_octet(uint8_t octet) {
	printf("\n Octet : [%02X]\n", octet);
}

//Change pc_ptr to other address 1200 > 1 | 200 /> 0x200
//1nnn -> nnn:
void go_to(uint16_t opcode, t_architecture *architecture) {
	uint16_t address = opcode & 0x0FFF;
	architecture->pc_ptr = architecture->memory + address;
}

//Skip is value of register is equal of value
//3xkk -> x:register kk:value to check
void skip_if_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t registre_number = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (architecture->registre_v[registre_number] == value) {
		architecture->pc_ptr += 2;
	}
}

//Skip is value of register is not equal of value
//4xkk -> x:register kk:value to check
void skip_if_not_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t registre_number = (opcode & 0x0F00) >> 8;
	uint8_t value = opcode & 0x00FF;
	if (architecture->registre_v[registre_number] != value) {
		architecture->pc_ptr += 2;
	}
}

//Skip if the registers value are equals
//5xy0 -> x:first register y:second register
void skip_if_registre_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_1 = (opcode & 0x0F00) >> 8;
	uint8_t register_2 = (opcode & 0x00F0) >> 4;
	if (architecture->registre_v[register_1] == architecture->registre_v[register_2])
		architecture->pc_ptr += 2;
}

//Load value into register
//6xkk -> x:register kk:value to load
void load_value_in(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_target = (opcode & 0x0F00) >> 8;
	uint8_t value_to_load = opcode & 0x00FF;
	architecture->registre_v[register_target] = value_to_load;
	debug_nible(register_target);
	//printf("\n Value : [%02X]\n", architecture->registre_v[register_target]);
}

//Load value from register to another register
//8xy0 x:register to copy y:register to load
void load_in_to_in(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	architecture->registre_v[register_y] = architecture->registre_v[register_x];
}

//Add value to register
//7xkk -> x:register kk:value to add
void add_value_in(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_target = (opcode & 0x0F00) >> 8;
	uint8_t value_to_add = opcode & 0x00FF;
	architecture->registre_v[register_target] += value_to_add;
	debug_octet(architecture->registre_v[register_target]);
}

void register_or(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	architecture->registre_v[register_x] = architecture->registre_v[register_x] |
		architecture->registre_v[register_y];
	//debug_octet(architecture->registre_v[register_x]);
}

void register_and(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	architecture->registre_v[register_x] = architecture->registre_v[register_x] &
		architecture->registre_v[register_y];
	//debug_octet(architecture->registre_v[register_x]);
}

//8xy3
void register_xor(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	architecture->registre_v[register_x] = architecture->registre_v[register_x] ^
		architecture->registre_v[register_y];
	//debug_octet(architecture->registre_v[register_x]);
}

//8xy4
void safe_add(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	uint8_t check = architecture->registre_v[register_x] + architecture->registre_v[register_y];
	if (check < architecture->registre_v[register_x]) {
		architecture->registre_v[0xF] = 1;
	} else {
		architecture->registre_v[0xF] = 0;
	}
	printf("-> %02X", check);
	printf("overflow %01X ", architecture->registre_v[0xF]);
	architecture->registre_v[register_x] = check;
}

//8xy5
void safe_sub(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	uint8_t check = architecture->registre_v[register_x] - architecture->registre_v[register_y];
	if (architecture->registre_v[register_x] > architecture->registre_v[register_y]) {
		architecture->registre_v[0xF] = 1;
	} else {
		architecture->registre_v[0xF] = 0;
	}
	printf("-> %02X ", check);
	printf("[Emprunt:%01X]", architecture->registre_v[0xF]);
	//architecture->registre_v[register_x] = check;
}

//cpu_cycle read the opcode at *pc_ptr and execute the command
void cpu_cycle(t_architecture *architecture, int cycle_id) {
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
		case (0x6000) :
			load_value_in(current_opcode, architecture);
			break;
		case (0x7000) :
			add_value_in(current_opcode, architecture);
			break;
		case (0x8000) :
			switch (current_opcode & 0x000F) {
				case (0x0000):
					load_in_to_in(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0001):
					register_or(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0002):
					register_and(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0003):
					register_xor(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0004):
					safe_add(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0005):
					safe_sub(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				break;
			
			default:
				break;
			}
			break;
		default:
			break;
	}
	//printf("\n[%02X]\n", current_opcode);*/
}