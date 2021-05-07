#include "CPU.h"

CPU::CPU(){
    // Gameboy Initializiation Sequence (emulates bootrom)
    //     |-> Maybe move to seperate func? or call some func loadRom() before all of this
    h_PC = 0x100 ;
    set_h_AF(0x01B0);
    set_h_BC(0x0013);
    set_h_DE(0x00D8);
    set_h_HL(0x014D);
    h_SP = 0xFFFE;
    h_MEMORY.writeMemory(0xFF05, 0x00, true);
    h_MEMORY.writeMemory(0xFF06, 0x00, true);
    h_MEMORY.writeMemory(0xFF07, 0x00, true);
    h_MEMORY.writeMemory(0xFF10, 0x80, true);
    h_MEMORY.writeMemory(0xFF11, 0xBF, true);
    h_MEMORY.writeMemory(0xFF12, 0xF3, true);
    h_MEMORY.writeMemory(0xFF14, 0xBF, true);
    h_MEMORY.writeMemory(0xFF16, 0x3F, true);
    h_MEMORY.writeMemory(0xFF17, 0x00, true);
    h_MEMORY.writeMemory(0xFF19, 0xBF, true);
    h_MEMORY.writeMemory(0xFF1A, 0x7F, true);
    h_MEMORY.writeMemory(0xFF1B, 0xFF, true);
    h_MEMORY.writeMemory(0xFF1C, 0x9F, true);
    h_MEMORY.writeMemory(0xFF1E, 0xBF, true);
    h_MEMORY.writeMemory(0xFF20, 0xFF, true);
    h_MEMORY.writeMemory(0xFF21, 0x00, true);
    h_MEMORY.writeMemory(0xFF22, 0x00, true);
    h_MEMORY.writeMemory(0xFF23, 0xBF, true);
    h_MEMORY.writeMemory(0xFF24, 0x77, true);
    h_MEMORY.writeMemory(0xFF25, 0xF3, true);
    h_MEMORY.writeMemory(0xFF26, 0xF1, true);
    h_MEMORY.writeMemory(0xFF40, 0x91, true);
    h_MEMORY.writeMemory(0xFF42, 0x00, true);
    h_MEMORY.writeMemory(0xFF43, 0x00, true);
    h_MEMORY.writeMemory(0xFF45, 0x00, true);
    h_MEMORY.writeMemory(0xFF47, 0xFC, true);
    h_MEMORY.writeMemory(0xFF48, 0xFF, true);
    h_MEMORY.writeMemory(0xFF49, 0xFF, true);
    h_MEMORY.writeMemory(0xFF4A, 0x00, true);
    h_MEMORY.writeMemory(0xFF4B, 0x00, true);
    h_MEMORY.writeMemory(0xFFFF, 0x00, true);
}

CPU::CPU(char* dir){
    // FOR TESTING
    h_MEMORY.loadGame(dir);
    h_PC = 0x100 ;
    set_h_AF(0x01B0);
    set_h_BC(0x0013);
    set_h_DE(0x00D8);
    set_h_HL(0x014D);
    h_SP = 0xFFFE;
    h_MEMORY.writeMemory(0xFF05, 0x00, true);
    h_MEMORY.writeMemory(0xFF06, 0x00, true);
    h_MEMORY.writeMemory(0xFF07, 0x00, true);
    h_MEMORY.writeMemory(0xFF0F, 0xe1, true);
    h_MEMORY.writeMemory(0xFF10, 0x80, true);
    h_MEMORY.writeMemory(0xFF11, 0xBF, true);
    h_MEMORY.writeMemory(0xFF12, 0xF3, true);
    h_MEMORY.writeMemory(0xFF14, 0xBF, true);
    h_MEMORY.writeMemory(0xFF16, 0x3F, true);
    h_MEMORY.writeMemory(0xFF17, 0x00, true);
    h_MEMORY.writeMemory(0xFF19, 0xBF, true);
    h_MEMORY.writeMemory(0xFF1A, 0x7F, true);
    h_MEMORY.writeMemory(0xFF1B, 0xFF, true);
    h_MEMORY.writeMemory(0xFF1C, 0x9F, true);
    h_MEMORY.writeMemory(0xFF1E, 0xBF, true);
    h_MEMORY.writeMemory(0xFF20, 0xFF, true);
    h_MEMORY.writeMemory(0xFF21, 0x00, true);
    h_MEMORY.writeMemory(0xFF22, 0x00, true);
    h_MEMORY.writeMemory(0xFF23, 0xBF, true);
    h_MEMORY.writeMemory(0xFF24, 0x77, true);
    h_MEMORY.writeMemory(0xFF25, 0xF3, true);
    h_MEMORY.writeMemory(0xFF26, 0xF1, true);
    h_MEMORY.writeMemory(0xFF40, 0x91, true);
    h_MEMORY.writeMemory(0xFF41, 0x85, true);
    h_MEMORY.writeMemory(0xFF42, 0x00, true);
    h_MEMORY.writeMemory(0xFF43, 0x00, true);
    //h_MEMORY.writeMemory(0xFF44, 0x90, true);   /* For Testing */ 
    h_MEMORY.writeMemory(0xFF45, 0x00, true);
    h_MEMORY.writeMemory(0xFF47, 0xFC, true);
    h_MEMORY.writeMemory(0xFF48, 0xFF, true);
    h_MEMORY.writeMemory(0xFF49, 0xFF, true);
    h_MEMORY.writeMemory(0xFF4A, 0x00, true);
    h_MEMORY.writeMemory(0xFF4B, 0x00, true);
    h_MEMORY.writeMemory(0xFFFF, 0x00, true);
}

CPU::~CPU(){

}

uint8_t CPU::getOpcode(){
    uint8_t opc = h_MEMORY.readMemory(h_PC++);
    return opc;
}

// FOR TESTING OPCODES
uint16_t CPU::getPC(){
    return h_PC;
}
// REMOVE AFTER DEBUGGING


void CPU::executeOpcode(uint8_t opc, bool CB_mode /*= false*/){

    // CB mode should be true when executeOpcode is called from opc_CB

    opcode_function opc_to_execute;
    if(CB_mode){
        opc_to_execute = opcode_CB_arr[opc];
    }
    else{
        opc_to_execute = opcode_arr[opc];
    }
    (this->*opc_to_execute)(opc);
}

////////////////////////////////////////
/* Opcode Helper Function Definitions */
////////////////////////////////////////

void CPU::conditionalRelativeJump(bool condition, int8_t offset){
    if(condition)relativeJump(offset);
    else h_CYCLES = 8;
}

void CPU::relativeJump(int8_t offset){
    h_PC += offset;
    h_CYCLES = 12;
}

void CPU::conditionalPositionJump(bool condition){
    if(condition) positionJump();
    else {h_CYCLES = 12; h_PC += 2;}
}

void CPU::positionJump(){
    uint8_t lsb = h_MEMORY.readMemory(h_PC++);
    uint8_t msb = h_MEMORY.readMemory(h_PC++);
    h_PC = uint16_t((msb << 8) | lsb);
    h_CYCLES = 16;
}

void CPU::ret(){
    uint8_t lsb = h_MEMORY.readMemory(h_SP++);
    uint8_t msb = h_MEMORY.readMemory(h_SP++);
    h_PC = uint16_t((msb << 8)  | lsb);
    h_CYCLES = 16;
}

void CPU::conditionalRet(bool condition){
    if(condition){
        ret();
        h_CYCLES = 20;
    }
    else{
        h_CYCLES = 8;
    }
}

void CPU::reti(){
    ret();
    h_IME = 1;
}

void CPU::call(){
    uint8_t lsb_PC = h_MEMORY.readMemory(h_PC++);
    uint8_t msb_PC = h_MEMORY.readMemory(h_PC++);
    uint8_t lsb = uint8_t(h_PC & 0x00ff);
    uint8_t msb = uint8_t((h_PC & 0xff00) >> 8);
    h_MEMORY.writeMemory(--h_SP, msb);
    h_MEMORY.writeMemory(--h_SP, lsb);
    h_PC = uint16_t((msb_PC << 8) | lsb_PC);
    h_CYCLES = 24;
}

void CPU::conditionalCall(bool condition){
    if(condition) call();
    else {h_CYCLES = 12; h_PC += 2;}
}

void CPU::rst(uint8_t f){
    uint8_t msb = uint8_t((h_PC & 0xff00) >> 8);
    uint8_t lsb = uint8_t(h_PC & 0x00ff);
    h_MEMORY.writeMemory(--h_SP, msb);
    h_MEMORY.writeMemory(--h_SP, lsb);
    h_PC = uint16_t((0 << 8) | f);
    h_CYCLES = 16;
}

