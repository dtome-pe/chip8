#include "../inc/chip8.h"

uint16_t    get_16_bit_instruction(Chip8 *chip8)
{
    uint8_t first_byte = chip8->memory[chip8->program_counter];
    uint8_t second_byte = chip8->memory[chip8->program_counter + 1];

    return (first_byte << 8) | second_byte;
}