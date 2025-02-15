#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../inc/chip8.h"

void    update_screen(Chip8 *chip8, Chip8Graphics *gfx)
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

void    program_counter_jumps(uint16_t instruction, Chip8 *chip8)
{
    chip8->program_counter= instruction & 0x0FFF;  // Extract NNN (12-bit address)
}

void    decode_and_execute(uint16_t instruction, Chip8 *chip8, Chip8Graphics *gfx)
{
    uint8_t nibble1 = (instruction & 0xF000) >> 12;  // First  4 bits
    uint8_t nibble2 = (instruction & 0x0F00) >> 8;   // Second 4 bits
    uint8_t nibble3 = (instruction & 0x00F0) >> 4;   // Third  4 bits
    uint8_t nibble4 = (instruction & 0x000F);        // Fourth 4 bits

    switch (nibble1) {

        case(0x0):

            if (instruction == 0x00E0)
                { // Clear screen
                printf("CLEARED SCREEN");
                memset(chip8->screen, 0, sizeof(chip8->screen));
                return;
                }
                
            if (instruction == 0x00EE) { // Return from subroutine (pop stack)
                chip8->program_counter = chip8->stack[chip8->stack_top];
                chip8->stack_top -= 1;
                }
                printf("return from subroutine\nprogram counter:%#06x\nstack size:%d\n", chip8->memory[chip8->program_counter], chip8->stack_top);
                return;

        case(0x1): // Program counter jumps

            program_counter_jumps(instruction, chip8);
            printf("INSTRUCTION 0x1 - Program counter jumps. At: %#04x (byte = %#04x)\n", chip8->program_counter, chip8->memory[chip8->program_counter]);
            return;

        case(0x2): // Jump to subroutine (jump but push previous pc address to stack)

            chip8->stack_top += 1;
            chip8->stack[chip8->stack_top] = chip8->program_counter;
            program_counter_jumps(instruction, chip8);
            printf("PC address stored in stack:%#04x\nPC jump to:%#04x\n", chip8->stack[chip8->stack_top], chip8->memory[chip8->program_counter]);
            return;

        case(0x3): // Skip instruction if value in  X is equal to NN

            printXNN(nibble2, chip8->registers[nibble2], instruction & 0x00FF);
            if (chip8->registers[nibble2] == (instruction & 0x00FF))
            {
                chip8->program_counter += 2;
                printf("value is equal, program counter skipped to: %#06x", chip8->memory[chip8->program_counter]);
            }
            printf("value not equal. PC does not move");
            printPC(chip8->program_counter, chip8->memory[chip8->program_counter]);
            return;

        case(0x4): // Skip instruction if value in X is not equal to NN

            printXNN(nibble2, chip8->registers[nibble2], instruction & 0x00FF);
            if (chip8->registers[nibble2] != (instruction & 0x00FF))
            {
                chip8->program_counter += 2;
                printf("value is not equal, program counter skips instruction\n");
                printPC(chip8->program_counter, chip8->memory[chip8->program_counter]);
            }
            printf("value equal. PC does not move\n");
            printPC(chip8->program_counter, chip8->memory[chip8->program_counter]);   
            return;

        case(0x05): // Skip instruction if value in X and value Y are equal

            printXY(nibble2, nibble3, chip8->registers[nibble2], chip8->registers[nibble3]);
            if (chip8->registers[nibble2] == chip8->registers[nibble3])
            {
                chip8->program_counter += 2;
                printf("Value between X and Y register is equal. Program counter skips instruction");
                printPC(chip8->program_counter, chip8->memory[chip8->program_counter]);
            }
            printf("Value not equal. PC does not move");
            printPC(chip8->program_counter, chip8->memory[chip8->program_counter]);
            return;

        case(0x6): // Set value to register

            printf("INSTRUCTION 0x6: %#04x was set at register[%#04x]", instruction & 0x00FF, nibble2);
            chip8->registers[nibble2] = instruction & 0x00FF;

            return;

        case(0x7): // Add value to register

            chip8->registers[nibble2] += instruction & 0x00FF;
            printf("INSTRUCTION 0x7: %#04x was added to register[%#04x] Totalling: %#04x", instruction & 0x00FF, nibble2, chip8->registers[nibble2]);
            return;

        case(0x8): 

            if (nibble4 == 0x0) //Set reguster[X] (nibble2)  to value of register[Y] (nibble3) = 0x8XY0
                chip8->registers[nibble2] = chip8->registers[nibble3];
            else if (nibble4 == 0x1)// Set register[X] (nibble2) to the bitwise OR of register[x] and register[y]
                chip8->registers[nibble2] = chip8->registers[nibble2] | chip8->registers[nibble3];
            else if (nibble4 == 0x2)// Set register[X] (nibble2) to the bitwise AND of register[x] and register[y]
                chip8->registers[nibble2] = chip8->registers[nibble2] & chip8->registers[nibble3];
            else if (nibble4 == 0x3)// Set register[X] (nibble2) to the bitwise XOR of register[x] and register[y]
                chip8->registers[nibble2] = chip8->registers[nibble2] ^ chip8->registers[nibble3];
            else if (nibble4 == 0x4) // Set register[X] (nibble2) to the addition of register[x] and register[y]. We set the carry flag to 1 
            {
                uint16_t sum = chip8->registers[nibble2] + chip8->registers[nibble3]; // Store in 16-bit to check overflow
                // Set carry flag if overflow occurs
                chip8->registers[0xF] = (sum > 0xFF) ? 1 : 0;

                // Store only the lower 8 bits in the register
                chip8->registers[nibble2] = sum & 0xFF;
            }
            else if (nibble4 == 0x5)// Set register[X] (nibble2) to subtraction of register[x] and register[y]. and carry flag if subtrahend is larger
            {
                chip8->registers[nibble2] = chip8->registers[nibble2] - chip8->registers[nibble3];
                chip8->registers[0xF] = (chip8->registers[nibble2] > chip8->registers[nibble3]) ? 1 : 0;
            }
            else if (nibble4 == 0x7)// Set register[X] (nibble2) to subtraction of register[y] and register[x] and carry flag if subtrahend is larger
            {
                chip8->registers[nibble2] = chip8->registers[nibble3] - chip8->registers[nibble2];
                chip8->registers[0xF] = (chip8->registers[nibble3] > chip8->registers[nibble2]) ? 1 : 0;

            }
            else if (nibble4 == 0x6 || nibble4 == 0xE)
            {
                printf("INSTRUCTION 0x8\n");

                if (chip8->og_0x8_instruction) //if og mode set, set vx with value of vy
                    chip8->registers[nibble2] = chip8->registers[nibble3]; 
                    
                if (nibble4 == 0x6)
                {
                    chip8->registers[0xF] = chip8->registers[nibble2] & 0x1;
                    chip8->registers[nibble2] >>= 1;
                }
                else
                {
                    chip8->registers[0xF] = (chip8->registers[nibble2] & 0x80) >> 7;
                    chip8->registers[nibble2] <<= 1;
                }
            }
            return;

        case(0x09): // Skip instruction if value in X and value Y are not equal
            
            if (chip8->registers[nibble2] != chip8->registers[nibble3])
            {
                printf("INSTRUCTION 0x8: register[%#04x] (%#04x) and register[%#04x] (%#04x) are not equal.", nibble2, chip8->registers[nibble2], nibble3, chip8->registers[nibble3]);
                printf("Program counter incremented 2 pointing at: %#04x (byte = %#04x)\n", chip8->program_counter, chip8->memory[chip8->program_counter]);
                chip8->program_counter += 2;
            }
                
            return;

        case(0xA): // Set Index register
            printf("INSTRUCTION 0xA: Index register set to %#04x", chip8->index_register);
            chip8->index_register = instruction & 0x0FFF;

            return;

        case(0xB):
            
            if (chip8->og_0xB_instruction)
                chip8->program_counter = (instruction & 0x0FFF) + chip8->registers[0x0];
            else
                chip8->program_counter = (instruction & 0x0FFF) + chip8->registers[nibble2];

            return;

        case(0xC):
            
            uint8_t random_value = rand() % 256;  // Random number from 0 to 255
            chip8->registers[nibble2] = random_value & (instruction & 0x00FF);

            return;
            

        case(0xD): // display
            display(nibble2, nibble3, nibble4, chip8);
            update_screen(chip8, gfx);
            return;

        case(0xE):

            if (nibble3 == 0x9 && nibble4 == 0xE)
                if (chip8->keys[nibble2] == PRESSED)
                    chip8->program_counter += 2;
            else if (nibble3 == 0xA && nibble4 == 0x1)
                if (chip8->keys[nibble2] == NOT_PRESSED)
                    chip8->program_counter += 2;

            return;

        case(0xF):

            if (instruction & 0x00FF == 0x07)
                chip8->registers[nibble2] = chip8->delay_timer;
            else if (instruction & 0x00FF == 0x15)
                chip8->delay_timer = chip8->registers[nibble2];
            else if (instruction & 0x00FF == 0x17)
                chip8->sound_timer = chip8->registers[nibble2];
            else if (instruction & 0x00FF == 0x1E)
                chip8->index_register += chip8->registers[nibble2];
            else if (instruction & 0x00FF == 0x0A) // blocks and wait for input
            {
                chip8->program_counter -= 2;
                while()
            }

        default:
            printf("Unknown opcode: 0x%04X\n", instruction);
            return;

    }

    return;
}