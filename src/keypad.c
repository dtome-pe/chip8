#include <stdint.h>
#include <stdio.h>
#include "../inc/chip8.h"

#define KEY_COUNT 16  // CHIP-8 has 16 keys

// Set a key as pressed (1)
void chip8_key_press(Chip8 *chip8, uint8_t key) {
    if (key < KEY_COUNT) {
        chip8->keypad[key] = 1;
    }
}

// Set a key as released (0)
void chip8_key_release(Chip8 *chip8, uint8_t key) {
    if (key < KEY_COUNT) {
        chip8->keypad[key] = 0;
    }
}

// Check if a key is pressed
uint8_t chip8_is_key_pressed(const Chip8 *chip8, uint8_t key) {
    return (key < KEY_COUNT) ? chip8->keypad[key] : 0;
}
