#include <SDL2/SDL.h>
#include "CPU.h"
#include "PPU.h"
#include <iostream>

#define CLOCKSPEED 4194304;

// Todo
const int MAXCYCLES = 69905;

//CPU cpu("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/03-op sp,hl.gb");
std::string game_dir = "C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/02-interrupts.gb";
CPU* cpu = new CPU(&game_dir[0]);
PPU* ppu = new PPU(cpu);

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* texture;

void init();

void draw();

void getInput(SDL_Event event);

int main(int argc, char* argv[]){
    init();
    ppu->init();
    FILE* out;
    out = fopen("out.txt", "w");
    int currentCycles = 0;
    bool quit = false;
    SDL_Event event;
    Uint32 time1 = SDL_GetTicks();
    while(!quit){

        // while(SDL_PollEvent(&event)){
        //     getInput(event);
        //     if(event.type == SDL_QUIT){
        //         quit = true;
        //     }
        // }

        Uint32 time2 = SDL_GetTicks();

        if(time1 + (1000/60) < time2){
            while(currentCycles < MAXCYCLES){
                uint8_t opcode = cpu->getOpcode();
                if(opcode != 0x00){
                    //breakpoint for testing
                    std::cout << "A";
                }

                if(cpu->getPC() == 0xc001){
                    //breakpoint for testing
                    std::cout << "A";
                }
                cpu->interrupts->doInterrupts();
                cpu->executeOpcode(opcode);
                cpu->timer->updateTimers(cpu->h_CYCLES);
                ppu->updateGraphics(cpu->h_CYCLES);
                currentCycles+= cpu->h_CYCLES;
                
            }
            draw();
            currentCycles = 0;
            time1 = time2;
        }
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
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    FILE* file = fopen("a.txt", "w");
    for(int i = 0; i < 160; i++){
        for(int j = 0; j < 144; j++){
            SDL_Rect rect;
            rect.h = 432/144;
            rect.w = 480/160;
            rect.x = ((480/160) * i);
            rect.y = ((432/144) * j) - rect.h/2; //-> maybe remove rect.h/2
            SDL_FillRect(surface, &rect, SDL_MapRGBA(surface->format, ppu->g_SCREENDATA[i][j][0], ppu->g_SCREENDATA[i][j][1], ppu->g_SCREENDATA[i][j][2], 0xff));
            fwrite(ppu->g_SCREENDATA[i][j][0] ? "1" : "0", 1, 1, file);
        }
        fwrite("\n", 1, 1, file);
    }
    fclose(file);
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(surface);
}

void getInput(SDL_Event event){
    if(event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym){
            case SDLK_RIGHT: cpu->handleInput(0, true); break;
            case SDLK_LEFT: cpu->handleInput(1, true); break;
            case SDLK_UP: cpu->handleInput(2, true); break;
            case SDLK_DOWN: cpu->handleInput(3, true); break;
            case SDLK_a: cpu->handleInput(4, true); break;
            case SDLK_s: cpu->handleInput(5, true); break;
            case SDLK_z: cpu->handleInput(6, true); break;
            case SDLK_x: cpu->handleInput(7, true); break;
        }
    }
    else if (event.type == SDL_KEYUP){
        switch (event.key.keysym.sym){
            case SDLK_RIGHT: cpu->handleInput(0, false); break;
            case SDLK_LEFT: cpu->handleInput(1, false); break;
            case SDLK_UP: cpu->handleInput(2, false); break;
            case SDLK_DOWN: cpu->handleInput(3, false); break;
            case SDLK_a: cpu->handleInput(4, false); break;
            case SDLK_s: cpu->handleInput(5, false); break;
            case SDLK_z: cpu->handleInput(6, false); break;
            case SDLK_x: cpu->handleInput(7, false); break;
        }
    }
}