
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define MEMORY_SIZE 4098
#define FONT_ADDRESS 0x050
#define FONT_SIZE 80 // 16 chars * 5 bytes
#define STACK_SIZE 16
#define TIMER_DECREMENT_PER_SECOND 60
#define KEY_COUNT 16
#define DEFAULT_IPS 700  // Default speed: 700 instructions per second
#define ROM_START 0x200
#define MAX_ROM_SIZE (MEMORY_SIZE - ROM_START)
#define PRESSED 1
#define NOT_PRESSED 0
#define FONT_START_ADDRESS


//bitwise pixel manipulation defines
#define SET_PIXEL(x, y) (screen[y][(x) / 8] |= (0x80 >> ((x) % 8)))
#define CLEAR_PIXEL(x, y) (screen[y][(x) / 8] &= ~(0x80 >> ((x) % 8)))
#define GET_PIXEL(x, y) ((screen[y][(x) / 8] & (0x80 >> ((x) % 8))) != 0)

// Declare the font set as a static constant array
static const uint8_t FONT_SET[FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Chip8Graphics;

typedef struct s_chip8 {

    uint8_t     memory[MEMORY_SIZE];
    uint8_t     screen[32][8];  // 32 rows, (8x8) 64 columns
    uint16_t    stack[STACK_SIZE];
    uint8_t     stack_top;
    uint8_t     delay_timer;
    uint8_t     sound_timer;
    uint8_t     keys[16];
    uint16_t    chip_speed;
    uint8_t     registers[16];
    uint16_t    program_counter; //program counter
    uint16_t    index_register;
    uint8_t     og_0x8_instruction; // bool configurable in config txt for ambiguous instruction
    uint8_t     og_0xB_instruction; // bool configurable in config txt for ambiguous instruction

} Chip8;


int         init(Chip8 *chip8, char *rom_path);
int         init_graphics(Chip8Graphics *gfx);
void        init_og_instruction(Chip8 *chip8);

void        chip8_print_memory(const Chip8 *chip8, size_t start, size_t end);
void        chip8_print_full_memory(const Chip8 *chip8);
void        chip8_print_keypad(const Chip8 *chip8);
void        printXNN(uint8_t nibble, uint16_t register_value, uint8_t nn);
void        printXY(uint8_t nibble_x, uint8_t nibble_y, uint16_t register_x, uint16_t register_y);
void        printPC(uint16_t pc, uint8_t pointed_byte);

uint16_t    get_16_bit_instruction(Chip8 *chip8);

void        decode_and_execute(uint16_t instruction, Chip8 *chip8, Chip8Graphics *gfx);
void        display(uint8_t x, uint8_t y, uint8_t n, Chip8 *chip8);
void        OxF_instruction(Chip8 *chip8, uint8_t x);
int         get_leftmost_digit(int num);