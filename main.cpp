//#include <SDL2/SDL.h>
#include "CPU.h"
#include <iostream>

#define CLOCKSPEED 4194304;

// Todo

//CPU cpu("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/03-op sp,hl.gb");

CPU* cpu = new CPU("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/06-ld r,r.gb");

int main(int argc, char* argv[]){
    FILE* out;
    out = fopen("out.txt", "w");
    uint8_t prev_opc;
    while(true){
        uint8_t opcode = cpu->getOpcode();
        if(opcode != 0x00){
            //breakpoint for testing
            std::cout << "A";
        }
        if(opcode == 0xcd){
            //breakpoint for testing
            std::cout <<"A";
        }
        if(opcode == 0x00 && prev_opc == 0x00){
            std::exit;
        }
        cpu->executeOpcode(opcode);
        std::cout << opcode;
        if(opcode) {
            //fwrite(&opcode, sizeof(opcode), 1, out);
        }
        prev_opc = opcode;
    }
    fclose(out);
    return 0;
}