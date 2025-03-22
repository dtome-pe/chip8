CHIP-8 Emulator

A simple and efficient CHIP-8 emulator written in C. This project emulates the classic CHIP-8 virtual machine, allowing you to run old-school CHIP-8 games and programs.

Features

Full CHIP-8 CPU emulation

Support for original CHIP-8 opcodes

Basic graphics and input handling

Simple and lightweight implementation

Prerequisites

Ensure you have the following installed:

A C compiler (GCC, Clang, or MSVC)

SDL2 (for graphics and input handling)

Install SDL2 (Linux/macOS)

sudo apt-get install libsdl2-dev  # Ubuntu/Debian
brew install sdl2  # macOS

To compile the emulator, run Make.

Usage

Run any .ch8 rom:

./chip8 [path of the rom]

Controls

1, 2, 3, 4 → Correspond to CHIP-8 keypad keys 1, 2, 3, C

Q, W, E, R → Correspond to CHIP-8 keypad keys 4, 5, 6, D

A, S, D, F → Correspond to CHIP-8 keypad keys 7, 8, 9, E

Z, X, C, V → Correspond to CHIP-8 keypad keys A, 0, B, F

