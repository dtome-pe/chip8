#include "../inc/chip8.h"

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    Chip8           chip8;
    Chip8Graphics   gfx;

    init(&chip8, argv[1]);
    init_graphics(&gfx);
    //chip8_print_full_memory(&chip8);

    while (1)
    {

        uint16_t instruction = get_16_bit_instruction(&chip8);
        chip8.program_counter += 2;

        char *instruction_made = decode_and_execute(instruction, &chip8, &gfx);
        printf("%s\n", instruction_made);
        getchar();
    }

    return 0;
}