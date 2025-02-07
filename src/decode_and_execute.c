#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../inc/chip8.h"

int decode_and_execute(uint16_t instruction, Chip8 *chip8)
{
    uint8_t nibble1 = (instruction & 0xF000) >> 12;  // First  4 bits
    uint8_t nibble2 = (instruction & 0x0F00) >> 8;   // Second 4 bits
    uint8_t nibble3 = (instruction & 0x00F0) >> 4;   // Third  4 bits
    uint8_t nibble4 = (instruction & 0x000F);        // Fourth 4 bits

    switch (nibble1) {

        case(0x0):
            if (instruction == 0x00E0) { // Clear screen
                memset(chip8->screen, 0, sizeof(chip8->screen));
                write(2, "screen", 7);
                }
            break;

        case(0x1): // Program counter jumps
            uint16_t address = instruction & 0x0FFF;  // Extract NNN (12-bit address)
            chip8->program_counter = address;  // Jump directly
            break;

        case(0x6): // Set value to register
            chip8->registers[nibble2] = instruction & 0x00FF;
            break;

        case(0x7): // Add value to register
            chip8->registers[nibble2] += instruction & 0x00FF;
            break;

        case(0xA): // Set Index register
            chip8->index_register = instruction & 0x0FFF;
            break;

        case(0xD): // display
            display(nibble2, nibble3, nibble4, chip8);
            break;

        default:
            printf("Unknown opcode: 0x%04X\n", instruction);
            return -1;

    }

    return (0);
}