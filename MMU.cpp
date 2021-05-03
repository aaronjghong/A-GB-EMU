#include "MMU.h"

#include <iostream>
#include <stdio.h>

uint8_t &MMU::readMemory(uint16_t address){
    switch (address){
        
        // case 0xFF13: /* Write Only */ break;
        // case 0xFF14: return memory[address] & 0b01111000; break;
        // case 0xFF16: return memory[address] & 0b01111000; break;
        // case 0xFF1E: return memory[address] & 0b01111000; break;
        // case 0xFF23: return memory[address] & 0b01111111; break;
        // case 0xFF46: /* Write Only */ break;
        
        
        default: return memory[address]; break;
    }
}

void MMU::writeMemory(uint16_t address, uint8_t data, bool init /* = false */){
    if(!init){
        switch (address){
            case 0xFF00: memory[address] = (memory[address] & 0b11001111) | (data & 0b00110000); break;
            case 0xFF02: memory[address] = (memory[address] & 0b01111100) | (data & 0b10000011); break;
            case 0xFF04: memory[address] = 0;                                                    break;
            case 0xFF07: memory[address] = (memory[address] & 0b11111000) | (data & 0b00000111); break;
            case 0xFF0F: memory[address] = (memory[address] & 0b11100000) | (data & 0b00011111); break;
            case 0xFF10: memory[address] = (memory[address] & 0b10000000) | (data & 0b01111111); break;
            case 0xFF1A: memory[address] = (memory[address] & 0b01111111) | (data & 0b10000000); break;
            case 0xFF1C: memory[address] = (memory[address] & 0b10011111) | (data & 0b01100000); break;
            case 0xFF21: memory[address] = (memory[address] & 0b11000000) | (data & 0b00111111); break;
            case 0xFF23: memory[address] = (memory[address] & 0b00111111) | (data & 0b11000000); break;
            case 0xFF26: memory[address] = (memory[address] & 0b01111111) | (data & 0b10000000); break;
            case 0xFF41: memory[address] = (memory[address] & 0b10000111) | (data & 0b01111000); break;
            case 0xFF44: memory[address] = 0;                                                    break;
            case 0xFF46: doDMATransfer(data);                                                    break;
            case 0xFFFF: memory[address] = (memory[address] & 0b11100000) | (data & 0b00011111); break;

            default: memory[address] = data; break;
        }
    }
    else{
        memory[address] = data;
    }

    if(address >= 0xFF80 && address <= 0xFFFE){
        // unusable
    }

    if(address == 0xFF41){
        data &= 0b01111111;
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

void MMU::doDMATransfer(uint8_t data){
    uint16_t address = data << 8;
    for(int i = 0; i < 0xA0; i++){
        memory[0xFE00 + i] = memory[address + i];
    }
}