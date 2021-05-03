#include "CPU.h"
#include "PPU.h"
#include <iostream>

#define CLOCKSPEED 4194304;

// Todo

//CPU cpu("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/03-op sp,hl.gb");

CPU* cpu = new CPU("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/11-op a,(hl).gb");
PPU* ppu = new PPU(cpu);



void init();

int main(int argc, char* argv[]){
    ppu->init();
    FILE* out;
    out = fopen("out.txt", "w");
    uint8_t prev_opc;
    while(true){
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
        std::cout << opcode;
        if(opcode) {
            //fwrite(&opcode, sizeof(opcode), 1, out);
        }
        prev_opc = opcode;
    }
    fclose(out);
    return 0;
}

void init(){
    
}