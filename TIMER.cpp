#include "TIMER.h"

TIMER::TIMER(uint8_t* FF04, INTERRUPT *interrupts)
    :DIV(FF04[0]),
    TIMA(FF04[1]),
    TMA(FF04[2]),
    TAC(FF04[3])
{
    r_TIMERS = FF04;
    interrupt = interrupts;
}

void TIMER::updateTimers(uint8_t cycles){
    // Divider Register is always Updated
    updateDividerRegister(cycles);

    if(isEnabled()){
        t_counter -= cycles;
        if(t_counter <= 0){
            // Reset t_counter
            setFrequency();
        }
        if(TIMA == 255){
            // Overflow will occur this step, set TIMA to TMA and request timer interrupt
            TIMA = TMA;
            interrupt->requestInterrupt(2);
        }
        else{
            TIMA++;
        }
    }
}

void TIMER::updateDividerRegister(uint8_t cycles){

    DIV_counter += cycles;
    if(DIV_counter >= 255){
        // "Hidden lower 8 bits" of DIV have overflowed, update "upper 8 bits" of DIV which is mapped to $FF04
        DIV_counter = 0;
        DIV++;
    }
}

bool TIMER::isEnabled(){
    if((TAC & 0b00000100) != 0){
        return true;
    }
    else{
        return false;
    }
}

void TIMER::setFrequency(){
    switch (TAC & 0b00000011){
        case 0b00:  t_counter = 1024; break;
        case 0b01:  t_counter = 16;   break;
        case 0b10:  t_counter = 64;   break;
        case 0b11:  t_counter = 256;  break;
    }   
}