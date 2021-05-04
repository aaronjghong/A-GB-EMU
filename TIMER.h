#pragma once

#include <iostream>
#include "INTERRUPT.h"

#define CLOCKSPEED 4194304;
//#define DIV r_TIMERS[0];
// #define TIMA r_TIMERS[1];
// #define TMA r_TIMERS[2];
// #define TAC r_TIMERS[3];

class TIMER{
    public:
        TIMER(uint8_t* FF04, INTERRUPT *interrupts);
        ~TIMER();
        void updateTimers(uint8_t cycles);
    private:
        int t_counter = 1024;
        int t_frequency;
        uint8_t* r_TIMERS;

        void updateDividerRegister(uint8_t cycles);
        void setFrequency();
        bool isEnabled();

        int DIV_counter = 0;

        uint8_t &DIV;// = r_TIMERS[0];
        uint8_t &TIMA;// = r_TIMERS[1];
        uint8_t &TMA;// = r_TIMERS[2];
        uint8_t &TAC;// = r_TIMERS[3];

        INTERRUPT *interrupt;
};