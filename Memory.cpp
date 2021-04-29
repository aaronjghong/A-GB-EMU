#include "Memory.h"

uint8_t Memory::readMemory(uint16_t address){
    return h_MEMORY[address];
}

void Memory::writeMemory(uint16_t address, uint8_t data){
    h_MEMORY[address] = data;
}