#include "Memory.h"

uint8_t &Memory::readMemory(uint16_t address){
    return memory[address];
}

void Memory::writeMemory(uint16_t address, uint8_t data){
    if(address >= 0xFF80 && address <= 0xFFFE){
        // unusable
    }
    memory[address] = data;
}