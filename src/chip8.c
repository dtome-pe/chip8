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
    //chip8_print_full_memory(&chip8);

    while (1)
    {

        uint16_t instruction = get_16_bit_instruction(&chip8);
        chip8.program_counter += 2;

        char *instruction_made = decode_and_execute(instruction, &chip8, &gfx);
        printf("%s\n", instruction_made);
        
        int c = getchar();
        if (c == 0x1B)
        {
            printf("\nESC pressed. Exiting gracefully.\n");
            break;
        }
    }

    return 0;
}