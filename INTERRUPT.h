#pragma once

#include <iostream>
#include "MMU.h"

class INTERRUPT{
    public:
        INTERRUPT(uint8_t* zero_page, MMU& memory, uint16_t& h_PC, uint16_t& h_SP);
        ~INTERRUPT();
        void requestInterrupt(int interrupt);
        void doInterrupts();
        bool IME = false;
    private:
        uint8_t* FF00;
        uint8_t &IE;
        uint8_t &IF;
        uint16_t cpu_PC;
        uint16_t cpu_SP;
        MMU cpu_MEMORY;
        void serviceInterrupt(int interrupt);
};