#pragma once

#include "CPU.h"
#include <SDL2/SDL.h>

#define TILE 8
#define r_LCDC g_REGISTERS[0x0];
#define r_STAT g_REGISTERS[0x1];

// Viewport x,y offset (from left and from top)
#define r_SCY g_REGISTERS[0x2];
#define r_SCX g_REGISTERS[0x3];


#define r_LY g_REGISTERS[0x4];
#define r_LYC g_REGISTERS[0x5];
#define current_scanline r_LY;

#define r_DMA g_REGISTERS[0x6];
#define r_BGP g_REGISTERS[0x7];
#define r_OBP0 g_REGISTERS[0x8];
#define r_OBP1 g_REGISTERS[0x9];

// Control g_WINDOW x,y
#define r_WX g_REGISTERS[0xA];
#define r_WY g_REGISTERS[0xB];

class PPU{
    public:
        PPU(CPU* cpu);
        ~PPU();

        uint8_t *h_VRAM;    // cpu->h_MEMORY.memory[0x8000 to 0x9FFF] (0x2000 bytes)
        uint8_t *h_OAM;     // cpu->h_MEMORY.memory[0xFE00 - 0xFE9F] (0x00A0 bytes)

        void init();
        uint8_t getScanline();
        void drawFrame();
    private:
        uint8_t *g_REGISTERS;   // cpu->h_MEMORY.memory[0xFF40 to 0xFF4B]
    
        uint8_t g_BACKGROUND[256][256][2];  // Conversely, 32 x 32 Tiles x 2 Bits per pixel
        uint8_t g_WINDOW[256][256][2];  // Conversely, 32 x 32 Tiles x 2 Bits per pixel
        uint8_t g_VIEWPORT[160][144][2]; // Conversely, 20 x 18 Tiles x 2 Bits per pixel

        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Texture* texture;
};