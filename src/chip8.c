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

void handle_input(Chip8 *chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: // Close window
                exit(0);
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP: {

                int is_pressed = (event.type == SDL_KEYDOWN) ? PRESSED : NOT_PRESSED;
                
                printf("key pressed\n");

                switch (event.key.keysym.sym) {
                    case SDLK_1: chip8->keys[0x1] = is_pressed; break;
                    case SDLK_2: chip8->keys[0x2] = is_pressed; break;
                    case SDLK_3: chip8->keys[0x3] = is_pressed; break;
                    case SDLK_4: chip8->keys[0xC] = is_pressed; break;

                    case SDLK_q: chip8->keys[0x4] = is_pressed; break;
                    case SDLK_w: chip8->keys[0x5] = is_pressed; break;
                    case SDLK_e: chip8->keys[0x6] = is_pressed; break;
                    case SDLK_r: chip8->keys[0xD] = is_pressed; break;

                    case SDLK_a: chip8->keys[0x7] = is_pressed; break;
                    case SDLK_s: chip8->keys[0x8] = is_pressed; break;
                    case SDLK_d: chip8->keys[0x9] = is_pressed; break;
                    case SDLK_f: chip8->keys[0xE] = is_pressed; break;

                    case SDLK_z: chip8->keys[0xA] = is_pressed; break;
                    case SDLK_x: chip8->keys[0x0] = is_pressed; break;
                    case SDLK_c: chip8->keys[0xB] = is_pressed; break;
                    case SDLK_v: chip8->keys[0xF] = is_pressed; break;
                }
                break;
            }
        }
    }
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

    chip8_print_full_memory(&chip8);

    SDL_Init(SDL_INIT_TIMER);
    SDL_AddTimer(1000 / 60, decrement_timers, &chip8);

    Uint32 last_cycle_time = SDL_GetTicks();

    while (!stop)
    {

        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - last_cycle_time;

        if (elapsed_time >= CYCLE_TIME) {
            last_cycle_time = current_time;  // Reset timer
    
            handle_input(&chip8);
            uint16_t instruction = get_16_bit_instruction(&chip8);
            chip8.program_counter += 2;
            decode_and_execute(instruction, &chip8, &gfx);

        } else {
            SDL_Delay(1);  // Sleep briefly to avoid busy-waiting
        }

    }
    return 0;
}