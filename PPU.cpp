#include "PPU.h"

PPU::PPU(CPU* cpu){
    h_VRAM = &cpu->h_MEMORY.memory[0x8000];
    h_OAM = &cpu->h_MEMORY.memory[0xFE00];
    g_REGISTERS = &cpu->h_MEMORY.memory[0xFF40];
    ref_cpu = cpu;
}

PPU::~PPU(){

}

void PPU::init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer);
    SDL_SetWindowSize(window, 480, 432);
    SDL_SetWindowResizable(window, SDL_TRUE);
}

void PPU::updateGraphics(int cycles){
    setMode();
    if(lcdEnabled()){
        cycles_scanLine -= cycles;
        if (cycles_scanLine <= 0){
            // Cpu cycles for 1 scanline have passed, proceed to next scanline
            r_LY++;
            cycles_scanLine = 456;

            if(r_LY > 153){
                // Reset current scanline back to 0
                r_LY = 0;
            }
            else if(r_LY >= 144){
                // call for vblank interrupt
                ref_cpu->interrupts->requestInterrupt(0);
            }
            else if(r_LY < 144){
                drawScanLine();
            }
        }
    }
}

void PPU::drawScanLine(){

}

void PPU::setMode(){
    if(!lcdEnabled()){
        // Reset for when lcd is enabled
        cycles_scanLine = 456;
        r_LY = 0;
        r_STAT &= 0b11111100;
        r_STAT |= 0b00000001;
        // LCD either enters mode 0 or 1, set to enter mode 1 for now
        return;
    }
    uint8_t currentMode = r_STAT & 0b00000011;
    uint8_t mode = 0;
    bool interruptNeeded;

    if(r_LY >= 144){
        mode = 0b00000001;
        interruptNeeded = 0b00010000;
    }
    else{
        int bounds_mode2 = 456-80;
        int bounds_mode3 = bounds_mode2 - 172;
        if(cycles_scanLine >= bounds_mode2){
            // mode 2
            mode = 0b00000010;
            interruptNeeded = 0b00100000;
        }
        else if(cycles_scanLine>= bounds_mode3){
            // mode 3
            mode = 0b00000011;
        }
        else{
            // mode 0
            mode = 0b00000000;
            interruptNeeded = 0b00001000;
        }
    }
    // Set new mode and check if interrupt is needed
    r_STAT = (r_STAT & 0b11111100) | mode;
    if((currentMode != mode) && interruptNeeded){
        ref_cpu->interrupts->requestInterrupt(1);
    }
    if(r_LY == r_LYC){
        // Check for coincidence flag and interrupt, handle accordingly
        r_STAT |= 0b00000100;
        if((r_STAT & 0b01000000) != 0){
            ref_cpu->interrupts->requestInterrupt(1);
        }
    }
    else{
        // Coincidence flag is false
        r_STAT &= 0b11111011;
    }
}

bool PPU::lcdEnabled(){
    return (r_LCDC & 0b10000000) ? true : false;
}