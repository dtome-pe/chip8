#include "../inc/chip8.h"
#include <stdio.h>   // For printf

#define ANSI_RESET   "\x1b[0m"
#define ANSI_GREEN   "\x1b[32m"

void chip8_print_memory(const Chip8 *chip8, size_t start, size_t end) {

    //Function that prints a section of the memory (bytes)

    if (start >= MEMORY_SIZE || end >= MEMORY_SIZE || start > end) {
        printf("Invalid memory range: %zu - %zu\n", start, end);
        return;
    }

    printf("\n=== CHIP-8 Memory Dump (0x%03X - 0x%03X) ===\n", (unsigned int)start, (unsigned int)end);
    
    for (size_t i = start; i <= end; i += 16) {  // Print 16 bytes per row
        printf("0x%03X: ", (unsigned int)i);

        for (size_t j = 0; j < 16 && (i + j) <= end; j++) {
            unsigned char byte = chip8->memory[i + j];
            if (byte != 0x00) {
                printf(ANSI_GREEN "%02X " ANSI_RESET, byte);
            } else {
                printf("%02X ", byte);
            }
        }

        printf("\n");
    }

    printf("============================================\n");
}

void chip8_print_full_memory(const Chip8 *chip8) {

    //Function that prints all the memory inside chip 8 (in bytes)

    printf("\n========= FULL MEMORY DUMP (4KB) =========\n");

    for (size_t section = 0; section < MEMORY_SIZE; section += 256) {
        size_t end = section + 255;

        // Ensure we don't go beyond 4095 (0xFFF)
        if (end >= MEMORY_SIZE) {
            end = MEMORY_SIZE - 1;
        }

        printf("\n-- Memory Block: 0x%03X to 0x%03X --\n", (unsigned int)section, (unsigned int)end);

        chip8_print_memory(chip8, section, end);
    }

    printf("\n============= END OF MEMORY =============\n");
}

void chip8_print_keypad(const Chip8 *chip8) {
    printf("Keypad State:\n");
    for (int i = 0; i < KEY_COUNT; i++) {
        printf("Key %X: %s\n", i, chip8->keypad[i] ? "Pressed" : "Released");
    }
}