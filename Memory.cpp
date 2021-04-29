#include "Memory.h"

uint8_t Memory::readMemory(uint16_t address){
    return memory[address];
}

void Memory::writeMemory(uint16_t address, uint8_t data){
    memory[address] = data;
}