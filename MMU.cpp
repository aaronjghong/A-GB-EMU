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
        FILE* out = fopen("tests.txt", "a");
        char c = readMemory(0xFF01);
        fwrite(&c, sizeof(c), 1, out);
        fclose(out);
        printf("%c", c);
        memory[0xFF02] = 0x0;
    }

    
    memory[address] = data;
}

void MMU::loadGame(char* dir){
    FILE* in;
    in = fopen(dir, "rb");
    int i = 0;
    memset(memory, 0, 0x10000);
    fread(&memory[0x00], 0x8000, 1, in);
    // while(fread(&memory[0x100 + i], 1, 1, in)){
    //     i++;
    // }
    fclose(in);
}