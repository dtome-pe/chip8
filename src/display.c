#include "../inc/chip8.h"

void    display(uint8_t x, uint8_t y, uint8_t n, Chip8 *chip8)
{
    uint8_t x_coordinate = chip8->registers[x] & 63;
    uint8_t y_coordinate = chip8->registers[y] & 31;
    chip8->registers[0xF] = 0;

    for(int row = 0; row < n; row++)
    {
        uint8_t byte = chip8->memory[chip8->index_register + row];

        for (int col = 0;  col < 8; col++)
        {
            uint8_t pixel = (byte >> (7 - col)) & 1;

            uint8_t screen_x = (x_coordinate + col) % 64;  // Wrap at 64
            uint8_t screen_y = (y_coordinate + row) % 32;  // Wrap at 32

            uint8_t *current_pixel = &chip8->screen[screen_y][screen_x / 8];  // Access correct byte
            uint8_t mask = 0x80 >> (screen_x % 8);  // Get bit mask for the current pixel
            
            if (pixel)  // If the sprite bit is "1"
            {
                if (*current_pixel & mask) {  // Collision detection (pixel is already "ON")
                    chip8->registers[0xF] = 1;  // Set VF register (collision flag)
                }
                *current_pixel ^= mask;  // XOR pixel (toggle it)
            }
        }
    }
}