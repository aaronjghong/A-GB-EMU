#pragma once

#include "CPU.h"
#include <SDL2/SDL.h>

#define TILE 8


class PPU{
    public:
        PPU(CPU* cpu);
        ~PPU();

        uint8_t *h_VRAM;    // cpu->h_MEMORY.memory[0x8000 to 0x9FFF] (0x2000 bytes)
        uint8_t *h_OAM;     // cpu->h_MEMORY.memory[0xFE00 - 0xFE9F] (0x00A0 bytes)

        void init();
        uint8_t getScanline();
        void updateGraphics(int cycles);
        
    private:
        CPU* ref_cpu;

        uint8_t *g_REGISTERS;   // cpu->h_MEMORY.memory[0xFF40 to 0xFF4B]
    
        uint8_t g_BACKGROUND[256][256][2];  // Conversely, 32 x 32 Tiles x 2 Bits per pixel
        uint8_t g_WINDOW[256][256][2];  // Conversely, 32 x 32 Tiles x 2 Bits per pixel
        uint8_t g_VIEWPORT[160][144][2]; // Conversely, 20 x 18 Tiles x 2 Bits per pixel

        int cycles_scanLine = 456;

        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Texture* texture;

        uint8_t &r_LCDC = g_REGISTERS[0x0];

        // PPU "mode" + interrupts and coincidence flag
        uint8_t &r_STAT = g_REGISTERS[0x1];

        // Viewport x,y offset (from left and from top)
        uint8_t &r_SCY = g_REGISTERS[0x2];
        uint8_t &r_SCX = g_REGISTERS[0x3];


        uint8_t &r_LY = g_REGISTERS[0x4];
        uint8_t &r_LYC = g_REGISTERS[0x5];

        uint8_t &r_DMA = g_REGISTERS[0x6];
        uint8_t &r_BGP = g_REGISTERS[0x7];
        uint8_t &r_OBP0 = g_REGISTERS[0x8];
        uint8_t &r_OBP1 = g_REGISTERS[0x9];

        // Control g_WINDOW x,y
        uint8_t &r_WX = g_REGISTERS[0xA];
        uint8_t &r_WY = g_REGISTERS[0xB];

        void drawScanLine();
        void setMode();
        bool lcdEnabled();
        bool windowEnabled();
        bool spriteEnabled();
        bool bgwindowEnabled();
};