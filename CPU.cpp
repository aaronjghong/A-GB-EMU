#include "CPU.h"

CPU::CPU(){

}

CPU::~CPU(){

}

uint8_t CPU::getOpcode(){
    uint8_t opc = h_MEMORY[h_PC++];
    return opc;
}


void CPU::executeOpcode(uint8_t opc, bool CB_mode = false){

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
    if(condition){
        relativeJump(offset);
    }
    else{
        h_CYCLES = 8;
    }
}
void CPU::relativeJump(int8_t offset){
    h_PC += offset;
    h_CYCLES = 12;
}

void CPU::add8(uint8_t target, uint8_t addend){}

void CPU::increment8(uint8_t &target){
    set_flag_N(false);
    uint8_t value = target + 1;
    set_flag_Z(value == 0);
    if(((target & 0xf) + 1) & 0x10 == 0x10) set_flag_H(true);
    target = value;
    h_CYCLES = 4;
}

void CPU::decrement8(uint8_t &target){
    set_flag_N(true);
    uint8_t value = target - 1;
    set_flag_Z(value == 0);
    if(((target & 0x10) - 1) & 0x10 == 0) set_flag_H(true);
    target = value;
    h_CYCLES = 4;
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
    uint16_t data = (h_MEMORY[h_PC++] << 8) | h_MEMORY[h_PC++];
    set_h_BC(data);
    h_CYCLES = 12;
}
void CPU::opc_02(uint8_t opc){
    // LD BC, u8 -> Revisit?
    h_MEMORY[get_h_BC()] = h_A;
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
    h_B = h_MEMORY[h_PC++];
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
    h_SP = (h_MEMORY[h_PC++] << 8) | h_MEMORY[h_PC++];
    h_CYCLES = 20;
}
void CPU::opc_09(uint8_t opc){
    // ADD HL, BC
    set_flag_N(false);
    if((get_h_BC() + get_h_HL() & 0x10000 == 0x10000)) set_flag_C(true);
    if(((get_h_BC() & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000 == 0x1000)) set_flag_H(true);
    set_h_HL(get_h_HL() + get_h_BC());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;

}
void CPU::opc_0A(uint8_t opc){
    // LD A, (BC)
    h_A = h_MEMORY[get_h_BC()];
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
    h_C = h_MEMORY[h_PC++];
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
    uint16_t data = (h_MEMORY[h_PC++] << 8) | h_MEMORY[h_PC++];
    set_h_DE(data);
    h_CYCLES = 12;
}
void CPU::opc_12(uint8_t opc){
    // LD (DE), A
    h_MEMORY[get_h_DE()] = h_A;
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
    h_D = h_MEMORY[h_PC++];
    h_CYCLES = 8;
}
void CPU::opc_17(uint8_t opc){
    // RLA
    set_flag_H(false);
    set_flag_N(false);
    set_flag_Z(false);

    bool bit0 = get_flag_C();
    set_flag_C((h_A & 0b10000000) >> 7 != 0);
    h_A = h_A << 1  + uint8_t(bit0);
    h_CYCLES = 4;
}
void CPU::opc_18(uint8_t opc){
    // JR i8
    int8_t address = h_MEMORY[h_PC++];
    h_PC = h_PC + address;
    h_CYCLES = 12;
}
void CPU::opc_19(uint8_t opc){
    // ADD HL, DE
    set_flag_N(false);
    if((get_h_DE() + get_h_HL() & 0x10000 == 0x10000)) set_flag_C(true);
    if(((get_h_DE() & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000 == 0x1000)) set_flag_H(true);
    set_h_HL(get_h_HL() + get_h_DE());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_1A(uint8_t opc){
    // LD A, (DE)
    h_A = h_MEMORY[get_h_DE()];
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
    h_E = h_MEMORY[h_PC++];
    h_CYCLES = 8;
}
void CPU::opc_1F(uint8_t opc){
    // RRA
    set_flag_H(false);
    set_flag_N(false);
    set_flag_Z(false);

    bool bit7 = get_flag_C();
    set_flag_C((h_A & 0b00000001) != 0);
    h_A = (h_A >> 1) | bit7;
    h_CYCLES = 4;
}

void CPU::opc_20(uint8_t opc){
    // JR NZ, i8
    bool condition = !get_flag_Z();
    int8_t offset = h_MEMORY[h_PC++];
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_21(uint8_t opc){
    // LD HL, u16
    uint16_t data = (h_MEMORY[h_PC++] << 8) | h_MEMORY[h_PC++];
    set_h_HL(data);
    h_CYCLES = 12;
}
void CPU::opc_22(uint8_t opc){
    // LD (HL+), A
    h_MEMORY[get_h_HL()] = h_A;
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
    h_H = h_MEMORY[h_PC++];
    h_CYCLES = 8;
}
void CPU::opc_27(uint8_t opc){
    // DAA
    if(!get_flag_N){ // Addition occurred 
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
    int8_t offset = h_MEMORY[h_PC++];
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_29(uint8_t opc){
    // ADD HL, HL
    set_flag_N(false);
    if((get_h_HL() + get_h_HL() & 0x10000 == 0x10000)) set_flag_C(true);
    if(((get_h_HL() & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000 == 0x1000)) set_flag_H(true);
    set_h_HL(get_h_HL() + get_h_HL());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_2A(uint8_t opc){
    // LD A, (HL+)
    h_A = h_MEMORY[get_h_HL()];
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
    h_L = h_MEMORY[h_PC++];
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
    bool condition = get_flag_C();
    int8_t offset = h_MEMORY[h_PC++];
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_31(uint8_t opc){
    // LD SP, u16
    uint16_t data = (h_MEMORY[h_PC++] << 8) | h_MEMORY[h_PC++];
    h_SP = data;
    h_CYCLES = 12;
}
void CPU::opc_32(uint8_t opc){
    // LD (HL-), A
    h_MEMORY[get_h_HL()] = h_A;
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
    increment8(h_MEMORY[get_h_HL()]);
}
void CPU::opc_35(uint8_t opc){
    // DEC (HL)
    decrement8(h_MEMORY[get_h_HL()]);
}
void CPU::opc_36(uint8_t opc){
    // LD (HL), u8
    h_MEMORY[get_h_HL()] = h_MEMORY[h_PC++];
    h_CYCLES = 8;
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
    bool condition = get_flag_C;
    int8_t offset = h_MEMORY[h_PC++];
    conditionalRelativeJump(condition, offset);
}
void CPU::opc_39(uint8_t opc){
    // ADD HL, SP
    set_flag_N(false);
    if((h_SP + get_h_HL() & 0x10000 == 0x10000)) set_flag_C(true);
    if(((h_SP & 0x0FFF) + (get_h_HL() & 0x0FFF) & 0x1000 == 0x1000)) set_flag_H(true);
    set_h_HL(h_SP + get_h_HL());  // Check if overflow or not (for sanity)
    h_CYCLES = 8;
}
void CPU::opc_3A(uint8_t opc){
    // LD A, (HL-)
    h_A = h_MEMORY[get_h_HL()];
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
    h_A = h_MEMORY[h_PC++];
    h_CYCLES = 8;
}
void CPU::opc_3F(uint8_t opc){
    set_flag_H(false);
    set_flag_N(false);
    set_flag_C(int(get_flag_C())^1);
    h_CYCLES = 4;
}

void CPU::opc_40(uint8_t opc){}
void CPU::opc_41(uint8_t opc){}
void CPU::opc_42(uint8_t opc){}
void CPU::opc_43(uint8_t opc){}
void CPU::opc_44(uint8_t opc){}
void CPU::opc_45(uint8_t opc){}
void CPU::opc_46(uint8_t opc){}
void CPU::opc_47(uint8_t opc){}
void CPU::opc_48(uint8_t opc){}
void CPU::opc_49(uint8_t opc){}
void CPU::opc_4A(uint8_t opc){}
void CPU::opc_4B(uint8_t opc){}
void CPU::opc_4C(uint8_t opc){}
void CPU::opc_4D(uint8_t opc){}
void CPU::opc_4E(uint8_t opc){}
void CPU::opc_4F(uint8_t opc){}

void CPU::opc_50(uint8_t opc){}
void CPU::opc_51(uint8_t opc){}
void CPU::opc_52(uint8_t opc){}
void CPU::opc_53(uint8_t opc){}
void CPU::opc_54(uint8_t opc){}
void CPU::opc_55(uint8_t opc){}
void CPU::opc_56(uint8_t opc){}
void CPU::opc_57(uint8_t opc){}
void CPU::opc_58(uint8_t opc){}
void CPU::opc_59(uint8_t opc){}
void CPU::opc_5A(uint8_t opc){}
void CPU::opc_5B(uint8_t opc){}
void CPU::opc_5C(uint8_t opc){}
void CPU::opc_5D(uint8_t opc){}
void CPU::opc_5E(uint8_t opc){}
void CPU::opc_5F(uint8_t opc){}

void CPU::opc_60(uint8_t opc){}
void CPU::opc_61(uint8_t opc){}
void CPU::opc_62(uint8_t opc){}
void CPU::opc_63(uint8_t opc){}
void CPU::opc_64(uint8_t opc){}
void CPU::opc_65(uint8_t opc){}
void CPU::opc_66(uint8_t opc){}
void CPU::opc_67(uint8_t opc){}
void CPU::opc_68(uint8_t opc){}
void CPU::opc_69(uint8_t opc){}
void CPU::opc_6A(uint8_t opc){}
void CPU::opc_6B(uint8_t opc){}
void CPU::opc_6C(uint8_t opc){}
void CPU::opc_6D(uint8_t opc){}
void CPU::opc_6E(uint8_t opc){}
void CPU::opc_6F(uint8_t opc){}

void CPU::opc_70(uint8_t opc){}
void CPU::opc_71(uint8_t opc){}
void CPU::opc_72(uint8_t opc){}
void CPU::opc_73(uint8_t opc){}
void CPU::opc_74(uint8_t opc){}
void CPU::opc_75(uint8_t opc){}
void CPU::opc_76(uint8_t opc){}
void CPU::opc_77(uint8_t opc){}
void CPU::opc_78(uint8_t opc){}
void CPU::opc_79(uint8_t opc){}
void CPU::opc_7A(uint8_t opc){}
void CPU::opc_7B(uint8_t opc){}
void CPU::opc_7C(uint8_t opc){}
void CPU::opc_7D(uint8_t opc){}
void CPU::opc_7E(uint8_t opc){}
void CPU::opc_7F(uint8_t opc){}

void CPU::opc_80(uint8_t opc){}
void CPU::opc_81(uint8_t opc){}
void CPU::opc_82(uint8_t opc){}
void CPU::opc_83(uint8_t opc){}
void CPU::opc_84(uint8_t opc){}
void CPU::opc_85(uint8_t opc){}
void CPU::opc_86(uint8_t opc){}
void CPU::opc_87(uint8_t opc){}
void CPU::opc_88(uint8_t opc){}
void CPU::opc_89(uint8_t opc){}
void CPU::opc_8A(uint8_t opc){}
void CPU::opc_8B(uint8_t opc){}
void CPU::opc_8C(uint8_t opc){}
void CPU::opc_8D(uint8_t opc){}
void CPU::opc_8E(uint8_t opc){}
void CPU::opc_8F(uint8_t opc){}

void CPU::opc_90(uint8_t opc){}
void CPU::opc_91(uint8_t opc){}
void CPU::opc_92(uint8_t opc){}
void CPU::opc_93(uint8_t opc){}
void CPU::opc_94(uint8_t opc){}
void CPU::opc_95(uint8_t opc){}
void CPU::opc_96(uint8_t opc){}
void CPU::opc_97(uint8_t opc){}
void CPU::opc_98(uint8_t opc){}
void CPU::opc_99(uint8_t opc){}
void CPU::opc_9A(uint8_t opc){}
void CPU::opc_9B(uint8_t opc){}
void CPU::opc_9C(uint8_t opc){}
void CPU::opc_9D(uint8_t opc){}
void CPU::opc_9E(uint8_t opc){}
void CPU::opc_9F(uint8_t opc){}

void CPU::opc_A0(uint8_t opc){}
void CPU::opc_A1(uint8_t opc){}
void CPU::opc_A2(uint8_t opc){}
void CPU::opc_A3(uint8_t opc){}
void CPU::opc_A4(uint8_t opc){}
void CPU::opc_A5(uint8_t opc){}
void CPU::opc_A6(uint8_t opc){}
void CPU::opc_A7(uint8_t opc){}
void CPU::opc_A8(uint8_t opc){}
void CPU::opc_A9(uint8_t opc){}
void CPU::opc_AA(uint8_t opc){}
void CPU::opc_AB(uint8_t opc){}
void CPU::opc_AC(uint8_t opc){}
void CPU::opc_AD(uint8_t opc){}
void CPU::opc_AE(uint8_t opc){}
void CPU::opc_AF(uint8_t opc){}

void CPU::opc_B0(uint8_t opc){}
void CPU::opc_B1(uint8_t opc){}
void CPU::opc_B2(uint8_t opc){}
void CPU::opc_B3(uint8_t opc){}
void CPU::opc_B4(uint8_t opc){}
void CPU::opc_B5(uint8_t opc){}
void CPU::opc_B6(uint8_t opc){}
void CPU::opc_B7(uint8_t opc){}
void CPU::opc_B8(uint8_t opc){}
void CPU::opc_B9(uint8_t opc){}
void CPU::opc_BA(uint8_t opc){}
void CPU::opc_BB(uint8_t opc){}
void CPU::opc_BC(uint8_t opc){}
void CPU::opc_BD(uint8_t opc){}
void CPU::opc_BE(uint8_t opc){}
void CPU::opc_BF(uint8_t opc){}

void CPU::opc_C0(uint8_t opc){}
void CPU::opc_C1(uint8_t opc){}
void CPU::opc_C2(uint8_t opc){}
void CPU::opc_C3(uint8_t opc){}
void CPU::opc_C4(uint8_t opc){}
void CPU::opc_C5(uint8_t opc){}
void CPU::opc_C6(uint8_t opc){}
void CPU::opc_C7(uint8_t opc){}
void CPU::opc_C8(uint8_t opc){}
void CPU::opc_C9(uint8_t opc){}
void CPU::opc_CA(uint8_t opc){}
void CPU::opc_CB(uint8_t opc){}
void CPU::opc_CC(uint8_t opc){}
void CPU::opc_CD(uint8_t opc){}
void CPU::opc_CE(uint8_t opc){}
void CPU::opc_CF(uint8_t opc){}

void CPU::opc_D0(uint8_t opc){}
void CPU::opc_D1(uint8_t opc){}
void CPU::opc_D2(uint8_t opc){}
void CPU::opc_D3(uint8_t opc){}
void CPU::opc_D4(uint8_t opc){}
void CPU::opc_D5(uint8_t opc){}
void CPU::opc_D6(uint8_t opc){}
void CPU::opc_D7(uint8_t opc){}
void CPU::opc_D8(uint8_t opc){}
void CPU::opc_D9(uint8_t opc){}
void CPU::opc_DA(uint8_t opc){}
void CPU::opc_DB(uint8_t opc){}
void CPU::opc_DC(uint8_t opc){}
void CPU::opc_DD(uint8_t opc){}
void CPU::opc_DE(uint8_t opc){}
void CPU::opc_DF(uint8_t opc){}

void CPU::opc_E0(uint8_t opc){}
void CPU::opc_E1(uint8_t opc){}
void CPU::opc_E2(uint8_t opc){}
void CPU::opc_E3(uint8_t opc){}
void CPU::opc_E4(uint8_t opc){}
void CPU::opc_E5(uint8_t opc){}
void CPU::opc_E6(uint8_t opc){}
void CPU::opc_E7(uint8_t opc){}
void CPU::opc_E8(uint8_t opc){}
void CPU::opc_E9(uint8_t opc){}
void CPU::opc_EA(uint8_t opc){}
void CPU::opc_EB(uint8_t opc){}
void CPU::opc_EC(uint8_t opc){}
void CPU::opc_ED(uint8_t opc){}
void CPU::opc_EE(uint8_t opc){}
void CPU::opc_EF(uint8_t opc){}

void CPU::opc_F0(uint8_t opc){}
void CPU::opc_F1(uint8_t opc){}
void CPU::opc_F2(uint8_t opc){}
void CPU::opc_F3(uint8_t opc){}
void CPU::opc_F4(uint8_t opc){}
void CPU::opc_F5(uint8_t opc){}
void CPU::opc_F6(uint8_t opc){}
void CPU::opc_F7(uint8_t opc){}
void CPU::opc_F8(uint8_t opc){}
void CPU::opc_F9(uint8_t opc){}
void CPU::opc_FA(uint8_t opc){}
void CPU::opc_FB(uint8_t opc){}
void CPU::opc_FC(uint8_t opc){}
void CPU::opc_FD(uint8_t opc){}
void CPU::opc_FE(uint8_t opc){}
void CPU::opc_FF(uint8_t opc){}

////////////////////////////////////
/* CB Prefixed Opcode Definitions */
////////////////////////////////////

void CPU::opc_CB_00(uint8_t opc){}
void CPU::opc_CB_01(uint8_t opc){}
void CPU::opc_CB_02(uint8_t opc){}
void CPU::opc_CB_03(uint8_t opc){}
void CPU::opc_CB_04(uint8_t opc){}
void CPU::opc_CB_05(uint8_t opc){}
void CPU::opc_CB_06(uint8_t opc){}
void CPU::opc_CB_07(uint8_t opc){}
void CPU::opc_CB_08(uint8_t opc){}
void CPU::opc_CB_09(uint8_t opc){}
void CPU::opc_CB_0A(uint8_t opc){}
void CPU::opc_CB_0B(uint8_t opc){}
void CPU::opc_CB_0C(uint8_t opc){}
void CPU::opc_CB_0D(uint8_t opc){}
void CPU::opc_CB_0E(uint8_t opc){}
void CPU::opc_CB_0F(uint8_t opc){}

void CPU::opc_CB_10(uint8_t opc){}
void CPU::opc_CB_11(uint8_t opc){}
void CPU::opc_CB_12(uint8_t opc){}
void CPU::opc_CB_13(uint8_t opc){}
void CPU::opc_CB_14(uint8_t opc){}
void CPU::opc_CB_15(uint8_t opc){}
void CPU::opc_CB_16(uint8_t opc){}
void CPU::opc_CB_17(uint8_t opc){}
void CPU::opc_CB_18(uint8_t opc){}
void CPU::opc_CB_19(uint8_t opc){}
void CPU::opc_CB_1A(uint8_t opc){}
void CPU::opc_CB_1B(uint8_t opc){}
void CPU::opc_CB_1C(uint8_t opc){}
void CPU::opc_CB_1D(uint8_t opc){}
void CPU::opc_CB_1E(uint8_t opc){}
void CPU::opc_CB_1F(uint8_t opc){}

void CPU::opc_CB_20(uint8_t opc){}
void CPU::opc_CB_21(uint8_t opc){}
void CPU::opc_CB_22(uint8_t opc){}
void CPU::opc_CB_23(uint8_t opc){}
void CPU::opc_CB_24(uint8_t opc){}
void CPU::opc_CB_25(uint8_t opc){}
void CPU::opc_CB_26(uint8_t opc){}
void CPU::opc_CB_27(uint8_t opc){}
void CPU::opc_CB_28(uint8_t opc){}
void CPU::opc_CB_29(uint8_t opc){}
void CPU::opc_CB_2A(uint8_t opc){}
void CPU::opc_CB_2B(uint8_t opc){}
void CPU::opc_CB_2C(uint8_t opc){}
void CPU::opc_CB_2D(uint8_t opc){}
void CPU::opc_CB_2E(uint8_t opc){}
void CPU::opc_CB_2F(uint8_t opc){}

void CPU::opc_CB_30(uint8_t opc){}
void CPU::opc_CB_31(uint8_t opc){}
void CPU::opc_CB_32(uint8_t opc){}
void CPU::opc_CB_33(uint8_t opc){}
void CPU::opc_CB_34(uint8_t opc){}
void CPU::opc_CB_35(uint8_t opc){}
void CPU::opc_CB_36(uint8_t opc){}
void CPU::opc_CB_37(uint8_t opc){}
void CPU::opc_CB_38(uint8_t opc){}
void CPU::opc_CB_39(uint8_t opc){}
void CPU::opc_CB_3A(uint8_t opc){}
void CPU::opc_CB_3B(uint8_t opc){}
void CPU::opc_CB_3C(uint8_t opc){}
void CPU::opc_CB_3D(uint8_t opc){}
void CPU::opc_CB_3E(uint8_t opc){}
void CPU::opc_CB_3F(uint8_t opc){}

void CPU::opc_CB_40(uint8_t opc){}
void CPU::opc_CB_41(uint8_t opc){}
void CPU::opc_CB_42(uint8_t opc){}
void CPU::opc_CB_43(uint8_t opc){}
void CPU::opc_CB_44(uint8_t opc){}
void CPU::opc_CB_45(uint8_t opc){}
void CPU::opc_CB_46(uint8_t opc){}
void CPU::opc_CB_47(uint8_t opc){}
void CPU::opc_CB_48(uint8_t opc){}
void CPU::opc_CB_49(uint8_t opc){}
void CPU::opc_CB_4A(uint8_t opc){}
void CPU::opc_CB_4B(uint8_t opc){}
void CPU::opc_CB_4C(uint8_t opc){}
void CPU::opc_CB_4D(uint8_t opc){}
void CPU::opc_CB_4E(uint8_t opc){}
void CPU::opc_CB_4F(uint8_t opc){}

void CPU::opc_CB_50(uint8_t opc){}
void CPU::opc_CB_51(uint8_t opc){}
void CPU::opc_CB_52(uint8_t opc){}
void CPU::opc_CB_53(uint8_t opc){}
void CPU::opc_CB_54(uint8_t opc){}
void CPU::opc_CB_55(uint8_t opc){}
void CPU::opc_CB_56(uint8_t opc){}
void CPU::opc_CB_57(uint8_t opc){}
void CPU::opc_CB_58(uint8_t opc){}
void CPU::opc_CB_59(uint8_t opc){}
void CPU::opc_CB_5A(uint8_t opc){}
void CPU::opc_CB_5B(uint8_t opc){}
void CPU::opc_CB_5C(uint8_t opc){}
void CPU::opc_CB_5D(uint8_t opc){}
void CPU::opc_CB_5E(uint8_t opc){}
void CPU::opc_CB_5F(uint8_t opc){}

void CPU::opc_CB_60(uint8_t opc){}
void CPU::opc_CB_61(uint8_t opc){}
void CPU::opc_CB_62(uint8_t opc){}
void CPU::opc_CB_63(uint8_t opc){}
void CPU::opc_CB_64(uint8_t opc){}
void CPU::opc_CB_65(uint8_t opc){}
void CPU::opc_CB_66(uint8_t opc){}
void CPU::opc_CB_67(uint8_t opc){}
void CPU::opc_CB_68(uint8_t opc){}
void CPU::opc_CB_69(uint8_t opc){}
void CPU::opc_CB_6A(uint8_t opc){}
void CPU::opc_CB_6B(uint8_t opc){}
void CPU::opc_CB_6C(uint8_t opc){}
void CPU::opc_CB_6D(uint8_t opc){}
void CPU::opc_CB_6E(uint8_t opc){}
void CPU::opc_CB_6F(uint8_t opc){}

void CPU::opc_CB_70(uint8_t opc){}
void CPU::opc_CB_71(uint8_t opc){}
void CPU::opc_CB_72(uint8_t opc){}
void CPU::opc_CB_73(uint8_t opc){}
void CPU::opc_CB_74(uint8_t opc){}
void CPU::opc_CB_75(uint8_t opc){}
void CPU::opc_CB_76(uint8_t opc){}
void CPU::opc_CB_77(uint8_t opc){}
void CPU::opc_CB_78(uint8_t opc){}
void CPU::opc_CB_79(uint8_t opc){}
void CPU::opc_CB_7A(uint8_t opc){}
void CPU::opc_CB_7B(uint8_t opc){}
void CPU::opc_CB_7C(uint8_t opc){}
void CPU::opc_CB_7D(uint8_t opc){}
void CPU::opc_CB_7E(uint8_t opc){}
void CPU::opc_CB_7F(uint8_t opc){}

void CPU::opc_CB_80(uint8_t opc){}
void CPU::opc_CB_81(uint8_t opc){}
void CPU::opc_CB_82(uint8_t opc){}
void CPU::opc_CB_83(uint8_t opc){}
void CPU::opc_CB_84(uint8_t opc){}
void CPU::opc_CB_85(uint8_t opc){}
void CPU::opc_CB_86(uint8_t opc){}
void CPU::opc_CB_87(uint8_t opc){}
void CPU::opc_CB_88(uint8_t opc){}
void CPU::opc_CB_89(uint8_t opc){}
void CPU::opc_CB_8A(uint8_t opc){}
void CPU::opc_CB_8B(uint8_t opc){}
void CPU::opc_CB_8C(uint8_t opc){}
void CPU::opc_CB_8D(uint8_t opc){}
void CPU::opc_CB_8E(uint8_t opc){}
void CPU::opc_CB_8F(uint8_t opc){}

void CPU::opc_CB_90(uint8_t opc){}
void CPU::opc_CB_91(uint8_t opc){}
void CPU::opc_CB_92(uint8_t opc){}
void CPU::opc_CB_93(uint8_t opc){}
void CPU::opc_CB_94(uint8_t opc){}
void CPU::opc_CB_95(uint8_t opc){}
void CPU::opc_CB_96(uint8_t opc){}
void CPU::opc_CB_97(uint8_t opc){}
void CPU::opc_CB_98(uint8_t opc){}
void CPU::opc_CB_99(uint8_t opc){}
void CPU::opc_CB_9A(uint8_t opc){}
void CPU::opc_CB_9B(uint8_t opc){}
void CPU::opc_CB_9C(uint8_t opc){}
void CPU::opc_CB_9D(uint8_t opc){}
void CPU::opc_CB_9E(uint8_t opc){}
void CPU::opc_CB_9F(uint8_t opc){}

void CPU::opc_CB_A0(uint8_t opc){}
void CPU::opc_CB_A1(uint8_t opc){}
void CPU::opc_CB_A2(uint8_t opc){}
void CPU::opc_CB_A3(uint8_t opc){}
void CPU::opc_CB_A4(uint8_t opc){}
void CPU::opc_CB_A5(uint8_t opc){}
void CPU::opc_CB_A6(uint8_t opc){}
void CPU::opc_CB_A7(uint8_t opc){}
void CPU::opc_CB_A8(uint8_t opc){}
void CPU::opc_CB_A9(uint8_t opc){}
void CPU::opc_CB_AA(uint8_t opc){}
void CPU::opc_CB_AB(uint8_t opc){}
void CPU::opc_CB_AC(uint8_t opc){}
void CPU::opc_CB_AD(uint8_t opc){}
void CPU::opc_CB_AE(uint8_t opc){}
void CPU::opc_CB_AF(uint8_t opc){}

void CPU::opc_CB_B0(uint8_t opc){}
void CPU::opc_CB_B1(uint8_t opc){}
void CPU::opc_CB_B2(uint8_t opc){}
void CPU::opc_CB_B3(uint8_t opc){}
void CPU::opc_CB_B4(uint8_t opc){}
void CPU::opc_CB_B5(uint8_t opc){}
void CPU::opc_CB_B6(uint8_t opc){}
void CPU::opc_CB_B7(uint8_t opc){}
void CPU::opc_CB_B8(uint8_t opc){}
void CPU::opc_CB_B9(uint8_t opc){}
void CPU::opc_CB_BA(uint8_t opc){}
void CPU::opc_CB_BB(uint8_t opc){}
void CPU::opc_CB_BC(uint8_t opc){}
void CPU::opc_CB_BD(uint8_t opc){}
void CPU::opc_CB_BE(uint8_t opc){}
void CPU::opc_CB_BF(uint8_t opc){}

void CPU::opc_CB_C0(uint8_t opc){}
void CPU::opc_CB_C1(uint8_t opc){}
void CPU::opc_CB_C2(uint8_t opc){}
void CPU::opc_CB_C3(uint8_t opc){}
void CPU::opc_CB_C4(uint8_t opc){}
void CPU::opc_CB_C5(uint8_t opc){}
void CPU::opc_CB_C6(uint8_t opc){}
void CPU::opc_CB_C7(uint8_t opc){}
void CPU::opc_CB_C8(uint8_t opc){}
void CPU::opc_CB_C9(uint8_t opc){}
void CPU::opc_CB_CA(uint8_t opc){}
void CPU::opc_CB_CB(uint8_t opc){}
void CPU::opc_CB_CC(uint8_t opc){}
void CPU::opc_CB_CD(uint8_t opc){}
void CPU::opc_CB_CE(uint8_t opc){}
void CPU::opc_CB_CF(uint8_t opc){}

void CPU::opc_CB_D0(uint8_t opc){}
void CPU::opc_CB_D1(uint8_t opc){}
void CPU::opc_CB_D2(uint8_t opc){}
void CPU::opc_CB_D3(uint8_t opc){}
void CPU::opc_CB_D4(uint8_t opc){}
void CPU::opc_CB_D5(uint8_t opc){}
void CPU::opc_CB_D6(uint8_t opc){}
void CPU::opc_CB_D7(uint8_t opc){}
void CPU::opc_CB_D8(uint8_t opc){}
void CPU::opc_CB_D9(uint8_t opc){}
void CPU::opc_CB_DA(uint8_t opc){}
void CPU::opc_CB_DB(uint8_t opc){}
void CPU::opc_CB_DC(uint8_t opc){}
void CPU::opc_CB_DD(uint8_t opc){}
void CPU::opc_CB_DE(uint8_t opc){}
void CPU::opc_CB_DF(uint8_t opc){}

void CPU::opc_CB_E0(uint8_t opc){}
void CPU::opc_CB_E1(uint8_t opc){}
void CPU::opc_CB_E2(uint8_t opc){}
void CPU::opc_CB_E3(uint8_t opc){}
void CPU::opc_CB_E4(uint8_t opc){}
void CPU::opc_CB_E5(uint8_t opc){}
void CPU::opc_CB_E6(uint8_t opc){}
void CPU::opc_CB_E7(uint8_t opc){}
void CPU::opc_CB_E8(uint8_t opc){}
void CPU::opc_CB_E9(uint8_t opc){}
void CPU::opc_CB_EA(uint8_t opc){}
void CPU::opc_CB_EB(uint8_t opc){}
void CPU::opc_CB_EC(uint8_t opc){}
void CPU::opc_CB_ED(uint8_t opc){}
void CPU::opc_CB_EE(uint8_t opc){}
void CPU::opc_CB_EF(uint8_t opc){}

void CPU::opc_CB_F0(uint8_t opc){}
void CPU::opc_CB_F1(uint8_t opc){}
void CPU::opc_CB_F2(uint8_t opc){}
void CPU::opc_CB_F3(uint8_t opc){}
void CPU::opc_CB_F4(uint8_t opc){}
void CPU::opc_CB_F5(uint8_t opc){}
void CPU::opc_CB_F6(uint8_t opc){}
void CPU::opc_CB_F7(uint8_t opc){}
void CPU::opc_CB_F8(uint8_t opc){}
void CPU::opc_CB_F9(uint8_t opc){}
void CPU::opc_CB_FA(uint8_t opc){}
void CPU::opc_CB_FB(uint8_t opc){}
void CPU::opc_CB_FC(uint8_t opc){}
void CPU::opc_CB_FD(uint8_t opc){}
void CPU::opc_CB_FE(uint8_t opc){}
void CPU::opc_CB_FF(uint8_t opc){}