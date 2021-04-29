#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

class Memory{

    public:
        uint8_t readMemory(uint16_t address);
        void writeMemory(uint16_t address, uint8_t data);
        uint8_t h_MEMORY[0x10000];
    private:
        //friend class CPU;
};