#include <SDL2/SDL.h>
#include "CPU.h"
#include "PPU.h"
#include <iostream>

#define CLOCKSPEED 4194304;

// Todo
const int MAXCYCLES = 69905;

//CPU cpu("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/03-op sp,hl.gb");
std::string game_dir = "C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/dm.gb";
CPU* cpu = new CPU(&game_dir[0]);
PPU* ppu = new PPU(cpu);

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* texture;

void init();

void draw();

int main(int argc, char* argv[]){
    init();
    ppu->init();
    FILE* out;
    out = fopen("out.txt", "w");
    uint8_t prev_opc;
    int currentCycles = 0;
    bool quit = false;
    while(!quit){
        while(currentCycles < MAXCYCLES){
            uint8_t opcode = cpu->getOpcode();
            if(opcode != 0x00){
                //breakpoint for testing
                std::cout << "A";
            }

            if(cpu->getPC() == 0xc8fa){
                //breakpoint for testing
                std::cout << "A";
            }
            cpu->executeOpcode(opcode);
            cpu->timer->updateTimers(cpu->h_CYCLES);
            ppu->updateGraphics(cpu->h_CYCLES);
            cpu->interrupts->doInterrupts();
            currentCycles+= cpu->h_CYCLES;
        }
        draw();
        currentCycles = 0;
    }
    fclose(out);
    return 0;
}

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_CreateWindowAndRenderer(160, 144, 0, &window, &renderer);
    SDL_SetWindowSize(window, 480, 432);
    SDL_SetWindowResizable(window, SDL_TRUE);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 480, 432);
}

void draw(){
    // uint8_t testArr[160][144][4];
    // for(int i = 0; i < 160; i++){
    //     for(int j = 0; j < 144; j++){
    //         testArr[i][j][0] = ppu->g_SCREENDATA[i][j][0];
    //         testArr[i][j][1] = ppu->g_SCREENDATA[i][j][1];
    //         testArr[i][j][2] = ppu->g_SCREENDATA[i][j][2];
    //         testArr[i][j][3] = 0xff;
    //     }
    // }
    //SDL_UpdateTexture(texture, NULL, testArr, sizeof(uint8_t) * 4 * 160);
    // SDL_UpdateTexture(texture, NULL, ppu->g_SCREENDATA, sizeof(uint8_t) * 3 * 160);
    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    // SDL_RenderPresent(renderer);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    FILE* file = fopen("a.txt", "w");
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 144; j++){
            SDL_Rect rect;
            rect.h = 432/144;
            rect.w = 480/160;
            rect.x = 480/160 * i;
            rect.y = 432/144 * j;
            SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, ppu->g_SCREENDATA[i][j][0], ppu->g_SCREENDATA[i][j][1], ppu->g_SCREENDATA[i][j][2], 0xff));
            fwrite(ppu->g_SCREENDATA[i][j][0] ? "1" : "0", 1, 1, file);
        }
        fwrite("\n", 1, 1, file);
    }
    fclose(file);
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(surface);
}