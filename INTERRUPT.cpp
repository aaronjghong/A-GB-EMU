#include "INTERRUPT.h"

INTERRUPT::INTERRUPT(uint8_t* zero_page, MMU& memory, uint16_t& h_PC, uint16_t& h_SP){
    FF00 = zero_page;
    cpu_MEMORY = memory;
    cpu_PC = h_PC;
    cpu_SP = h_SP;
}

INTERRUPT::~INTERRUPT(){

}

void INTERRUPT::requestInterrupt(int interrupt){
    switch (interrupt){
        case 0: IF |= 0b00000001; break;    // V-Blank
        case 1: IF |= 0b00000010; break;    // LCD STAT
        case 2: IF |= 0b00000100; break;    // TIMER
        case 3: IF |= 0b00001000; break;    // SERIAL
        case 4: IF |= 0b00010000; break;    // JOYPAD
    }
}

void INTERRUPT::doInterrupts(){
    if(IME == true){
        // Interrupts can be performed
        if(IF != 0){
            for(int i = 0; i < 4; i++){
                if(((IF >> i) & 0b00000001) != 0){
                    if(((IE >> i) & 0b00000001) != 0){
                        // Interrupt detected, enabled and must be serviced
                        serviceInterrupt(i);
                    }
                }
            }
        }
    }
}
void INTERRUPT::serviceInterrupt(int interrupt){
    // Disable interrupts, disable interrupt request bit, push PC and jump to appropriate interrupt address
    IME = false;
    uint8_t mask = (1 << interrupt)^0b11111111;
    IF &= mask;
    uint8_t msb_PC = (cpu_PC & 0xFF00) >> 8;
    uint8_t lsb_PC = cpu_PC & 0xFF;
    
    cpu_MEMORY.writeMemory(--cpu_SP, msb_PC);
    cpu_MEMORY.writeMemory(--cpu_SP, lsb_PC);

    switch (interrupt){
        case 0: cpu_PC = 0x40; break;    // V-Blank
        case 1: cpu_PC = 0x48; break;    // LCD STAT
        case 2: cpu_PC = 0x50; break;    // TIMER
        case 3: cpu_PC = 0x58; break;    // SERIAL
        case 4: cpu_PC = 0x60; break;    // JOYPAD
    }
}