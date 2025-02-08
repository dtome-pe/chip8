#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../inc/chip8.h"

void update_screen(Chip8 *chip8, Chip8Graphics *gfx)
{
    uint32_t pixels[64 * 32];  // Temporary buffer (RGBA format)

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            int byte_index = x / 8;
            int bit_index = 7 - (x % 8);
            uint8_t pixel = (chip8->screen[y][byte_index] >> bit_index) & 1;

            pixels[(y * 64) + x] = pixel ? 0xFFFFFFFF : 0x000000FF; // White or Black
        }
    }

    SDL_UpdateTexture(gfx->texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(gfx->renderer);
    SDL_RenderCopy(gfx->renderer, gfx->texture, NULL, NULL);
    SDL_RenderPresent(gfx->renderer);
}

void program_counter_jumps(uint16_t instruction, Chip8 *chip8)
{
    uint16_t address = instruction & 0x0FFF;  // Extract NNN (12-bit address)
    chip8->program_counter = address;  // Jump directly
}

char *decode_and_execute(uint16_t instruction, Chip8 *chip8, Chip8Graphics *gfx)
{
    uint8_t nibble1 = (instruction & 0xF000) >> 12;  // First  4 bits
    uint8_t nibble2 = (instruction & 0x0F00) >> 8;   // Second 4 bits
    uint8_t nibble3 = (instruction & 0x00F0) >> 4;   // Third  4 bits
    uint8_t nibble4 = (instruction & 0x000F);        // Fourth 4 bits

    switch (nibble1) {

        case(0x0):
            if (instruction == 0x00E0) { // Clear screen
                memset(chip8->screen, 0, sizeof(chip8->screen));
                }
                return "clear screen";
            if (instruction == 0x00EE) { // Return from subroutine (pop stack)
                chip8->program_counter = chip8->stack[chip8->stack_top];
                chip8->stack_top -= 1;
                }
                return "return from subroutine";

        case(0x1): // Program counter jumps
            program_counter_jumps(instruction, chip8);
            return "jump";

        case(0x2): // Jump to subroutine (jump but push previous pc address to stack)
            chip8->stack_top += 1;
            chip8->stack[chip8->stack_top] = chip8->program_counter;
            program_counter_jumps(instruction, chip8);
            return "jump to soubroutine";

        case(0x3): // Skip instruction if value in  X is equal to NN
            if (chip8->registers[nibble2] == (instruction & 0x00FF))
                chip8->program_counter += 2;
            return "Skip instruction if equals";

        case(0x4): // Skip instruction if value in X is not equal to NN
            if (chip8->registers[nibble2] != (instruction & 0x00FF))
                chip8->program_counter += 2;
            return "Skip instruction if not equals";

        case(0x05): // Skip instruction if value in X and value Y are equal
            if (chip8->registers[nibble2] == chip8->registers[nibble3])
                chip8->program_counter += 2;
            return "Skip instruction if both registers equals";

        case(0x6): // Set value to register
            chip8->registers[nibble2] = instruction & 0x00FF;

            return "set value to register";

        case(0x7): // Add value to register
            chip8->registers[nibble2] += instruction & 0x00FF;

            return "add value to register";

        case(0x09): // Skip instruction if value in X and value Y are not equal
            if (chip8->registers[nibble2] != chip8->registers[nibble3])
                chip8->program_counter += 2;
            return "Skip instruction if both registers not equals";

        case(0xA): // Set Index register
            chip8->index_register = instruction & 0x0FFF;

            return "set index register";

        case(0xD): // display
            display(nibble2, nibble3, nibble4, chip8);
            update_screen(chip8, gfx);
            return "display";

        default:
            printf("Unknown opcode: 0x%04X\n", instruction);
            return "error";

    }

    return (0);
}