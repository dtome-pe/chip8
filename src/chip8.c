#include "../inc/chip8.h"
#include <signal.h>


volatile sig_atomic_t stop = 0;

void handle_sigint(int sig)
{
    (void)sig;  // Unused parameter
    stop = 1;   // Set flag to exit loop
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    signal(SIGINT, handle_sigint);  // Set up signal handler

    Chip8           chip8;
    Chip8Graphics   gfx;

    init(&chip8, argv[1]);
    init_graphics(&gfx);
    chip8_print_full_memory(&chip8);

    while (1)
    {

        uint16_t instruction = get_16_bit_instruction(&chip8);
        chip8.program_counter += 2;
        printf("Instruction fetched: %#04x\nProgram counter incremented 2 pointing at: %#04x (byte = %#04x)\n", instruction, chip8.program_counter, chip8.memory[chip8.program_counter]);

        decode_and_execute(instruction, &chip8, &gfx);

        int c = getchar();
        if (c == 0x1B)
        {
            printf("\nESC pressed. Exiting gracefully.\n");
            break;
        }
    }

    return 0;
}