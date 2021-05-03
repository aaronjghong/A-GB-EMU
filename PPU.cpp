#include "PPU.h"

PPU::PPU(CPU* cpu){
    h_VRAM = &cpu->h_MEMORY.memory[0x8000];
    h_OAM = &cpu->h_MEMORY.memory[0xFE00];
    g_REGISTERS = &cpu->h_MEMORY.memory[0xFF40];
}

PPU::~PPU(){

}

uint8_t PPU::getScanline(){

}

void PPU::init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer);
    SDL_SetWindowSize(window, 480, 432);
    SDL_SetWindowResizable(window, SDL_TRUE);
}

void PPU::drawFrame(){
    //SDL.UpdateTexture(&texture, NULL, /*Buffer goes here*/, )
}