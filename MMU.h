#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>

class MMU{

    public:
        void loadGame(char* dir);
        uint8_t &readMemory(uint16_t address);
        void writeMemory(uint16_t address, uint8_t data, bool init = false);
        uint8_t memory[0x10000];
    private:
        void doDMATransfer(uint8_t data);

        // Memory Map
        // 0000-3FFF 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
        // 4000-7FFF 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
        // 8000-9FFF 8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
        // A000-BFFF 8KB External RAM (in cartridge, switchable bank, if any)
        // C000-CFFF 4KB Work RAM Bank 0 (WRAM)
        // D000-DFFF 4KB Work RAM Bank 1 (WRAM) (switchable bank 1-7 in CGB Mode)
        // E000-FDFF Same as C000-DDFF (ECHO) (typically not used)
        // FE00-FE9F Sprite Attribute Table (OAM)
        // FEA0-FEFF Not Usable
        // FF00-FF7F I/O Ports
        // FF80-FFFE High RAM (HRAM)
        // FFFF Interrupt Enable Register
};