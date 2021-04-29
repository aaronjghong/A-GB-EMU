#include "CPU.h"

#define CLOCKSPEED 4194304;

// Todo

CPU cpu("C:/Users/Aaron Hong/Desktop/GB/ROMS/cpu_instrs/individual/03-op sp,hl.gb");

int main(int argc, char* argv[]){
    int i = 0;
    while(true){
        uint8_t opcode = cpu.getOpcode();
        cpu.executeOpcode(opcode);
        //i++;
    }
    return 0;
}