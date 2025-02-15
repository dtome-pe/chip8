#include "../inc/chip8.h"

uint16_t    get_16_bit_instruction(Chip8 *chip8)
{
    uint8_t first_byte = chip8->memory[chip8->program_counter];
    uint8_t second_byte = chip8->memory[chip8->program_counter + 1];

    return (first_byte << 8) | second_byte;
}

void    init_og_instruction(Chip8 *chip8) {
    chip8->og_0x8_instruction = 0; 
    chip8->og_0xB_instruction = 0;
    //Check instruction mode in config.txt. if not found emulator wont set Vx to the value of Vy in instructions 0XY6 and 0XYE t
    FILE *file = fopen("config.txt", "r");
    if (!file){
        printf("OG instructions not set\n");
        return ;
    } 

    char line[32];
    while (fgets(line, sizeof(line), file)) {

        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "OG_0x8_INSTRUCTION=ON", 22) == 0) {
            chip8->og_0x8_instruction = 1;
            printf("Emulator set to set VX to value of VY in 0X8 instructions\n");
        }
        if (strncmp(line, "OG_0xB_INSTRUCTION=ON", 22) == 0) {
            chip8->og_0xB_instruction = 1;
            printf("Emulator set to to execute 0xBNNN instructions instead of 0xBXNN\n");
        }

    }
    fclose(file);
    if (chip8->og_0x8_instruction == 0 && chip8->og_0xB_instruction == 0)
        printf("OG instructions not set\n");
    return ;
}