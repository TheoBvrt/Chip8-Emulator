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

//9xy0
void skip_if_register_not_equal(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_1 = (opcode & 0x0F00) >> 8;
	uint8_t register_2 = (opcode & 0x00F0) >> 4;
	printf("test");
	if (architecture->registre_v[register_1] != architecture->registre_v[register_2])
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
	architecture->registre_v[register_x] = architecture->registre_v[register_y];
}

//Add value to register
//7xkk -> x:register kk:value to add
void add_value_in(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_target = (opcode & 0x0F00) >> 8;
	uint8_t value_to_add = opcode & 0x00FF;
	architecture->registre_v[register_target] += value_to_add;
	//printf("%02X", architecture->registre_v[register_target]);
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

//8xy4 -> bug ?
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

//8xy5 -> bug ?
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
	architecture->registre_v[register_x] = check;
}

//8xy6
void shift_to_right(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	architecture->registre_v[0xF] = architecture->registre_v[register_x] & 0x1;
	architecture->registre_v[register_x] >>= 1;
	printf("[%02X]", architecture->registre_v[register_x]);
}

//8xy7
void safe_sub_2(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	uint8_t register_y = (opcode & 0x00F0) >> 4;
	uint8_t check = architecture->registre_v[register_y] - architecture->registre_v[register_x];
	if (architecture->registre_v[register_x] < architecture->registre_v[register_y]) {
		architecture->registre_v[0xF] = 1;
	} else {
		architecture->registre_v[0xF] = 0;
	}
	printf("-> %02X ", check);
	printf("[Emprunt:%01X]", architecture->registre_v[0xF]);
	architecture->registre_v[register_x] = check;
}

//8xyE
void shift_to_left(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_x = (opcode & 0x0F00) >> 8;
	architecture->registre_v[0xF] = (architecture->registre_v[register_x] & 0x80) >> 7;
	architecture->registre_v[register_x] <<= 1;
	printf("[%02X]", architecture->registre_v[register_x]);
	printf("[%02X]", architecture->registre_v[0xF]); 
}

//Annn -> nnn:
void load_addr_ptr(uint16_t opcode, t_architecture *architecture) {
	uint16_t address = opcode & 0x0FFF;
	architecture->addr_ptr = &architecture->memory[address];
}

void op_bnnn(uint16_t opcode, t_architecture *architecture) {
	uint16_t address = opcode & 0x0FFF  + architecture->registre_v[0x0];
	architecture->pc_ptr = architecture->memory + address;
}

void op_cxkk(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	uint8_t bit_mask = opcode & 0x00FF;
	architecture->registre_v[register_number] = (uint8_t)(rand() % 256) & bit_mask;
	//printf("(%02X)", architecture->registre_v[register_number]);
}

//register(x) take value from dalay timer;
void op_fx07(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	architecture->registre_v[register_number] = architecture->delay_timer;
}

//delay timer take value from register(x)
void op_fx15(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	architecture->delay_timer = architecture->registre_v[register_number];

	printf("[%02X]", architecture->delay_timer);
}

//sound timer take value from register(x)
void op_fx18(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	architecture->sound_timer = architecture->registre_v[register_number];
}

void op_fx33(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	uint8_t *copy_ptr = architecture->addr_ptr;
	int number = (int) architecture->registre_v[register_number];
	int index = 0;
	int digit[3];

	if (number < 100)
		digit[0] = 0;
	if (number < 10)
		digit[1] = 0;
	index = 2;
	while (number) {
		digit[index--] = number % 10;
		number /= 10;
	}
	for (size_t i = 0; i < 3; i++) {
		*copy_ptr++ = digit[i];
	}
}

void op_fx55(uint16_t opcode, t_architecture *architecture) {
	uint8_t register_number = (opcode & 0x0F00) >> 8;
	uint8_t *copy_ptr = architecture->addr_ptr;
	for (size_t i = 0; i < register_number; i++) {
		*copy_ptr++ = architecture->registre_v[i];
	}
}

//cpu_cycle read the opcode at *pc_ptr and execute the command
void cpu_cycle(t_architecture *architecture, int cycle_id) {
	uint8_t octet1 = *architecture->pc_ptr++;
	uint8_t octet2 = *architecture->pc_ptr++;
	uint16_t current_opcode;
	//ptr on next opcode

	current_opcode = (octet1 << 8 | octet2);
	printf("[c:%d | %04X] debug > ", cycle_id, current_opcode);
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
				case (0x0006):
					shift_to_right(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x0007):
					safe_sub_2(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				case (0x000E):
					shift_to_left(current_opcode, architecture);
					//debug_nible(architecture->registre_v[0x1]);
					break;
				break;
			default:
				break;
			}
			break;
		case (0x9000) :
			skip_if_register_not_equal(current_opcode, architecture);
			break;
		case (0xA000) :
			load_addr_ptr(current_opcode, architecture);
			break;
		case (0xB000) :
			op_bnnn(current_opcode, architecture);
			break;
		case (0xC000) :
			op_cxkk(current_opcode, architecture);
			break;
		case (0xD000) :
			draw_sprite(current_opcode, architecture);
			break;
		default:
			break;
		case (0xF000) :
			switch (current_opcode & 0x00FF) {
				case (0x0007):
					op_fx07(current_opcode, architecture);
					break;
				case (0x0015):
					op_fx15(current_opcode, architecture);
					break;
				case (0x0018):
					op_fx18(current_opcode, architecture);
					break;				
				case (0x0033):
					op_fx33(current_opcode, architecture);
					break;
				case (0x0055):
					op_fx55(current_opcode, architecture);
					break;
				default:
					break;
				}
				break;
			break;
	}
	printf("\n");
	//printf("\n[%02X]\n", current_opcode);*/
}
