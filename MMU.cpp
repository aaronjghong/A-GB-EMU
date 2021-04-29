#include "MMU.h"

#include <iostream>
#include <stdio.h>

uint8_t &MMU::readMemory(uint16_t address){
    return memory[address];
}

void MMU::writeMemory(uint16_t address, uint8_t data){
    if(address >= 0xFF80 && address <= 0xFFFE){
        // unusable
    }

    // Temporary code to set-up tests with Blargg's instruction rom
    if(address == 0xFF02 && data == 0x81){
        char c = readMemory(0xFF01);
        printf("%c", c);
        memory[0xFF02] = 0x0;
    }

    
    memory[address] = data;
}

void MMU::loadGame(char* dir){
    FILE* in;
    in = fopen(dir, "rb");
    fread(&memory[0x100], 0x8000, 1, in);
    fclose(in);
}