#include "../inc/chip8.h"
#include <stdio.h>   // For printf

void chip8_print_memory(const Chip8 *chip8, size_t start, size_t end) {
    if (start >= MEMORY_SIZE || end >= MEMORY_SIZE || start > end) {
        printf("Invalid memory range: %zu - %zu\n", start, end);
        return;
    }

    printf("\n=== CHIP-8 Memory Dump (0x%03X - 0x%03X) ===\n", (unsigned int)start, (unsigned int)end);
    
    for (size_t i = start; i <= end; i += 16) {  // Print 16 bytes per row
        printf("0x%03X: ", (unsigned int)i);

        for (size_t j = 0; j < 16 && (i + j) <= end; j++) {
            printf("%02X ", chip8->memory[i + j]);
        }

        printf("\n");
    }

    printf("============================================\n");
}

void chip8_print_full_memory(const Chip8 *chip8) {
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