void CPU::add8(uint8_t &target, uint8_t addend){
    set_flag_N(false);
    uint8_t value = target + addend;
    set_flag_H((((target & 0xf) + (addend & 0xf)) & 0x10) != 0);
    set_flag_C(uint16_t(target + addend) > 0xff);
    set_flag_Z(value == 0);
    target = value;
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::adc8(uint8_t &target, uint8_t addend){
    set_flag_N(false);
    uint8_t value_C = uint8_t(get_flag_C());
    uint8_t value = target + addend + value_C;
    set_flag_H((((target & 0xf) + (addend & 0xf) + value_C) & 0x10) != 0);
    set_flag_C(uint16_t(target + addend + value_C) > 0xff);
    set_flag_Z(value == 0);
    target = value;
    h_CYCLES = 4;
}

void CPU::sub8(uint8_t &target, uint8_t subtrahend){
    set_flag_N(true);
    uint8_t value = target - subtrahend;
    set_flag_H((target & 0xf) < (subtrahend & 0xf));
    set_flag_C(target < subtrahend);
    set_flag_Z(value == 0);
    target = value;
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::sbc8(uint8_t &target, uint8_t subtrahend){
    set_flag_N(true);
    uint8_t value_C = uint8_t(get_flag_C());
    uint8_t value = target - subtrahend - value_C;
    set_flag_H((target & 0xf) < ((subtrahend & 0xf) + value_C));
    set_flag_C(target < (subtrahend + value_C));
    set_flag_Z(value == 0);
    target = value;
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::and8(uint8_t &target, uint8_t other){
    set_flag_N(false);
    set_flag_H(true);
    set_flag_C(false);
    target &= other;
    set_flag_Z(target == 0);
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::xor8(uint8_t &target, uint8_t other){
    set_flag_N(false);
    set_flag_H(false);
    set_flag_C(false);
    target ^= other;
    set_flag_Z(target == 0);
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::or8(uint8_t &target, uint8_t other){
    set_flag_N(false);
    set_flag_H(false);
    set_flag_C(false);
    target |= other;
    set_flag_Z(target == 0);
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::cp8(uint8_t target, uint8_t subtrahend){
    set_flag_N(true);
    uint8_t value = target - subtrahend;
    set_flag_H((target & 0xf) < (subtrahend & 0xf));
    set_flag_C(target < subtrahend);
    set_flag_Z(value == 0);
    h_CYCLES = 4; // Set to 8 seperately for edge cases
}

void CPU::increment8(uint8_t &target){
    set_flag_N(false);
    uint8_t value = target + 1;
    set_flag_Z(value == 0);
    set_flag_H((((target & 0xf) + 1) & 0x10) == 0x10);
    target = value;
    h_CYCLES = 4; // Set to 12 seperately for edge cases
}

void CPU::decrement8(uint8_t &target){
    set_flag_N(true);
    uint8_t value = target - 1;
    set_flag_Z(value == 0);
    set_flag_H((((target & 0xf) - 1) & 0x10) != 0); // Because underflow would occur if half carry is needed
    target = value;
    h_CYCLES = 4; // Set to 12 seperately for edge cases
}

/////////////////////////////////////////
/* CB-Prefixed Opcode Helper Functions */
/////////////////////////////////////////

void CPU::rlc8(uint8_t &target){
    bool carry = false;
    if((target & 0b10000000) != 0) carry = true;
    set_flag_C(carry);
    if(carry) target |= 0b00000001;
    set_flag_Z(target == 0);
    set_flag_H(false);
    set_flag_N(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::rl8(uint8_t &target){
    uint8_t bit0 = uint8_t(get_flag_C());
    set_flag_C((target & 0b10000000) != 0);
    target = (target << 1) | bit0;
    set_flag_Z(target == 0);
    set_flag_H(false);
    set_flag_N(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::rrc8(uint8_t &target){
    bool carry = false;
    if((target & 0b00000001) != 0) carry = true;
    set_flag_C(carry);
    target >>= 1;
    if(carry) target |= 0b10000000;
    set_flag_Z(target == 0);
    set_flag_H(false);
    set_flag_N(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::rr8(uint8_t &target){
    uint8_t bit7 = uint8_t(get_flag_C()) << 7;
    set_flag_C((target & 0b00000001) != 0);
    target = (target >> 1) | bit7;
    set_flag_Z(target == 0);
    set_flag_H(false);
    set_flag_N(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::sla8(uint8_t &target){
    set_flag_C((target & 0b10000000) != 0);
    target <<= 1;
    set_flag_Z(target == 0);
    set_flag_N(false);
    set_flag_H(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::sra8(uint8_t &target){
    uint8_t bit7 = target & 0b10000000;
    set_flag_C((target & 0b00000001) != 0);
    target = (target >> 1) | bit7;
    set_flag_Z(target == 0);
    set_flag_N(false);
    set_flag_H(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::swap(uint8_t &target){
    uint8_t upper = target & 0xf0;
    uint8_t lower = target & 0x0f;
    target = (lower << 8) | upper;
    set_flag_Z(target == 0);
    set_flag_N(false);
    set_flag_N(false);
    set_flag_C(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::srl8(uint8_t &target){
    set_flag_C((target & 0b00000001) != 0);
    target >>= 1;
    set_flag_Z(target == 0);
    set_flag_N(false);
    set_flag_H(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::bit8(uint8_t bit, uint8_t target){
    uint8_t mask = (1 << bit);
    set_flag_Z((target & mask) == 0); // Gets the complement of the bit
    set_flag_H(true);
    set_flag_N(false);
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::res8(uint8_t bit, uint8_t &target){
    uint8_t mask = (1 << bit) ^ 0xff; 
    target &= mask;
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

void CPU::set8(uint8_t bit, uint8_t &target){
    uint8_t mask = 1 << bit;
    target |= mask;
    h_CYCLES = 8; // Set 16 seperately for edge cases
}

////////////////////////////////////////
/* Non-CB Prefixed Opcode Definitions */
////////////////////////////////////////

void CPU::opc_00(uint8_t opc){
    // NOP - No Operation
    h_CYCLES = 4;
}
void CPU::opc_01(uint8_t opc){
    // LD BC, u16
    uint8_t lsb = h_MEMORY.readMemory(h_PC++);
    uint8_t msb = h_MEMORY.readMemory(h_PC++);
    uint16_t data = uint16_t((msb << 8) | lsb);
    set_h_BC(data);
    h_CYCLES = 12;
}
void CPU::opc_02(uint8_t opc){
    // LD (BC), h_A
    h_MEMORY.writeMemory(get_h_BC(), h_A);
    h_CYCLES = 8;
}
void CPU::opc_03(uint8_t opc){
    // INC BC
    set_h_BC(get_h_BC() + 1);
    h_CYCLES = 8;
}
void CPU::opc_04(uint8_t opc){
    // INC B
    increment8(h_B);
}
void CPU::opc_05(uint8_t opc){
    // DEC B
    decrement8(h_B);
}
void CPU::opc_06(uint8_t opc){
    // LD B, u8
    h_B = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_07(uint8_t opc){
    // RLCA
    set_flag_H(false);
    set_flag_Z(false);
    set_flag_N(false);

    bool carry = false;
    if(h_A & 0b10000000) carry = true;
    set_flag_C(carry);
    h_A <<= 1;
    if(carry) h_A |= 0b00000001;
    h_CYCLES = 4;
}
void CPU::opc_08(uint8_t opc){
    // LD (u16), SP
    uint8_t lsb = uint8_t(h_SP & 0xff);
    uint8_t msb = uint8_t((h_SP & 0xff00) >> 8);
    uint8_t lsb_PC = h_MEMORY.readMemory(h_PC++);
    uint8_t msb_PC = h_MEMORY.readMemory(h_PC++);
    uint16_t address = uint16_t((msb_PC << 8) | lsb_PC);
    h_MEMORY.writeMemory(address, lsb);
    h_MEMORY.writeMemory(address + 1, msb);
    h_CYCLES = 20;
}
void CPU::opc_09(uint8_t opc){
    // ADD HL, BC
    set_flag_N(false);
    set_flag_C(uint32_t(get_h_BC() + get_h_HL()) > 0xFFFF);
    set_flag_H((((get_h_BC() & 0x0FFF) + (get_h_HL() & 0x0FFF)) & 0x1000) == 0x1000);
    set_h_HL(get_h_HL() + get_h_BC());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;

}
void CPU::opc_0A(uint8_t opc){
    // LD A, (BC)
    h_A = h_MEMORY.readMemory(get_h_BC());
    h_CYCLES = 8;
}
void CPU::opc_0B(uint8_t opc){
    // DEC BC
    set_h_BC(get_h_BC() - 1);
    h_CYCLES = 8;
}
void CPU::opc_0C(uint8_t opc){
    // INC C
    increment8(h_C);
}
void CPU::opc_0D(uint8_t opc){
    // DEC C
    decrement8(h_C);
}
void CPU::opc_0E(uint8_t opc){
    // LD C, u8
    h_C = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_0F(uint8_t opc){
    // RRCA
    set_flag_H(false);
    set_flag_Z(false);
    set_flag_N(false);

    bool carry = false;
    if(h_A & 0b00000001) carry = true;
    set_flag_C(carry);
    h_A >>= 1;
    if(carry) h_A |= 0b10000000;
    h_CYCLES = 4;
}

void CPU::opc_10(uint8_t opc){
    // STOP -> double check if this takes 2 bytes or 1 byte
    // h_IME = false; -> check if this is true
    stopped = true;
    h_CYCLES = 4;
}
void CPU::opc_11(uint8_t opc){
    // LD DE, u16
    uint8_t lsb = h_MEMORY.readMemory(h_PC++);
    uint8_t msb = h_MEMORY.readMemory(h_PC++);
    uint16_t data = uint16_t((msb << 8) | lsb);
    set_h_DE(data);
    h_CYCLES = 12;
}
void CPU::opc_12(uint8_t opc){
    // LD (DE), A
    h_MEMORY.writeMemory(get_h_DE(), h_A);
    h_CYCLES = 8;
}
void CPU::opc_13(uint8_t opc){
    // INC DE
    set_h_DE(get_h_DE() + 1);
    h_CYCLES = 8;
}
void CPU::opc_14(uint8_t opc){
    // INC D
    increment8(h_D);
}
void CPU::opc_15(uint8_t opc){
    // DEC D
    decrement8(h_D);
}
void CPU::opc_16(uint8_t opc){
    // LD D, u8
    h_D = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_17(uint8_t opc){
    // RLA
    set_flag_H(false);
    set_flag_N(false);
    set_flag_Z(false);

    bool bit0 = get_flag_C();
    set_flag_C((h_A & 0b10000000) >> 7 != 0);
    h_A = (h_A << 1)  + uint8_t(bit0);
    h_CYCLES = 4;
}
void CPU::opc_18(uint8_t opc){
    // JR i8
    int8_t address = h_MEMORY.readMemory(h_PC++);
    h_PC = h_PC + address;
    h_CYCLES = 12;
}
void CPU::opc_19(uint8_t opc){
    // ADD HL, DE
    set_flag_N(false);
    set_flag_C(uint32_t(get_h_DE() + get_h_HL()) > 0xFFFF);
    set_flag_H(((get_h_DE() & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000) == 0x1000);
    set_h_HL(get_h_HL() + get_h_DE());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_1A(uint8_t opc){
    // LD A, (DE)
    h_A = h_MEMORY.readMemory(get_h_DE());
    h_CYCLES = 8;
}
void CPU::opc_1B(uint8_t opc){
    // DEC DE
    set_h_DE(get_h_DE() - 1);
    h_CYCLES = 8;
}
void CPU::opc_1C(uint8_t opc){
    // INC E
    increment8(h_E);
}
void CPU::opc_1D(uint8_t opc){
    // DEC E
    uint8_t value = h_E - 1;
    decrement8(h_E);
}
void CPU::opc_1E(uint8_t opc){
    // LD E, u8
    h_E = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_1F(uint8_t opc){
    // RRA
    set_flag_H(false);
    set_flag_N(false);
    set_flag_Z(false);

    uint8_t bit7 = get_flag_C() << 7;
    set_flag_C((h_A & 0b00000001) != 0);
    h_A = (h_A >> 1) | bit7;
    h_CYCLES = 4;
}

void CPU::opc_20(uint8_t opc){
    // JR NZ, i8
    bool condition = !get_flag_Z();
    int8_t offset = h_MEMORY.readMemory(h_PC++);
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_21(uint8_t opc){
    // LD HL, u16
    uint8_t lsb = h_MEMORY.readMemory(h_PC++);
    uint8_t msb = h_MEMORY.readMemory(h_PC++);
    uint16_t data = uint16_t((msb << 8) | lsb);
    set_h_HL(data);
    h_CYCLES = 12;
}
void CPU::opc_22(uint8_t opc){
    // LD (HL+), A
    h_MEMORY.writeMemory(get_h_HL(), h_A);
    set_h_HL(get_h_HL() + 1);
    h_CYCLES = 8;
}
void CPU::opc_23(uint8_t opc){
    // INC HL
    set_h_HL(get_h_HL() + 1);
    h_CYCLES = 8;
}
void CPU::opc_24(uint8_t opc){
    // INC H
    increment8(h_H);
}
void CPU::opc_25(uint8_t opc){
    // DEC H
    decrement8(h_H);
}
void CPU::opc_26(uint8_t opc){
    // LD H, u8
    h_H = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_27(uint8_t opc){
    // DAA
    if(!get_flag_N()){ // Addition occurred 
        if((h_A & 0xF) > 0x9 || get_flag_H()) h_A += 0x6;
        if(h_A > 0x99 || get_flag_C()) {h_A += 0x60; set_flag_C(true);}
    }
    else{ // Subtraction occurred
        if(get_flag_C()) h_A -= 0x60;
        if(get_flag_H()) h_A -= 0x6;
    }
    set_flag_Z(h_A == 0);
    set_flag_H(false);
    h_CYCLES = 4;
}
void CPU::opc_28(uint8_t opc){
    // JR Z, i8
    bool condition = get_flag_Z();
    int8_t offset = h_MEMORY.readMemory(h_PC++);
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_29(uint8_t opc){
    // ADD HL, HL
    set_flag_N(false);
    set_flag_C(uint32_t(get_h_HL() + get_h_HL()) > 0xFFFF);
    set_flag_H(((get_h_HL() & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000) == 0x1000);
    set_h_HL(get_h_HL() + get_h_HL());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_2A(uint8_t opc){
    // LD A, (HL+)
    h_A = h_MEMORY.readMemory(get_h_HL());
    set_h_HL(get_h_HL() + 1);
    h_CYCLES = 8;
}
void CPU::opc_2B(uint8_t opc){
    // DEC HL
    set_h_HL(get_h_HL() - 1);
    h_CYCLES = 8;
}
void CPU::opc_2C(uint8_t opc){
    // INC L
    increment8(h_L);
}
void CPU::opc_2D(uint8_t opc){
    // DEC L
    decrement8(h_L);
}
void CPU::opc_2E(uint8_t opc){
    // LD L, u8
    h_L = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_2F(uint8_t opc){
    // CPL
    set_flag_H(true);
    set_flag_N(true);

    h_A ^= 0b11111111;
    h_CYCLES = 4;
}

void CPU::opc_30(uint8_t opc){
    // JR NC, i8
    bool condition = !get_flag_C();
    int8_t offset = h_MEMORY.readMemory(h_PC++);
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_31(uint8_t opc){
    // LD SP, u16
    uint8_t lsb = h_MEMORY.readMemory(h_PC++);
    uint8_t msb = h_MEMORY.readMemory(h_PC++);
    uint16_t data = uint16_t((msb << 8) | lsb);
    h_SP = data;
    h_CYCLES = 12;
}
void CPU::opc_32(uint8_t opc){
    // LD (HL-), A
    h_MEMORY.writeMemory(get_h_HL(), h_A);
    set_h_HL(get_h_HL() - 1);
    h_CYCLES = 8;
}
void CPU::opc_33(uint8_t opc){
    // INC SP
    h_SP += 1;
    h_CYCLES = 8;
}
void CPU::opc_34(uint8_t opc){
    // INC (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    increment8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value); // -> might be redundant but i'm not sure if it'll work without this
    h_CYCLES = 12; // Correction
}
void CPU::opc_35(uint8_t opc){
    // DEC (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    decrement8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value); // -> might be redundant but i'm not sure if it'll work without this
    h_CYCLES = 12; // Correction
}
void CPU::opc_36(uint8_t opc){
    // LD (HL), u8
    h_MEMORY.writeMemory(get_h_HL(), h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 12;
}
void CPU::opc_37(uint8_t opc){
    // SCF
    set_flag_N(false);
    set_flag_H(false);
    set_flag_C(true);
    h_CYCLES = 4;
}
void CPU::opc_38(uint8_t opc){
    // JR C, s8
    bool condition = get_flag_C();
    int8_t offset = h_MEMORY.readMemory(h_PC++);
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_39(uint8_t opc){
    // ADD HL, SP
    set_flag_N(false);
    set_flag_C(uint32_t(h_SP + get_h_HL()) > 0xFFFF); // Overflow has occured (should have)
    set_flag_H(((h_SP & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000) == 0x1000);
    set_h_HL(h_SP + get_h_HL());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_3A(uint8_t opc){
    // LD A, (HL-)
    h_A = h_MEMORY.readMemory(get_h_HL());
    set_h_HL(get_h_HL() - 1);
    h_CYCLES = 8;
}
void CPU::opc_3B(uint8_t opc){
    // DEC SP
    h_SP -= 1;
    h_CYCLES = 8;
}
void CPU::opc_3C(uint8_t opc){
    // INC A
    increment8(h_A);
}
void CPU::opc_3D(uint8_t opc){
    // DEC A
    decrement8(h_A);
}
void CPU::opc_3E(uint8_t opc){
    // LD A, u8
    h_A = h_MEMORY.readMemory(h_PC++);
    h_CYCLES = 8;
}
void CPU::opc_3F(uint8_t opc){
    // CCF
    set_flag_H(false);
    set_flag_N(false);
    set_flag_C(get_flag_C() ? false : true);
    h_CYCLES = 4;
}

void CPU::opc_40(uint8_t opc){ // Is this not redundant?
    // LD B, B
    h_B = h_B;
    h_CYCLES = 4;
}
void CPU::opc_41(uint8_t opc){
    // LD B, C
    h_B = h_C;
    h_CYCLES = 4;
}
void CPU::opc_42(uint8_t opc){
    // LD B, E
    h_B = h_E;
    h_CYCLES = 4;
}
void CPU::opc_43(uint8_t opc){
    // LD B, D
    h_B = h_D;
    h_CYCLES = 4;
}
void CPU::opc_44(uint8_t opc){
    // LD B, H
    h_B = h_H;
    h_CYCLES = 4;
}
void CPU::opc_45(uint8_t opc){
    // LD B, L
    h_B = h_L;
    h_CYCLES = 4;
}
void CPU::opc_46(uint8_t opc){
    // LD B, (HL)
    h_B = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_47(uint8_t opc){
    // LD B, A
    h_B = h_A;
    h_CYCLES = 4;
}
void CPU::opc_48(uint8_t opc){
    // LD C, B
    h_C = h_B;
    h_CYCLES = 4;
}
void CPU::opc_49(uint8_t opc){
    // LD C, C
    h_C = h_C;
    h_CYCLES = 4;
}
void CPU::opc_4A(uint8_t opc){
    // LD C, D
    h_C = h_D;
    h_CYCLES = 4;
}
void CPU::opc_4B(uint8_t opc){
    // LD C, E
    h_C = h_E;
    h_CYCLES = 4;
}
void CPU::opc_4C(uint8_t opc){
    // LD C, H
    h_C = h_H;
    h_CYCLES = 4;
}
void CPU::opc_4D(uint8_t opc){
    // LD C, L
    h_C = h_L;
    h_CYCLES = 4;
}
void CPU::opc_4E(uint8_t opc){
    // LD C, (HL)
    h_C = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_4F(uint8_t opc){
    // LD C, A
    h_C = h_A;
    h_CYCLES = 4;
}

void CPU::opc_50(uint8_t opc){
    // LD D, B
    h_D = h_B;
    h_CYCLES = 4;
}
void CPU::opc_51(uint8_t opc){
    // LD D, C
    h_D = h_C;
    h_CYCLES = 4;
}
void CPU::opc_52(uint8_t opc){
    // LD D, D
    h_D = h_D;
    h_CYCLES = 4;
}
void CPU::opc_53(uint8_t opc){
    // LD D, E
    h_D = h_E;
    h_CYCLES = 4;
}
void CPU::opc_54(uint8_t opc){
    // LD D, H
    h_D = h_H;
    h_CYCLES = 4;
}
void CPU::opc_55(uint8_t opc){
    // LD D, L
    h_D = h_L;
    h_CYCLES = 4;
}
void CPU::opc_56(uint8_t opc){
    // LD D, (HL)
    h_D = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_57(uint8_t opc){
    // LD D, A
    h_D = h_A;
    h_CYCLES = 4;
}
void CPU::opc_58(uint8_t opc){
    // LD E, B
    h_E = h_B;
    h_CYCLES = 4;
}
void CPU::opc_59(uint8_t opc){
    // LD E, C
    h_E = h_C;
    h_CYCLES = 4;
}
void CPU::opc_5A(uint8_t opc){
    // LD E, D
    h_E = h_D;
    h_CYCLES = 4;
}
void CPU::opc_5B(uint8_t opc){
    // LD E, E
    h_E = h_E;
    h_CYCLES = 4;
}
void CPU::opc_5C(uint8_t opc){
    // LD E, H
    h_E = h_H;
    h_CYCLES = 4;
}
void CPU::opc_5D(uint8_t opc){
    // LD E, L
    h_E = h_L;
    h_CYCLES = 4;
}
void CPU::opc_5E(uint8_t opc){
    // LD E, (HL)
    h_E = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_5F(uint8_t opc){
    // LD E, A
    h_E = h_A;
    h_CYCLES = 4;
}

void CPU::opc_60(uint8_t opc){
    // LD H, B
    h_H = h_B;
    h_CYCLES = 4;
}
void CPU::opc_61(uint8_t opc){
    // LD H, C
    h_H = h_C;
    h_CYCLES = 4;
}
void CPU::opc_62(uint8_t opc){
    // LD H, D
    h_H = h_D;
    h_CYCLES = 4;
}
void CPU::opc_63(uint8_t opc){
    // LD H, E
    h_H = h_E;
    h_CYCLES = 4;
}
void CPU::opc_64(uint8_t opc){
    // LD H, H
    h_H = h_H;
    h_CYCLES = 4;
}
void CPU::opc_65(uint8_t opc){
    // LD H, L
    h_H = h_L;
    h_CYCLES = 4;
}
void CPU::opc_66(uint8_t opc){
    // LD H, (HL)
    h_H = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_67(uint8_t opc){
    // LD H, A
    h_H = h_A;
    h_CYCLES = 4;
}
void CPU::opc_68(uint8_t opc){
    // LD L, B
    h_L = h_B;
    h_CYCLES = 4;
}
void CPU::opc_69(uint8_t opc){
    // LD L, C
    h_L = h_C;
    h_CYCLES = 4;
}
void CPU::opc_6A(uint8_t opc){
    // LD L, D
    h_L = h_D;
    h_CYCLES = 4;
}
void CPU::opc_6B(uint8_t opc){
    // LD L, E
    h_L = h_E;
    h_CYCLES = 4;
}
void CPU::opc_6C(uint8_t opc){
    // LD L, H
    h_L = h_H;
    h_CYCLES = 4;
}
void CPU::opc_6D(uint8_t opc){
    // LD L, L
    h_L = h_L;
    h_CYCLES = 4;
}
void CPU::opc_6E(uint8_t opc){
    // LD L, (HL)
    h_L = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_6F(uint8_t opc){
    // LD L, A
    h_L = h_A;
    h_CYCLES = 4;
}

void CPU::opc_70(uint8_t opc){
    // LD (HL), B
    h_MEMORY.writeMemory(get_h_HL(), h_B);
    h_CYCLES = 8;
}
void CPU::opc_71(uint8_t opc){
    // LD (HL), C
    h_MEMORY.writeMemory(get_h_HL(), h_C);
    h_CYCLES = 8;
}
void CPU::opc_72(uint8_t opc){
    // LD (HL), D
    h_MEMORY.writeMemory(get_h_HL(), h_D);
    h_CYCLES = 8;
}
void CPU::opc_73(uint8_t opc){
    // LD (HL), E
    h_MEMORY.writeMemory(get_h_HL(), h_E);
    h_CYCLES = 8;
}
void CPU::opc_74(uint8_t opc){
    // LD (HL), H
    h_MEMORY.writeMemory(get_h_HL(), h_H);
    h_CYCLES = 8;
}
void CPU::opc_75(uint8_t opc){
    // LD (HL), L
    h_MEMORY.writeMemory(get_h_HL(), h_L);
    h_CYCLES = 8;
}
void CPU::opc_76(uint8_t opc){
    // HALT -> double check if this takes 2 bytes or 1 byte
    // h_IME = false; -> check if this is true
    halted = true;
    h_CYCLES = 4;
}
void CPU::opc_77(uint8_t opc){
    // LD (HL), A
    h_MEMORY.writeMemory(get_h_HL(), h_A);
    h_CYCLES = 8;
}
void CPU::opc_78(uint8_t opc){
    // LD A, B
    h_A = h_B;
    h_CYCLES = 4;
}
void CPU::opc_79(uint8_t opc){
    // LD A, C
    h_A = h_C;
    h_CYCLES = 4;
}
void CPU::opc_7A(uint8_t opc){
    // LD A, D
    h_A = h_D;
    h_CYCLES = 4;
}
void CPU::opc_7B(uint8_t opc){
    // LD A, E
    h_A = h_E;
    h_CYCLES = 4;
}
void CPU::opc_7C(uint8_t opc){
    // LD A, H
    h_A = h_H;
    h_CYCLES = 4;
}
void CPU::opc_7D(uint8_t opc){
    // LD A, L
    h_A = h_L;
    h_CYCLES = 4;
}
void CPU::opc_7E(uint8_t opc){
    // LD A, (HL)
    h_A = h_MEMORY.readMemory(get_h_HL());
    h_CYCLES = 8;
}
void CPU::opc_7F(uint8_t opc){
    // LD A, A
    h_A = h_A;
    h_CYCLES = 4;
}

void CPU::opc_80(uint8_t opc){
    // ADD A, B
    add8(h_A, h_B);
}
void CPU::opc_81(uint8_t opc){
    // ADD A, C
    add8(h_A, h_C);
}
void CPU::opc_82(uint8_t opc){
    // ADD A, D
    add8(h_A, h_D);
}
void CPU::opc_83(uint8_t opc){
    // ADD A, E
    add8(h_A, h_E);
}
void CPU::opc_84(uint8_t opc){
    // ADD A, H
    add8(h_A, h_H);
}
void CPU::opc_85(uint8_t opc){
    // ADD A, L
    add8(h_A, h_L);
}
void CPU::opc_86(uint8_t opc){
    // ADD A, (HL)
    add8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_87(uint8_t opc){
    // ADD A, A
    add8(h_A, h_A);
}
void CPU::opc_88(uint8_t opc){
    // ADC A, B
    adc8(h_A, h_B);
}
void CPU::opc_89(uint8_t opc){
    // ADC A, C
    adc8(h_A, h_C);
}
void CPU::opc_8A(uint8_t opc){
    // ADC A, D
    adc8(h_A, h_D);
}
void CPU::opc_8B(uint8_t opc){
    // ADC A, E
    adc8(h_A, h_E);
}
void CPU::opc_8C(uint8_t opc){
    // ADC A, H
    adc8(h_A, h_H);
}
void CPU::opc_8D(uint8_t opc){
    // ADC A, L
    adc8(h_A, h_L);
}
void CPU::opc_8E(uint8_t opc){
    // ADC A, (HL)
    adc8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_8F(uint8_t opc){
    // ADC A, A
    adc8(h_A, h_A);
}

void CPU::opc_90(uint8_t opc){
    // SUB A, B
    sub8(h_A, h_B);
}
void CPU::opc_91(uint8_t opc){
    // SUB A, C
    sub8(h_A, h_C);
}
void CPU::opc_92(uint8_t opc){
    // SUB A, D
    sub8(h_A, h_D);
}
void CPU::opc_93(uint8_t opc){
    // SUB A, E
    sub8(h_A, h_E);
}
void CPU::opc_94(uint8_t opc){
    // SUB A, H
    sub8(h_A, h_H);
}
void CPU::opc_95(uint8_t opc){
    // SUB A, L
    sub8(h_A, h_L);
}
void CPU::opc_96(uint8_t opc){
    // SUB A, (HL)
    sub8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_97(uint8_t opc){
    // SUB A, A
    sub8(h_A, h_A);
}
void CPU::opc_98(uint8_t opc){
    // SBC A, B
    sbc8(h_A, h_B);
}
void CPU::opc_99(uint8_t opc){
    // SBC A, C
    sbc8(h_A, h_C);
}
void CPU::opc_9A(uint8_t opc){
    // SBC A, D
    sbc8(h_A, h_D);
}
void CPU::opc_9B(uint8_t opc){
    // SBC A, E
    sbc8(h_A, h_E);
}
void CPU::opc_9C(uint8_t opc){
    // SBC A, H
    sbc8(h_A, h_H);
}
void CPU::opc_9D(uint8_t opc){
    // SBC A, L
    sbc8(h_A, h_L);
}
void CPU::opc_9E(uint8_t opc){
    // SBC A, (HL)
    sbc8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_9F(uint8_t opc){
    // SBC A, A
    sbc8(h_A, h_A);
}

void CPU::opc_A0(uint8_t opc){
    // AND A, B
    and8(h_A, h_B);
}
void CPU::opc_A1(uint8_t opc){
    // AND A, C
    and8(h_A, h_C);
}
void CPU::opc_A2(uint8_t opc){
    // AND A, D
    and8(h_A, h_D);
}
void CPU::opc_A3(uint8_t opc){
    // AND A, E
    and8(h_A, h_E);
}
void CPU::opc_A4(uint8_t opc){
    // AND A, H
    and8(h_A, h_H);
}
void CPU::opc_A5(uint8_t opc){
    // AND A, L
    and8(h_A, h_L);
}
void CPU::opc_A6(uint8_t opc){
    // AND A, (HL)
    and8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_A7(uint8_t opc){
    // AND A, A
    and8(h_A, h_A);
}
void CPU::opc_A8(uint8_t opc){
    // XOR A, B
    xor8(h_A, h_B);
}
void CPU::opc_A9(uint8_t opc){
    // XOR A, C
    xor8(h_A, h_C);
}
void CPU::opc_AA(uint8_t opc){
    // XOR A, D
    xor8(h_A, h_D);
}
void CPU::opc_AB(uint8_t opc){
    // XOR A, E
    xor8(h_A, h_E);
}
void CPU::opc_AC(uint8_t opc){
    // XOR A, H
    xor8(h_A, h_H);
}
void CPU::opc_AD(uint8_t opc){
    // XOR A, L
    xor8(h_A, h_L);
}
void CPU::opc_AE(uint8_t opc){
    // XOR A, (HL)
    xor8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_AF(uint8_t opc){
    // XOR A, A
    xor8(h_A, h_A);
}

void CPU::opc_B0(uint8_t opc){
    // OR A, B
    or8(h_A, h_B);
}
void CPU::opc_B1(uint8_t opc){
    // OR A, C
    or8(h_A, h_C);
}
void CPU::opc_B2(uint8_t opc){
    // OR A, D
    or8(h_A, h_D);
}
void CPU::opc_B3(uint8_t opc){
    // OR A, E
    or8(h_A, h_E);
}
void CPU::opc_B4(uint8_t opc){
    // OR A, H
    or8(h_A, h_H);
}
void CPU::opc_B5(uint8_t opc){
    // OR A, L
    or8(h_A, h_L);
}
void CPU::opc_B6(uint8_t opc){
    // OR A, (HL)
    or8(h_A, h_MEMORY.readMemory(get_h_HL()));
    h_CYCLES = 8;
}
void CPU::opc_B7(uint8_t opc){
    // OR A, (A)
    or8(h_A, h_A);
}
void CPU::opc_B8(uint8_t opc){
    // CP A, B
    cp8(h_A, h_B);
}
void CPU::opc_B9(uint8_t opc){
    // CP A, C
    cp8(h_A, h_C);
}
void CPU::opc_BA(uint8_t opc){
    // CP A, D
    cp8(h_A, h_D);
}
void CPU::opc_BB(uint8_t opc){
    // CP A, E
    cp8(h_A, h_E);
}
void CPU::opc_BC(uint8_t opc){
    // CP A, H
    cp8(h_A, h_H);
}
void CPU::opc_BD(uint8_t opc){
    // CP A, L
    cp8(h_A, h_L);
}
void CPU::opc_BE(uint8_t opc){
    // CP A, (HL)
    cp8(h_A, h_MEMORY.readMemory(get_h_HL()));
}
void CPU::opc_BF(uint8_t opc){
    // CP A, A
    cp8(h_A, h_A);
}

void CPU::opc_C0(uint8_t opc){
    // RET NZ
    conditionalRet(!get_flag_Z());
}
void CPU::opc_C1(uint8_t opc){
    // POP BC
    uint8_t lsb = h_MEMORY.readMemory(h_SP++);
    uint8_t msb = h_MEMORY.readMemory(h_SP++);
    set_h_BC(uint16_t((msb << 8) | lsb));
    h_CYCLES = 12;
}
void CPU::opc_C2(uint8_t opc){
    // JP NZ, u16
    conditionalPositionJump(!get_flag_Z());
}
void CPU::opc_C3(uint8_t opc){
    // JP u16
    positionJump();
}
void CPU::opc_C4(uint8_t opc){
    // CALL NZ, u16
    conditionalCall(!get_flag_Z());
}
void CPU::opc_C5(uint8_t opc){
    // PUSH BC
    h_MEMORY.writeMemory(--h_SP, h_B);
    h_MEMORY.writeMemory(--h_SP, h_C);
    h_CYCLES = 16;
}
void CPU::opc_C6(uint8_t opc){
    // ADD A, u8
    add8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_C7(uint8_t opc){
    // RST 0
    rst(0x00);
}
void CPU::opc_C8(uint8_t opc){
    // RET Z
    conditionalRet(get_flag_Z());
}
void CPU::opc_C9(uint8_t opc){
    // RET
    ret();
}
void CPU::opc_CA(uint8_t opc){
    // JP Z, u16
    conditionalPositionJump(get_flag_Z());
}
void CPU::opc_CB(uint8_t opc){
    // CB Prefix, get next instruction and handle as CB opcode
    // Handle cycle for this function in CB functions
    executeOpcode(getOpcode(), true);
}
void CPU::opc_CC(uint8_t opc){
    // CALL Z, u16
    conditionalCall(get_flag_Z());
}
void CPU::opc_CD(uint8_t opc){
    // CALL u16
    call();
}
void CPU::opc_CE(uint8_t opc){
    // ADC A, u8
    adc8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_CF(uint8_t opc){
    // RST 08
    rst(0x08);
}

void CPU::opc_D0(uint8_t opc){
    // RET NC
    conditionalRet(!get_flag_C());
}
void CPU::opc_D1(uint8_t opc){
    // POP DE
    uint8_t lsb = h_MEMORY.readMemory(h_SP++);
    uint8_t msb = h_MEMORY.readMemory(h_SP++);
    set_h_DE(uint16_t((msb << 8) | lsb));
    h_CYCLES = 12;
}
void CPU::opc_D2(uint8_t opc){
    // JP NC, u16
    conditionalPositionJump(!get_flag_C());
}
void CPU::opc_D3(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_D4(uint8_t opc){
    // CALL NC, u16
    conditionalCall(!get_flag_C());
}
void CPU::opc_D5(uint8_t opc){
    // PUSH DE
    h_MEMORY.writeMemory(--h_SP, h_D);
    h_MEMORY.writeMemory(--h_SP, h_E);
    h_CYCLES = 16;
}
void CPU::opc_D6(uint8_t opc){
    // SUB, A, u8
    sub8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_D7(uint8_t opc){
    // RST 10
    rst(0x10);
}
void CPU::opc_D8(uint8_t opc){
    // RET C
    conditionalRet(get_flag_C());
}
void CPU::opc_D9(uint8_t opc){
    // RETI
    reti();
}
void CPU::opc_DA(uint8_t opc){
    // JP C, u16
    conditionalPositionJump(get_flag_C());
}
void CPU::opc_DB(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_DC(uint8_t opc){
    // CALL C, u16
    conditionalCall(get_flag_C());
}
void CPU::opc_DD(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_DE(uint8_t opc){
    // SBC A, u8
    sbc8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_DF(uint8_t opc){
    // RST 18
    rst(0x18);
}

void CPU::opc_E0(uint8_t opc){
    // LD (FF00 + u8), A
    h_MEMORY.writeMemory(0xFF00 + h_MEMORY.readMemory(h_PC++), h_A);
    h_CYCLES = 12;
}
void CPU::opc_E1(uint8_t opc){
    // POP HL
    uint8_t lsb = h_MEMORY.readMemory(h_SP++);
    uint8_t msb = h_MEMORY.readMemory(h_SP++);
    set_h_HL(uint16_t((msb << 8) | lsb));
    h_CYCLES = 12;
}
void CPU::opc_E2(uint8_t opc){
    // LD (FF00 + C), A
    h_MEMORY.writeMemory(0xFF00 + uint16_t(get_flag_C()), h_A);
    h_CYCLES = 8;
}
void CPU::opc_E3(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_E4(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_E5(uint8_t opc){
    // PUSH HL
    h_MEMORY.writeMemory(--h_SP, h_H);
    h_MEMORY.writeMemory(--h_SP, h_L);
    h_CYCLES = 16;
}
void CPU::opc_E6(uint8_t opc){
    // AND A, u8
    and8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_E7(uint8_t opc){
    // RST 20
    rst(0x20);
}
void CPU::opc_E8(uint8_t opc){
    // ADD SP, i8 -> double check this one
    set_flag_N(false);
    set_flag_Z(false);
    int8_t value = h_MEMORY.readMemory(h_PC++);
    set_flag_H((((h_SP & 0xF) + (value & 0xF)) & 0x10) != 0);
    set_flag_C((((h_SP & 0xFF) + value) & 0x100) != 0);
    h_SP += value;
    h_CYCLES = 16;
}
void CPU::opc_E9(uint8_t opc){
    // JP HL
    h_PC = get_h_HL();
    h_CYCLES = 4;
}
void CPU::opc_EA(uint8_t opc){
    // LD (u16), A
    uint8_t lsb_PC = h_MEMORY.readMemory(h_PC++);
    uint8_t msb_PC = h_MEMORY.readMemory(h_PC++);
    uint16_t address = uint16_t((msb_PC << 8) | lsb_PC);
    h_MEMORY.writeMemory(address, h_A);
    h_CYCLES = 16;
}
void CPU::opc_EB(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_EC(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_ED(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_EE(uint8_t opc){
    // XOR A, u8
    xor8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_EF(uint8_t opc){
    // RST 28
    rst(0x28);
}

void CPU::opc_F0(uint8_t opc){
    // LD A, (FF00 + u8)
    h_A = h_MEMORY.readMemory(0xFF00 + h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 12;
}
void CPU::opc_F1(uint8_t opc){
    // POP AF
    uint8_t lsb = h_MEMORY.readMemory(h_SP++);
    uint8_t msb = h_MEMORY.readMemory(h_SP++);
    set_h_AF(uint16_t((msb << 8) | lsb));
    h_CYCLES = 12;
}
void CPU::opc_F2(uint8_t opc){
    // LD A, (FF00 + C)
    h_A = h_MEMORY.readMemory(0xFF00 + uint16_t(get_flag_C()));
    h_CYCLES = 8;
}
void CPU::opc_F3(uint8_t opc){
    // DI -> check later
    h_IME = 0;
    h_CYCLES = 4;
}
void CPU::opc_F4(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_F5(uint8_t opc){
    // PUSH AF
    h_MEMORY.writeMemory(--h_SP, h_A);
    h_MEMORY.writeMemory(--h_SP, h_F);
    h_CYCLES = 16;
}
void CPU::opc_F6(uint8_t opc){
    // OR A, u8
    or8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_F7(uint8_t opc){
    // RST 30
    rst(0x30);
}
void CPU::opc_F8(uint8_t opc){
    // LD HL, SP+i8 -> double check  this too
    int8_t value = h_MEMORY.readMemory(h_PC++);
    set_flag_N(false);
    set_flag_Z(false);
    set_flag_H((((h_SP & 0x0F) + (value & 0xF)) & 0x10) != 0);
    set_flag_C((((h_SP & 0xFF) + value) & 0x100) != 0);
    set_h_HL(h_SP + value);
    h_CYCLES = 12;
}
void CPU::opc_F9(uint8_t opc){
    // LD SP, HL
    h_SP = get_h_HL();
    h_CYCLES = 8;
}
void CPU::opc_FA(uint8_t opc){
    // LD A, (u16)
    uint8_t lsb_PC = h_MEMORY.readMemory(h_PC++);
    uint8_t msb_PC = h_MEMORY.readMemory(h_PC++);
    uint16_t address = uint16_t((msb_PC << 8) | lsb_PC);
    h_A = h_MEMORY.readMemory(address);
    h_CYCLES = 16;
}
void CPU::opc_FB(uint8_t opc){
    // EI -> check later
    //i_scheduled = true;
    // scheduling vs explicitly setting h_IME to true apparently has no visible effect?
    h_IME = true;
    h_CYCLES = 4;
}
void CPU::opc_FC(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_FD(uint8_t opc){} // -> empty func (I should add error if empty funcs are called?)
void CPU::opc_FE(uint8_t opc){
    // CP A, u8
    cp8(h_A, h_MEMORY.readMemory(h_PC++));
    h_CYCLES = 8;
}
void CPU::opc_FF(uint8_t opc){
    // RST 38
    rst(0x38);
}

////////////////////////////////////
/* CB Prefixed Opcode Definitions */
////////////////////////////////////

void CPU::opc_CB_00(uint8_t opc){
    // RLC B
    rlc8(h_B);
}
void CPU::opc_CB_01(uint8_t opc){
    // RLC C
    rlc8(h_C);
}
void CPU::opc_CB_02(uint8_t opc){
    // RLC D
    rlc8(h_D);
}
void CPU::opc_CB_03(uint8_t opc){
    // RLC E
    rlc8(h_E);
}
void CPU::opc_CB_04(uint8_t opc){
    // RLC H
    rlc8(h_H);
}
void CPU::opc_CB_05(uint8_t opc){
    // RLC L
    rlc8(h_L);
}
void CPU::opc_CB_06(uint8_t opc){
    // RLC (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    rlc8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_07(uint8_t opc){
    // RLC A
    rlc8(h_A);
}
void CPU::opc_CB_08(uint8_t opc){
    // RRC B
    rrc8(h_B);
}
void CPU::opc_CB_09(uint8_t opc){
    // RRC C
    rrc8(h_C);
}
void CPU::opc_CB_0A(uint8_t opc){
    // RRC D
    rrc8(h_D);
}
void CPU::opc_CB_0B(uint8_t opc){
    // RRC E
    rrc8(h_E);
}
void CPU::opc_CB_0C(uint8_t opc){
    // RRC H
    rrc8(h_H);
}
void CPU::opc_CB_0D(uint8_t opc){
    // RRC L
    rrc8(h_L);
}
void CPU::opc_CB_0E(uint8_t opc){
    // RRC (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    rrc8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_0F(uint8_t opc){
    // RRC A
    rrc8(h_A);
}

void CPU::opc_CB_10(uint8_t opc){
    // RL B
    rl8(h_B);
}
void CPU::opc_CB_11(uint8_t opc){
    // RL C
    rl8(h_C);
}
void CPU::opc_CB_12(uint8_t opc){
    // RL D
    rl8(h_D);
}
void CPU::opc_CB_13(uint8_t opc){
    // RL E
    rl8(h_E);
}
void CPU::opc_CB_14(uint8_t opc){
    // RL H
    rl8(h_H);
}
void CPU::opc_CB_15(uint8_t opc){
    // RL L
    rl8(h_L);
}
void CPU::opc_CB_16(uint8_t opc){
    // RL (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    rl8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_17(uint8_t opc){
    // RL A
    rl8(h_A);
}
void CPU::opc_CB_18(uint8_t opc){
    // RR B
    rr8(h_B);
}
void CPU::opc_CB_19(uint8_t opc){
    // RR C
    rr8(h_C);
}
void CPU::opc_CB_1A(uint8_t opc){
    // RR D
    rr8(h_D);
}
void CPU::opc_CB_1B(uint8_t opc){
    // RR E
    rr8(h_E);
}
void CPU::opc_CB_1C(uint8_t opc){
    // RR H
    rr8(h_H);
}
void CPU::opc_CB_1D(uint8_t opc){
    // RR L
    rr8(h_L);
}
void CPU::opc_CB_1E(uint8_t opc){
    // RR (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    rr8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_1F(uint8_t opc){
    // RR A
    rr8(h_A);
}

void CPU::opc_CB_20(uint8_t opc){
    // SLA B
    sla8(h_B);
}
void CPU::opc_CB_21(uint8_t opc){
    // SLA C
    sla8(h_C);
}
void CPU::opc_CB_22(uint8_t opc){
    // SLA D
    sla8(h_D);
}
void CPU::opc_CB_23(uint8_t opc){
    // SLA E
    sla8(h_E);
}
void CPU::opc_CB_24(uint8_t opc){
    // SLA H
    sla8(h_H);
}
void CPU::opc_CB_25(uint8_t opc){
    // SLA L
    sla8(h_L);
}
void CPU::opc_CB_26(uint8_t opc){
    // SLA (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    sla8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_27(uint8_t opc){
    // SLA A
    sla8(h_A);
}
void CPU::opc_CB_28(uint8_t opc){
    // SRA B
    sra8(h_B);
}
void CPU::opc_CB_29(uint8_t opc){
    // SRA C
    sra8(h_C);
}
void CPU::opc_CB_2A(uint8_t opc){
    // SRA D
    sra8(h_D);
}
void CPU::opc_CB_2B(uint8_t opc){
    // SRA E
    sra8(h_E);
}
void CPU::opc_CB_2C(uint8_t opc){
    // SRA H
    sra8(h_H);
}
void CPU::opc_CB_2D(uint8_t opc){
    // SRA L
    sra8(h_L);
}
void CPU::opc_CB_2E(uint8_t opc){
    // SRA (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    sra8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_2F(uint8_t opc){
    // SRA A
    sra8(h_A);
}

void CPU::opc_CB_30(uint8_t opc){
    // SWAP B
    swap(h_B);
}
void CPU::opc_CB_31(uint8_t opc){
    // SWAP C
    swap(h_C);
}
void CPU::opc_CB_32(uint8_t opc){
    // SWAP D
    swap(h_D);
}
void CPU::opc_CB_33(uint8_t opc){
    // SWAP E
    swap(h_E);
}
void CPU::opc_CB_34(uint8_t opc){
    // SWAP H
    swap(h_H);
}
void CPU::opc_CB_35(uint8_t opc){
    // SWAP L
    swap(h_L);
}
void CPU::opc_CB_36(uint8_t opc){
    // SWAP (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    swap(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_37(uint8_t opc){
    // SWAP A
    swap(h_A);
}
void CPU::opc_CB_38(uint8_t opc){
    // SRL B
    srl8(h_B);
}
void CPU::opc_CB_39(uint8_t opc){
    // SRL C
    srl8(h_C);
}
void CPU::opc_CB_3A(uint8_t opc){
    // SRL D
    srl8(h_D);
}
void CPU::opc_CB_3B(uint8_t opc){
    // SRL E
    srl8(h_E);
}
void CPU::opc_CB_3C(uint8_t opc){
    // SRL H
    srl8(h_H);
}
void CPU::opc_CB_3D(uint8_t opc){
    // SRL L
    srl8(h_L);
}
void CPU::opc_CB_3E(uint8_t opc){
    // SRL (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    srl8(value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_3F(uint8_t opc){
    // SRL A
    srl8(h_A);
}

void CPU::opc_CB_40(uint8_t opc){
    // BIT 0, B
    bit8(0, h_B);
}
void CPU::opc_CB_41(uint8_t opc){
    // BIT 0, C
    bit8(0, h_C);
}
void CPU::opc_CB_42(uint8_t opc){
    // BIT 0, D
    bit8(0, h_D);
}
void CPU::opc_CB_43(uint8_t opc){
    // BIT 0, E
    bit8(0, h_E);
}
void CPU::opc_CB_44(uint8_t opc){
    // BIT 0, H
    bit8(0, h_H);
}
void CPU::opc_CB_45(uint8_t opc){
    // BIT 0, L
    bit8(0, h_L);
}
void CPU::opc_CB_46(uint8_t opc){
    // BIT 0, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(0, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_47(uint8_t opc){
    // BIT 0, A
    bit8(0, h_A);
}
void CPU::opc_CB_48(uint8_t opc){
    // BIT 1, B
    bit8(1, h_B);
}
void CPU::opc_CB_49(uint8_t opc){
    // BIT 1, C
    bit8(1, h_C);
}
void CPU::opc_CB_4A(uint8_t opc){
    // BIT 1, D
    bit8(1, h_D);
}
void CPU::opc_CB_4B(uint8_t opc){
    // BIT 1, E
    bit8(1, h_E);
}
void CPU::opc_CB_4C(uint8_t opc){
    // BIT 1, H
    bit8(1, h_H);
}
void CPU::opc_CB_4D(uint8_t opc){
    // BIT 1, L
    bit8(1, h_L);
}
void CPU::opc_CB_4E(uint8_t opc){
    // BIT 1, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(1, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_4F(uint8_t opc){
    // BIT 1, A
    bit8(1, h_A);
}

void CPU::opc_CB_50(uint8_t opc){
    // BIT 2, B
    bit8(2, h_B);
}
void CPU::opc_CB_51(uint8_t opc){
    // BIT 2, C
    bit8(2, h_C);
}
void CPU::opc_CB_52(uint8_t opc){
    // BIT 2, D
    bit8(2, h_D);
}
void CPU::opc_CB_53(uint8_t opc){
    // BIT 2, E
    bit8(2, h_E);
}
void CPU::opc_CB_54(uint8_t opc){
    // BIT 2, H
    bit8(2, h_H);
}
void CPU::opc_CB_55(uint8_t opc){
    // BIT 2, L
    bit8(2, h_L);
}
void CPU::opc_CB_56(uint8_t opc){
    // BIT 2, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(2, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_57(uint8_t opc){
    // BIT 2, A
    bit8(2, h_A);
}
void CPU::opc_CB_58(uint8_t opc){
    // BIT 3, B
    bit8(3, h_B);
}
void CPU::opc_CB_59(uint8_t opc){
    // BIT 3, C
    bit8(3, h_C);
}
void CPU::opc_CB_5A(uint8_t opc){
    // BIT 3, D
    bit8(3, h_D);
}
void CPU::opc_CB_5B(uint8_t opc){
    // BIT 3, E
    bit8(3, h_E);
}
void CPU::opc_CB_5C(uint8_t opc){
    // BIT 3, H
    bit8(3, h_H);
}
void CPU::opc_CB_5D(uint8_t opc){
    // BIT 3, L
    bit8(3, h_L);
}
void CPU::opc_CB_5E(uint8_t opc){
    // BIT 3, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(3, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_5F(uint8_t opc){
    // BIT 3, A
    bit8(3, h_A);
}

void CPU::opc_CB_60(uint8_t opc){
    // BIT 4, B
    bit8(4, h_B);
}
void CPU::opc_CB_61(uint8_t opc){
    // BIT 4, C
    bit8(4, h_C);
}
void CPU::opc_CB_62(uint8_t opc){
    // BIT 4, D
    bit8(4, h_D);
}
void CPU::opc_CB_63(uint8_t opc){
    // BIT 4, E
    bit8(4, h_E);
}
void CPU::opc_CB_64(uint8_t opc){
    // BIT 4, H
    bit8(4, h_H);
}
void CPU::opc_CB_65(uint8_t opc){
    // BIT 4, L
    bit8(4, h_L);
}
void CPU::opc_CB_66(uint8_t opc){
    // BIT 4, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(4, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_67(uint8_t opc){
    // BIT 4, A
    bit8(4, h_A);
}
void CPU::opc_CB_68(uint8_t opc){
    // BIT 5, B
    bit8(5, h_B);
}
void CPU::opc_CB_69(uint8_t opc){
    // BIT 5, C
    bit8(5, h_C);
}
void CPU::opc_CB_6A(uint8_t opc){
    // BIT 5, D
    bit8(5, h_D);
}
void CPU::opc_CB_6B(uint8_t opc){
    // BIT 5, E
    bit8(5, h_E);
}
void CPU::opc_CB_6C(uint8_t opc){
    // BIT 5, H
    bit8(5, h_H);
}
void CPU::opc_CB_6D(uint8_t opc){
    // BIT 5, L
    bit8(5, h_L);
}
void CPU::opc_CB_6E(uint8_t opc){
    // BIT 5, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(5, value);
   //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_6F(uint8_t opc){
    // BIT 5, A
    bit8(5, h_A);
}

void CPU::opc_CB_70(uint8_t opc){
    // BIT 6, B
    bit8(6, h_B);
}
void CPU::opc_CB_71(uint8_t opc){
    // BIT 6, C
    bit8(6, h_C);
}
void CPU::opc_CB_72(uint8_t opc){
    // BIT 6, D
    bit8(6, h_D);
}
void CPU::opc_CB_73(uint8_t opc){
    // BIT 6, E
    bit8(6, h_E);
}
void CPU::opc_CB_74(uint8_t opc){
    // BIT 6, H
    bit8(6, h_H);
}
void CPU::opc_CB_75(uint8_t opc){
    // BIT 6, L
    bit8(6, h_L);
}
void CPU::opc_CB_76(uint8_t opc){
    // BIT 6, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(6, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_77(uint8_t opc){
    // BIT 6, A
    bit8(6, h_A);
}
void CPU::opc_CB_78(uint8_t opc){
    // BIT 7, B
    bit8(7, h_B);
}
void CPU::opc_CB_79(uint8_t opc){
    // BIT 7, C
    bit8(7, h_C);
}
void CPU::opc_CB_7A(uint8_t opc){
    // BIT 7, D
    bit8(7, h_D);
}
void CPU::opc_CB_7B(uint8_t opc){
    // BIT 7, E
    bit8(7, h_E);
}
void CPU::opc_CB_7C(uint8_t opc){
    // BIT 7, H
    bit8(7, h_H);
}
void CPU::opc_CB_7D(uint8_t opc){
    // BIT 7, L
    bit8(7, h_L);
}
void CPU::opc_CB_7E(uint8_t opc){
    // BIT 7, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    bit8(7, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_7F(uint8_t opc){
    // BIT 7, A
    bit8(7, h_A);
}

void CPU::opc_CB_80(uint8_t opc){
    // RES 0, B
    res8(0, h_B);
}
void CPU::opc_CB_81(uint8_t opc){
    // RES 0, C
    res8(0, h_C);
}
void CPU::opc_CB_82(uint8_t opc){
    // RES 0, D
    res8(0, h_D);
}
void CPU::opc_CB_83(uint8_t opc){
    // RES 0, E
    res8(0, h_E);
}
void CPU::opc_CB_84(uint8_t opc){
    // RES 0, H
    res8(0, h_H);
}
void CPU::opc_CB_85(uint8_t opc){
    // RES 0, L
    res8(0, h_L);
}
void CPU::opc_CB_86(uint8_t opc){
    // RES 0, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(0, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_87(uint8_t opc){
    // RES 0, A
    res8(0, h_A);
}
void CPU::opc_CB_88(uint8_t opc){
    // RES 1, B
    res8(1, h_B);
}
void CPU::opc_CB_89(uint8_t opc){
    // RES 1, C
    res8(1, h_C);
}
void CPU::opc_CB_8A(uint8_t opc){
    // RES 1, D
    res8(1, h_D);
}
void CPU::opc_CB_8B(uint8_t opc){
    // RES 1, E
    res8(1, h_E);
}
void CPU::opc_CB_8C(uint8_t opc){
    // RES 1, H
    res8(1, h_H);
}
void CPU::opc_CB_8D(uint8_t opc){
    // RES 1, L
    res8(1, h_L);
}
void CPU::opc_CB_8E(uint8_t opc){
    // RES 1, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(1, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_8F(uint8_t opc){
    // RES 1, A
    res8(1, h_A);
}

void CPU::opc_CB_90(uint8_t opc){
    // RES 2, B
    res8(2, h_B);
}
void CPU::opc_CB_91(uint8_t opc){
    // RES 2, C
    res8(2, h_C);
}
void CPU::opc_CB_92(uint8_t opc){
    // RES 2, D
    res8(2, h_D);
}
void CPU::opc_CB_93(uint8_t opc){
    // RES 2, E
    res8(2, h_E);
}
void CPU::opc_CB_94(uint8_t opc){
    // RES 2, H
    res8(2, h_H);
}
void CPU::opc_CB_95(uint8_t opc){
    // RES 2, L
    res8(2, h_L);
}
void CPU::opc_CB_96(uint8_t opc){
    // RES 2, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(2, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_97(uint8_t opc){
    // RES 2, A
    res8(2, h_A);
}
void CPU::opc_CB_98(uint8_t opc){
    // RES 3, B
    res8(3, h_B);
}
void CPU::opc_CB_99(uint8_t opc){
    // RES 3, C
    res8(3, h_C);
}
void CPU::opc_CB_9A(uint8_t opc){
    // RES 3, D
    res8(3, h_D);
}
void CPU::opc_CB_9B(uint8_t opc){
    // RES 3, E
    res8(3, h_E);
}
void CPU::opc_CB_9C(uint8_t opc){
    // RES 3, H
    res8(3, h_H);
}
void CPU::opc_CB_9D(uint8_t opc){
    // RES 3, L
    res8(3, h_L);
}
void CPU::opc_CB_9E(uint8_t opc){
    // RES 3, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(3, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_9F(uint8_t opc){
    // RES 3, A
    res8(3, h_A);
}

void CPU::opc_CB_A0(uint8_t opc){
    // RES 4, B
    res8(4, h_B);
}
void CPU::opc_CB_A1(uint8_t opc){
    // RES 4, C
    res8(4, h_C);
}
void CPU::opc_CB_A2(uint8_t opc){
    // RES 4, D
    res8(4, h_D);
}
void CPU::opc_CB_A3(uint8_t opc){
    // RES 4, E
    res8(4, h_E);
}
void CPU::opc_CB_A4(uint8_t opc){
    // RES 4, H
    res8(4, h_H);
}
void CPU::opc_CB_A5(uint8_t opc){
    // RES 4, L
    res8(4, h_L);
}
void CPU::opc_CB_A6(uint8_t opc){
    // RES 4, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(4, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_A7(uint8_t opc){
    // RES 4, A
    res8(4, h_A);
}
void CPU::opc_CB_A8(uint8_t opc){
    // RES 5, B
    res8(5, h_B);
}
void CPU::opc_CB_A9(uint8_t opc){
    // RES 5, C
    res8(5, h_C);
}
void CPU::opc_CB_AA(uint8_t opc){
    // RES 5, D
    res8(5, h_D);
}
void CPU::opc_CB_AB(uint8_t opc){
    // RES 5, E
    res8(5, h_E);
}
void CPU::opc_CB_AC(uint8_t opc){
    // RES 5, H
    res8(5, h_H);
}
void CPU::opc_CB_AD(uint8_t opc){
    // RES 5, L
    res8(5, h_L);
}
void CPU::opc_CB_AE(uint8_t opc){
    // RES 5, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(5, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_AF(uint8_t opc){
    // RES 5, A
    res8(5, h_A);
}

void CPU::opc_CB_B0(uint8_t opc){
    // RES 6, B
    res8(6, h_B);
}
void CPU::opc_CB_B1(uint8_t opc){
    // RES 6, C
    res8(6, h_C);
}
void CPU::opc_CB_B2(uint8_t opc){
    // RES 6, D
    res8(6, h_D);
}
void CPU::opc_CB_B3(uint8_t opc){
    // RES 6, E
    res8(6, h_E);
}
void CPU::opc_CB_B4(uint8_t opc){
    // RES 6, H
    res8(6, h_H);
}
void CPU::opc_CB_B5(uint8_t opc){
    // RES 6, L
    res8(6, h_L);
}
void CPU::opc_CB_B6(uint8_t opc){
    // RES 6, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(6, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_B7(uint8_t opc){
    // RES 6, A
    res8(6, h_A);
}
void CPU::opc_CB_B8(uint8_t opc){
    // RES 7, B
    res8(7, h_B);
}
void CPU::opc_CB_B9(uint8_t opc){
    // RES 7, C
    res8(7, h_C);
}
void CPU::opc_CB_BA(uint8_t opc){
    // RES 7, D
    res8(7, h_D);
}
void CPU::opc_CB_BB(uint8_t opc){
    // RES 7, E
    res8(7, h_E);
}
void CPU::opc_CB_BC(uint8_t opc){
    // RES 7, H
    res8(7, h_H);
}
void CPU::opc_CB_BD(uint8_t opc){
    // RES 7, L
    res8(7, h_L);
}
void CPU::opc_CB_BE(uint8_t opc){
    // RES 7, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    res8(7, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_BF(uint8_t opc){
    // RES 7, A
    res8(7, h_A);
}

void CPU::opc_CB_C0(uint8_t opc){
    // SET 0, B
    set8(0, h_B);
}
void CPU::opc_CB_C1(uint8_t opc){
    // SET 0, C
    set8(0, h_C);
}
void CPU::opc_CB_C2(uint8_t opc){
    // SET 0, D
    set8(0, h_D);
}
void CPU::opc_CB_C3(uint8_t opc){
    // SET 0, E
    set8(0, h_E);
}
void CPU::opc_CB_C4(uint8_t opc){
    // SET 0, H
    set8(0, h_H);
}
void CPU::opc_CB_C5(uint8_t opc){
    // SET 0, L
    set8(0, h_L);
}
void CPU::opc_CB_C6(uint8_t opc){
    // SET 0, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(0, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_C7(uint8_t opc){
    // SET 0, A
    set8(0, h_A);
}
void CPU::opc_CB_C8(uint8_t opc){
    // SET 1, B
    set8(1, h_B);
}
void CPU::opc_CB_C9(uint8_t opc){
    // SET 1, C
    set8(1, h_C);
}
void CPU::opc_CB_CA(uint8_t opc){
    // SET 1, D
    set8(1, h_D);
}
void CPU::opc_CB_CB(uint8_t opc){
    // SET 1, E
    set8(1, h_E);
}
void CPU::opc_CB_CC(uint8_t opc){
    // SET 1, H
    set8(1, h_H);
}
void CPU::opc_CB_CD(uint8_t opc){
    // SET 1, L
    set8(1, h_L);
}
void CPU::opc_CB_CE(uint8_t opc){
    // SET 1, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(1, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_CF(uint8_t opc){
    // SET 1, A
    set8(1, h_A);
}

void CPU::opc_CB_D0(uint8_t opc){
    // SET 2, B
    set8(2, h_B);
}
void CPU::opc_CB_D1(uint8_t opc){
    // SET 2, C
    set8(2, h_C);
}
void CPU::opc_CB_D2(uint8_t opc){
    // SET 2, D
    set8(2, h_D);
}
void CPU::opc_CB_D3(uint8_t opc){
    // SET 2, E
    set8(2, h_E);
}
void CPU::opc_CB_D4(uint8_t opc){
    // SET 2, H
    set8(2, h_H);
}
void CPU::opc_CB_D5(uint8_t opc){
    // SET 2, L
    set8(2, h_L);
}
void CPU::opc_CB_D6(uint8_t opc){
    // SET 2, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(2, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_D7(uint8_t opc){
    // SET 2, A
    set8(2, h_A);
}
void CPU::opc_CB_D8(uint8_t opc){
    // SET 3, B
    set8(3, h_B);
}
void CPU::opc_CB_D9(uint8_t opc){
    // SET 3, C
    set8(3, h_C);
}
void CPU::opc_CB_DA(uint8_t opc){
    // SET 3, D
    set8(3, h_D);
}
void CPU::opc_CB_DB(uint8_t opc){
    // SET 3, E
    set8(3, h_E);
}
void CPU::opc_CB_DC(uint8_t opc){
    // SET 3, H
    set8(3, h_H);
}
void CPU::opc_CB_DD(uint8_t opc){
    // SET 3, L
    set8(3, h_L);
}
void CPU::opc_CB_DE(uint8_t opc){
    // SET 3, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(3, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_DF(uint8_t opc){
    // SET 3, A
    set8(3, h_A);
}

void CPU::opc_CB_E0(uint8_t opc){
    // SET 4, B
    set8(4, h_B);
}
void CPU::opc_CB_E1(uint8_t opc){
    // SET 4, C
    set8(4, h_C);
}
void CPU::opc_CB_E2(uint8_t opc){
    // SET 4, D
    set8(4, h_D);
}
void CPU::opc_CB_E3(uint8_t opc){
    // SET 4, E
    set8(4, h_E);
}
void CPU::opc_CB_E4(uint8_t opc){
    // SET 4, H
    set8(4, h_H);
}
void CPU::opc_CB_E5(uint8_t opc){
    // SET 4, L
    set8(4, h_L);
}
void CPU::opc_CB_E6(uint8_t opc){
    // SET 4, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(4, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_E7(uint8_t opc){
    // SET 4, A
    set8(4, h_A);
}
void CPU::opc_CB_E8(uint8_t opc){
    // SET 5, B
    set8(5, h_B);
}
void CPU::opc_CB_E9(uint8_t opc){
    // SET 5, C
    set8(5, h_C);
}
void CPU::opc_CB_EA(uint8_t opc){
    // SET 5, D
    set8(5, h_D);
}
void CPU::opc_CB_EB(uint8_t opc){
    // SET 5, E
    set8(5, h_E);
}
void CPU::opc_CB_EC(uint8_t opc){
    // SET 5, H
    set8(5, h_H);
}
void CPU::opc_CB_ED(uint8_t opc){
    // SET 5, L
    set8(5, h_L);
}
void CPU::opc_CB_EE(uint8_t opc){
    // SET 5, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(5, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_EF(uint8_t opc){
    // SET 5, A
    set8(5, h_A);
}

void CPU::opc_CB_F0(uint8_t opc){
    // SET 6, B
    set8(6, h_B);
}
void CPU::opc_CB_F1(uint8_t opc){
    // SET 6, C
    set8(6, h_C);
}
void CPU::opc_CB_F2(uint8_t opc){
    // SET 6, D
    set8(6, h_D);
}
void CPU::opc_CB_F3(uint8_t opc){
    // SET 6, E
    set8(6, h_E);
}
void CPU::opc_CB_F4(uint8_t opc){
    // SET 6, H
    set8(6, h_H);
}
void CPU::opc_CB_F5(uint8_t opc){
    // SET 6, L
    set8(6, h_L);
}
void CPU::opc_CB_F6(uint8_t opc){
    // SET 6, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(6, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_F7(uint8_t opc){
    // SET 6, A
    set8(6, h_A);
}
void CPU::opc_CB_F8(uint8_t opc){
    // SET 7, B
    set8(7, h_B);
}
void CPU::opc_CB_F9(uint8_t opc){
    // SET 7, C
    set8(7, h_C);
}
void CPU::opc_CB_FA(uint8_t opc){
    // SET 7, D
    set8(7, h_D);
}
void CPU::opc_CB_FB(uint8_t opc){
    // SET 7, E
    set8(7, h_E);
}
void CPU::opc_CB_FC(uint8_t opc){
    // SET 7, H
    set8(7, h_H);
}
void CPU::opc_CB_FD(uint8_t opc){
    // SET 7, L
    set8(7, h_L);
}
void CPU::opc_CB_FE(uint8_t opc){
    // SET 7, (HL)
    uint8_t &value = h_MEMORY.readMemory(get_h_HL());
    set8(7, value);
    //h_MEMORY.writeMemory(get_h_HL(), value);
    h_CYCLES = 16;
}
void CPU::opc_CB_FF(uint8_t opc){
    // SET 7, A
    set8(7, h_A);
}

void CPU::handleInput(uint8_t key, bool pressed){
    if(pressed){
        bool newlySet = false;
        switch (key){
            case 0:{ // Right
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000001);
                h_MEMORY.memory[0xFF00] &= 0b11111110; 
                break; 
            } 
            case 1:{ // Left
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000010);
                h_MEMORY.memory[0xFF00] &= 0b11111101; 
                break; 
            } 
            case 2:{ // Up
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000100);
                h_MEMORY.memory[0xFF00] &= 0b11111011; 
                break; 
            } 
            case 3:{ // Down
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00001000);
                h_MEMORY.memory[0xFF00] &= 0b11110111; 
                break; 
            } 
            case 4:{ // A
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000001);
                h_MEMORY.memory[0xFF00] &= 0b11111110; 
                break; 
            } 
            case 5:{ // B
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000010);
                h_MEMORY.memory[0xFF00] &= 0b11111101; 
                break; 
            } 
            case 6:{ // Select
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00000100);
                h_MEMORY.memory[0xFF00] &= 0b11111011; 
                break; 
            } 
            case 7:{ // Start
                newlySet = (h_MEMORY.memory[0xFF00] & 0b00001000);
                h_MEMORY.memory[0xFF00] &= 0b11110111; 
                break; 
            } 
        }
        bool interruptNeeded = false;
        if((key <= 3) && !(h_MEMORY.memory[0xFF00] & 0b00010000)) interruptNeeded = true;
        else if((key > 3) && !(h_MEMORY.memory[0xFF00] & 0b00100000)) interruptNeeded = true;
        if(interruptNeeded && newlySet) interrupts->requestInterrupt(4);
    }
    else{
        switch (key){
            case 0: h_MEMORY.memory[0xFF00] |= 0b00000001; break;  // Right
            case 1: h_MEMORY.memory[0xFF00] |= 0b00000010; break;  // Left
            case 2: h_MEMORY.memory[0xFF00] |= 0b00000100; break;  // Up
            case 3: h_MEMORY.memory[0xFF00] |= 0b00001000; break;  // Down
            case 4: h_MEMORY.memory[0xFF00] |= 0b00000001; break;  // A
            case 5: h_MEMORY.memory[0xFF00] |= 0b00000010; break;  // B
            case 6: h_MEMORY.memory[0xFF00] |= 0b00000100; break;  // Select
            case 7: h_MEMORY.memory[0xFF00] |= 0b00001000; break;  // Start
        }
    }
}