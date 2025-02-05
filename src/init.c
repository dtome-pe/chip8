#include "../inc/chip8.h"
#include <string.h>  // For memset



void init(Chip8 *chip8)
{
    memset(chip8->memory, 0, MEMORY_SIZE);

    // Optional: Load font data into memory at FONT_ADDRESS
    for (size_t i = 0; i < FONT_SIZE; i++) {
        chip8->memory[FONT_ADDRESS + i] = FONT_SET[i];
    }
}