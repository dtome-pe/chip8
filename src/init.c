#include "../inc/chip8.h"
#include <string.h>  // For memset
#include <stdio.h>

int check_file_extension(const char *filename, const char *ext) {
    const char *dot = strrchr(filename, '.');
    return (dot && strcmp(dot, ext) == 0);
}

// Function to load ROM into memory
int load_rom(Chip8 *chip8, const char *rom_path) {
    FILE *file = fopen(rom_path, "rb");
    if (!file) {
        printf("Error: Could not open ROM file: %s\n", rom_path);
        return 0;
    }

    // Read ROM into memory starting at 0x200
    size_t bytes_read = fread(&chip8->memory[ROM_START], 1, MAX_ROM_SIZE, file);
    fclose(file);

    if (bytes_read == 0) {
        printf("Error: ROM file is empty or unreadable.\n");
        return 0;
    }

    printf("Loaded %zu bytes from %s into memory at 0x200.\n", bytes_read, rom_path);
    return 1;
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
                int pressed = (event.type == SDL_KEYDOWN) ? 1 : 0;

                switch (event.key.keysym.sym) {
                    case SDLK_1: chip8->keys[0x1] = pressed; break;
                    case SDLK_2: chip8->keys[0x2] = pressed; break;
                    case SDLK_3: chip8->keys[0x3] = pressed; break;
                    case SDLK_4: chip8->keys[0xC] = pressed; break;

                    case SDLK_q: chip8->keys[0x4] = pressed; break;
                    case SDLK_w: chip8->keys[0x5] = pressed; break;
                    case SDLK_e: chip8->keys[0x6] = pressed; break;
                    case SDLK_r: chip8->keys[0xD] = pressed; break;

                    case SDLK_a: chip8->keys[0x7] = pressed; break;
                    case SDLK_s: chip8->keys[0x8] = pressed; break;
                    case SDLK_d: chip8->keys[0x9] = pressed; break;
                    case SDLK_f: chip8->keys[0xE] = pressed; break;

                    case SDLK_z: chip8->keys[0xA] = pressed; break;
                    case SDLK_x: chip8->keys[0x0] = pressed; break;
                    case SDLK_c: chip8->keys[0xB] = pressed; break;
                    case SDLK_v: chip8->keys[0xF] = pressed; break;
                }
                break;
            }
        }
    }
}

void init(Chip8 *chip8, char *rom_path)
{
    memset(chip8->memory, 0, MEMORY_SIZE);

    //Storing FONT_SET at conventional address
    for (size_t i = 0; i < FONT_SIZE; i++) {
        chip8->memory[FONT_ADDRESS + i] = FONT_SET[i];
    }

    memset(chip8->screen, 0, sizeof(chip8->screen));
    memset(chip8->keys, 0, sizeof(chip8->keys));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->registers, 0, sizeof(chip8->registers));

    chip8->stack_top = -1;
    chip8->chip_speed = DEFAULT_IPS;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->program_counter = 0x200;

    // Check file extension
    if (!check_file_extension(rom_path, ".ch8")) {
        printf("Error: Invalid file extension (must be .ch8)\n");
        return;
    }

    // Load ROM into memory
    if (!load_rom(chip8, rom_path)) {
        printf("Error: Failed to load ROM.\n");
    }

    init_og_instruction(chip8);

    sleep(2);

    handle_input(chip8);
}

int init_graphics(Chip8Graphics *gfx)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    gfx->window = SDL_CreateWindow("CHIP-8 Emulator",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                640, 320, SDL_WINDOW_SHOWN);
    if (!gfx->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    gfx->renderer = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);
    if (!gfx->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    gfx->texture = SDL_CreateTexture(gfx->renderer,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    64, 32);
    if (!gfx->texture) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}