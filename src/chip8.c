#include "../inc/chip8.h"

int main()
{
    Chip8 chip8;
    init(&chip8);

    chip8_print_full_memory(&chip8);

    return 0;
}