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

void init(Chip8 *chip8, char *rom_path)
{
    memset(chip8->memory, 0, MEMORY_SIZE);

    //Storing FONT_SET at conventional address
    for (size_t i = 0; i < FONT_SIZE; i++) {
        chip8->memory[FONT_ADDRESS + i] = FONT_SET[i];
    }

    memset(chip8->screen, 0, sizeof(chip8->screen));
    memset(chip8->keypad, 0, sizeof(chip8->keypad));
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