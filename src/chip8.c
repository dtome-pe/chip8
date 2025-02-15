#include "../inc/chip8.h"
#include <signal.h>


volatile sig_atomic_t stop = 0;

void handle_sigint(int sig)
{
    (void)sig;  // Unused parameter
    stop = 1;   // Set flag to exit loop
}

Uint32 decrement_timers(Uint32 interval, void *param) {
    Chip8 *chip8 = (Chip8 *)param;
    
    if (chip8->delay_timer > 0) chip8->delay_timer--;
    if (chip8->sound_timer > 0) chip8->sound_timer--;

    //printf("decrease timers");
    return interval; // Keep calling this function every `interval` ms
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    signal(SIGINT, handle_sigint);  // Set up signal handler

    Chip8           chip8;
    Chip8Graphics   gfx;

    if (init(&chip8, argv[1]) == 1)
        return 1;
    init_graphics(&gfx);
    //chip8_print_full_memory(&chip8);

    SDL_AddTimer(1000 / 60, decrement_timers, &chip8);

    while (!stop)
    {
        uint16_t instruction = get_16_bit_instruction(&chip8);
        chip8.program_counter += 2;
        decode_and_execute(instruction, &chip8, &gfx);
    }

    return 0;
}