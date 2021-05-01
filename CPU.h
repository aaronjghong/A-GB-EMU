#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "MMU.h"

class CPU{

    typedef void (CPU::*opcode_function)(uint8_t);

    public:

        CPU();
        CPU(char* dir); // REMOVE THIS CONSTRUCTOR WHEN IMPLEMENTING MEMORY AND RE-IMPLEMENT LOAD ELSEWHERE
        ~CPU();
        
        // FOR TESTING OPCODES
        uint16_t getPC();
        // REMOVE AFTER DEBUGGING

        uint8_t getOpcode();
        void executeOpcode(uint8_t opc, bool CB_mode = false);

        uint8_t h_CYCLES;   // For Opcode cycle count, public as main.cpp will use this to emulate accurately

        bool stopped = false;   // For STOP instruction (?)
        bool halted = false;    // For HALT instruction (?)

        MMU h_MEMORY;    // CPU's Memory

    private:

        uint8_t h_SCREENDATA[160][144][2];  // 160x144 Monitor with 2bpp format

        uint8_t h_A;    // Accumulator Register
        uint8_t h_F;    // Flag Register
        uint8_t h_B, h_C, h_D, h_E, h_H, h_L;    // 8-bit Data Registers

        uint16_t h_SP;   // Stack Pointer
        uint16_t h_PC;   // Program Counter

        bool h_IME = false;   // IME Flag (interrupt master enable)
        bool i_scheduled = false;   // For EI opcode -> Enable interrupts at the next machine cycle
        uint8_t h_IE = h_MEMORY.memory[0xFFFF];    // IE Register
        uint8_t h_IF = h_MEMORY.memory[0xFF0F];    // IF Flag

        /* 16-bit Register Functions */

        // AF Register
        uint16_t get_h_AF(){
            return (h_A << 8) | h_F;
        }
        void  set_h_AF(uint16_t data){
            h_F = data & 0x00F0;    // CANNOT WRITE TO LOWER 4 BITS OF F FLAG
            h_A = (data >> 8) & 0x00FF; // bitwise and likely not needed here 
        }

        // BC Register
        uint16_t get_h_BC(){
            return (h_B << 8) | h_C;
        }
        void set_h_BC(uint16_t data){
            h_C = data & 0x00FF;
            h_B = (data >> 8) & 0x00FF;
        }

        // DE Register
        uint16_t get_h_DE(){
            return (h_D << 8) | h_E;
        }
        void set_h_DE(uint16_t data){
            h_E = data & 0x00FF;
            h_D = (data >> 8) & 0x00FF;
        }

        // HL Register
        uint16_t get_h_HL(){
            return (h_H << 8) | h_L;
        }
        void set_h_HL(uint16_t data){
            h_L = data & 0x00FF;
            h_H = (data >> 8) & 0x00FF;
        }

        /* Flag Register Functions */

        // Flag Z is bit 7 of h_F
        bool get_flag_Z(){
            return (h_F >> 7 & 0b00000001) != 0;
        }
        void set_flag_Z(bool on){
            if(on) h_F |= 0b10000000;
            else h_F &= 0b01111111;
        }

        // Flag N is bit 6 of h_F
        bool get_flag_N(){
            return (h_F >> 6 & 0b00000001) != 0;
        }
        void set_flag_N(bool on){
            if(on) h_F |= 0b01000000;
            else h_F &= 0b10111111;
        }

        // Flag H is  bit 5 of h_F
        bool get_flag_H(){
            return (h_F >> 5 & 0b00000001) != 0;
        }
        void set_flag_H(bool on){
            if(on) h_F |= 0b00100000;
            else h_F &= 0b11011111;
        }

        // Flag C is bit 4 of h_F
        bool get_flag_C(){
            return (h_F >> 4 & 0b00000001) != 0;
        }
        void set_flag_C(bool on){
            if(on) h_F |= 0b00010000;
            else h_F &= 0b11101111;
        }

        /* Opcode Helper Functions */

        void conditionalRelativeJump(bool condition, int8_t offset);
        void relativeJump(int8_t offset);
        void conditionalPositionJump(bool condition);
        void positionJump();

        void ret();
        void conditionalRet(bool condition);
        void reti();

        void call();
        void conditionalCall(bool condition);

        void rst(uint8_t f);

        void add8(uint8_t &target, uint8_t addend); 
        void adc8(uint8_t &target, uint8_t addend);

        void sub8(uint8_t &target, uint8_t subtrahend);
        void sbc8(uint8_t &target, uint8_t subtrahend);

        void and8(uint8_t &target, uint8_t other);
        void xor8(uint8_t &target, uint8_t other);
        void or8(uint8_t &target, uint8_t other);

        void cp8(uint8_t target, uint8_t subtrahend);

        void increment8(uint8_t &target);
        void decrement8(uint8_t &target);

        //void increment16(uint16_t target); // -> unnecessary?
        //void decrement16(uint16_t target); // -> unnecessary?
        //void load16(uint16_t &target, uint16_t data); // -> want to implement this but not sure how at the moment 
        //    |-> set higher and lower seperately? (EG load16(uint8_t msb, uint8_t lsb, uint16_t data); 
        //        |-> load16(h_B, h_C, data))
        //void load8(uint8_t &target, uint8_t data); // -> unnecessary? will implement if load16 gets implemented
        //void add16(uint16_t target, uint16_t addend); // -> want to implement this but not sure how at the moment


        /* CB-Prefixed Opcode Helper Functions */

        void rlc8(uint8_t &target);
        void rl8(uint8_t &target);
        void rrc8(uint8_t &target);
        void rr8(uint8_t &target);
        void sla8(uint8_t &target);
        void sra8(uint8_t &target);
        void swap(uint8_t &target);
        void srl8(uint8_t &target);
        void bit8(uint8_t bit, uint8_t target);
        void res8(uint8_t bit, uint8_t &target);
        void set8(uint8_t bit, uint8_t &target);

        /* Opcode Functions */

        void opc_00(uint8_t opc);
        void opc_01(uint8_t opc);
        void opc_02(uint8_t opc);
        void opc_03(uint8_t opc);
        void opc_04(uint8_t opc);
        void opc_05(uint8_t opc);
        void opc_06(uint8_t opc);
        void opc_07(uint8_t opc);
        void opc_08(uint8_t opc);
        void opc_09(uint8_t opc);
        void opc_0A(uint8_t opc);
        void opc_0B(uint8_t opc);
        void opc_0C(uint8_t opc);
        void opc_0D(uint8_t opc);
        void opc_0E(uint8_t opc);
        void opc_0F(uint8_t opc);

        void opc_10(uint8_t opc);
        void opc_11(uint8_t opc);
        void opc_12(uint8_t opc);
        void opc_13(uint8_t opc);
        void opc_14(uint8_t opc);
        void opc_15(uint8_t opc);
        void opc_16(uint8_t opc);
        void opc_17(uint8_t opc);
        void opc_18(uint8_t opc);
        void opc_19(uint8_t opc);
        void opc_1A(uint8_t opc);
        void opc_1B(uint8_t opc);
        void opc_1C(uint8_t opc);
        void opc_1D(uint8_t opc);
        void opc_1E(uint8_t opc);
        void opc_1F(uint8_t opc);

        void opc_20(uint8_t opc);
        void opc_21(uint8_t opc);
        void opc_22(uint8_t opc);
        void opc_23(uint8_t opc);
        void opc_24(uint8_t opc);
        void opc_25(uint8_t opc);
        void opc_26(uint8_t opc);
        void opc_27(uint8_t opc);
        void opc_28(uint8_t opc);
        void opc_29(uint8_t opc);
        void opc_2A(uint8_t opc);
        void opc_2B(uint8_t opc);
        void opc_2C(uint8_t opc);
        void opc_2D(uint8_t opc);
        void opc_2E(uint8_t opc);
        void opc_2F(uint8_t opc);

        void opc_30(uint8_t opc);
        void opc_31(uint8_t opc);
        void opc_32(uint8_t opc);
        void opc_33(uint8_t opc);
        void opc_34(uint8_t opc);
        void opc_35(uint8_t opc);
        void opc_36(uint8_t opc);
        void opc_37(uint8_t opc);
        void opc_38(uint8_t opc);
        void opc_39(uint8_t opc);
        void opc_3A(uint8_t opc);
        void opc_3B(uint8_t opc);
        void opc_3C(uint8_t opc);
        void opc_3D(uint8_t opc);
        void opc_3E(uint8_t opc);
        void opc_3F(uint8_t opc);

        void opc_40(uint8_t opc);
        void opc_41(uint8_t opc);
        void opc_42(uint8_t opc);
        void opc_43(uint8_t opc);
        void opc_44(uint8_t opc);
        void opc_45(uint8_t opc);
        void opc_46(uint8_t opc);
        void opc_47(uint8_t opc);
        void opc_48(uint8_t opc);
        void opc_49(uint8_t opc);
        void opc_4A(uint8_t opc);
        void opc_4B(uint8_t opc);
        void opc_4C(uint8_t opc);
        void opc_4D(uint8_t opc);
        void opc_4E(uint8_t opc);
        void opc_4F(uint8_t opc);

        void opc_50(uint8_t opc);
        void opc_51(uint8_t opc);
        void opc_52(uint8_t opc);
        void opc_53(uint8_t opc);
        void opc_54(uint8_t opc);
        void opc_55(uint8_t opc);
        void opc_56(uint8_t opc);
        void opc_57(uint8_t opc);
        void opc_58(uint8_t opc);
        void opc_59(uint8_t opc);
        void opc_5A(uint8_t opc);
        void opc_5B(uint8_t opc);
        void opc_5C(uint8_t opc);
        void opc_5D(uint8_t opc);
        void opc_5E(uint8_t opc);
        void opc_5F(uint8_t opc);

        void opc_60(uint8_t opc);
        void opc_61(uint8_t opc);
        void opc_62(uint8_t opc);
        void opc_63(uint8_t opc);
        void opc_64(uint8_t opc);
        void opc_65(uint8_t opc);
        void opc_66(uint8_t opc);
        void opc_67(uint8_t opc);
        void opc_68(uint8_t opc);
        void opc_69(uint8_t opc);
        void opc_6A(uint8_t opc);
        void opc_6B(uint8_t opc);
        void opc_6C(uint8_t opc);
        void opc_6D(uint8_t opc);
        void opc_6E(uint8_t opc);
        void opc_6F(uint8_t opc);

        void opc_70(uint8_t opc);
        void opc_71(uint8_t opc);
        void opc_72(uint8_t opc);
        void opc_73(uint8_t opc);
        void opc_74(uint8_t opc);
        void opc_75(uint8_t opc);
        void opc_76(uint8_t opc);
        void opc_77(uint8_t opc);
        void opc_78(uint8_t opc);
        void opc_79(uint8_t opc);
        void opc_7A(uint8_t opc);
        void opc_7B(uint8_t opc);
        void opc_7C(uint8_t opc);
        void opc_7D(uint8_t opc);
        void opc_7E(uint8_t opc);
        void opc_7F(uint8_t opc);

        void opc_80(uint8_t opc);
        void opc_81(uint8_t opc);
        void opc_82(uint8_t opc);
        void opc_83(uint8_t opc);
        void opc_84(uint8_t opc);
        void opc_85(uint8_t opc);
        void opc_86(uint8_t opc);
        void opc_87(uint8_t opc);
        void opc_88(uint8_t opc);
        void opc_89(uint8_t opc);
        void opc_8A(uint8_t opc);
        void opc_8B(uint8_t opc);
        void opc_8C(uint8_t opc);
        void opc_8D(uint8_t opc);
        void opc_8E(uint8_t opc);
        void opc_8F(uint8_t opc);

        void opc_90(uint8_t opc);
        void opc_91(uint8_t opc);
        void opc_92(uint8_t opc);
        void opc_93(uint8_t opc);
        void opc_94(uint8_t opc);
        void opc_95(uint8_t opc);
        void opc_96(uint8_t opc);
        void opc_97(uint8_t opc);
        void opc_98(uint8_t opc);
        void opc_99(uint8_t opc);
        void opc_9A(uint8_t opc);
        void opc_9B(uint8_t opc);
        void opc_9C(uint8_t opc);
        void opc_9D(uint8_t opc);
        void opc_9E(uint8_t opc);
        void opc_9F(uint8_t opc);

        void opc_A0(uint8_t opc);
        void opc_A1(uint8_t opc);
        void opc_A2(uint8_t opc);
        void opc_A3(uint8_t opc);
        void opc_A4(uint8_t opc);
        void opc_A5(uint8_t opc);
        void opc_A6(uint8_t opc);
        void opc_A7(uint8_t opc);
        void opc_A8(uint8_t opc);
        void opc_A9(uint8_t opc);
        void opc_AA(uint8_t opc);
        void opc_AB(uint8_t opc);
        void opc_AC(uint8_t opc);
        void opc_AD(uint8_t opc);
        void opc_AE(uint8_t opc);
        void opc_AF(uint8_t opc);

        void opc_B0(uint8_t opc);
        void opc_B1(uint8_t opc);
        void opc_B2(uint8_t opc);
        void opc_B3(uint8_t opc);
        void opc_B4(uint8_t opc);
        void opc_B5(uint8_t opc);
        void opc_B6(uint8_t opc);
        void opc_B7(uint8_t opc);
        void opc_B8(uint8_t opc);
        void opc_B9(uint8_t opc);
        void opc_BA(uint8_t opc);
        void opc_BB(uint8_t opc);
        void opc_BC(uint8_t opc);
        void opc_BD(uint8_t opc);
        void opc_BE(uint8_t opc);
        void opc_BF(uint8_t opc);

        void opc_C0(uint8_t opc);
        void opc_C1(uint8_t opc);
        void opc_C2(uint8_t opc);
        void opc_C3(uint8_t opc);
        void opc_C4(uint8_t opc);
        void opc_C5(uint8_t opc);
        void opc_C6(uint8_t opc);
        void opc_C7(uint8_t opc);
        void opc_C8(uint8_t opc);
        void opc_C9(uint8_t opc);
        void opc_CA(uint8_t opc);
        void opc_CB(uint8_t opc);
        void opc_CC(uint8_t opc);
        void opc_CD(uint8_t opc);
        void opc_CE(uint8_t opc);
        void opc_CF(uint8_t opc);

        void opc_D0(uint8_t opc);
        void opc_D1(uint8_t opc);
        void opc_D2(uint8_t opc);
        void opc_D3(uint8_t opc);
        void opc_D4(uint8_t opc);
        void opc_D5(uint8_t opc);
        void opc_D6(uint8_t opc);
        void opc_D7(uint8_t opc);
        void opc_D8(uint8_t opc);
        void opc_D9(uint8_t opc);
        void opc_DA(uint8_t opc);
        void opc_DB(uint8_t opc);
        void opc_DC(uint8_t opc);
        void opc_DD(uint8_t opc);
        void opc_DE(uint8_t opc);
        void opc_DF(uint8_t opc);

        void opc_E0(uint8_t opc);
        void opc_E1(uint8_t opc);
        void opc_E2(uint8_t opc);
        void opc_E3(uint8_t opc);
        void opc_E4(uint8_t opc);
        void opc_E5(uint8_t opc);
        void opc_E6(uint8_t opc);
        void opc_E7(uint8_t opc);
        void opc_E8(uint8_t opc);
        void opc_E9(uint8_t opc);
        void opc_EA(uint8_t opc);
        void opc_EB(uint8_t opc);
        void opc_EC(uint8_t opc);
        void opc_ED(uint8_t opc);
        void opc_EE(uint8_t opc);
        void opc_EF(uint8_t opc);

        void opc_F0(uint8_t opc);
        void opc_F1(uint8_t opc);
        void opc_F2(uint8_t opc);
        void opc_F3(uint8_t opc);
        void opc_F4(uint8_t opc);
        void opc_F5(uint8_t opc);
        void opc_F6(uint8_t opc);
        void opc_F7(uint8_t opc);
        void opc_F8(uint8_t opc);
        void opc_F9(uint8_t opc);
        void opc_FA(uint8_t opc);
        void opc_FB(uint8_t opc);
        void opc_FC(uint8_t opc);
        void opc_FD(uint8_t opc);
        void opc_FE(uint8_t opc);
        void opc_FF(uint8_t opc);

        /* CB Prefixed Opcodes */

        void opc_CB_00(uint8_t opc);
        void opc_CB_01(uint8_t opc);
        void opc_CB_02(uint8_t opc);
        void opc_CB_03(uint8_t opc);
        void opc_CB_04(uint8_t opc);
        void opc_CB_05(uint8_t opc);
        void opc_CB_06(uint8_t opc);
        void opc_CB_07(uint8_t opc);
        void opc_CB_08(uint8_t opc);
        void opc_CB_09(uint8_t opc);
        void opc_CB_0A(uint8_t opc);
        void opc_CB_0B(uint8_t opc);
        void opc_CB_0C(uint8_t opc);
        void opc_CB_0D(uint8_t opc);
        void opc_CB_0E(uint8_t opc);
        void opc_CB_0F(uint8_t opc);

        void opc_CB_10(uint8_t opc);
        void opc_CB_11(uint8_t opc);
        void opc_CB_12(uint8_t opc);
        void opc_CB_13(uint8_t opc);
        void opc_CB_14(uint8_t opc);
        void opc_CB_15(uint8_t opc);
        void opc_CB_16(uint8_t opc);
        void opc_CB_17(uint8_t opc);
        void opc_CB_18(uint8_t opc);
        void opc_CB_19(uint8_t opc);
        void opc_CB_1A(uint8_t opc);
        void opc_CB_1B(uint8_t opc);
        void opc_CB_1C(uint8_t opc);
        void opc_CB_1D(uint8_t opc);
        void opc_CB_1E(uint8_t opc);
        void opc_CB_1F(uint8_t opc);

        void opc_CB_20(uint8_t opc);
        void opc_CB_21(uint8_t opc);
        void opc_CB_22(uint8_t opc);
        void opc_CB_23(uint8_t opc);
        void opc_CB_24(uint8_t opc);
        void opc_CB_25(uint8_t opc);
        void opc_CB_26(uint8_t opc);
        void opc_CB_27(uint8_t opc);
        void opc_CB_28(uint8_t opc);
        void opc_CB_29(uint8_t opc);
        void opc_CB_2A(uint8_t opc);
        void opc_CB_2B(uint8_t opc);
        void opc_CB_2C(uint8_t opc);
        void opc_CB_2D(uint8_t opc);
        void opc_CB_2E(uint8_t opc);
        void opc_CB_2F(uint8_t opc);

        void opc_CB_30(uint8_t opc);
        void opc_CB_31(uint8_t opc);
        void opc_CB_32(uint8_t opc);
        void opc_CB_33(uint8_t opc);
        void opc_CB_34(uint8_t opc);
        void opc_CB_35(uint8_t opc);
        void opc_CB_36(uint8_t opc);
        void opc_CB_37(uint8_t opc);
        void opc_CB_38(uint8_t opc);
        void opc_CB_39(uint8_t opc);
        void opc_CB_3A(uint8_t opc);
        void opc_CB_3B(uint8_t opc);
        void opc_CB_3C(uint8_t opc);
        void opc_CB_3D(uint8_t opc);
        void opc_CB_3E(uint8_t opc);
        void opc_CB_3F(uint8_t opc);

        void opc_CB_40(uint8_t opc);
        void opc_CB_41(uint8_t opc);
        void opc_CB_42(uint8_t opc);
        void opc_CB_43(uint8_t opc);
        void opc_CB_44(uint8_t opc);
        void opc_CB_45(uint8_t opc);
        void opc_CB_46(uint8_t opc);
        void opc_CB_47(uint8_t opc);
        void opc_CB_48(uint8_t opc);
        void opc_CB_49(uint8_t opc);
        void opc_CB_4A(uint8_t opc);
        void opc_CB_4B(uint8_t opc);
        void opc_CB_4C(uint8_t opc);
        void opc_CB_4D(uint8_t opc);
        void opc_CB_4E(uint8_t opc);
        void opc_CB_4F(uint8_t opc);

        void opc_CB_50(uint8_t opc);
        void opc_CB_51(uint8_t opc);
        void opc_CB_52(uint8_t opc);
        void opc_CB_53(uint8_t opc);
        void opc_CB_54(uint8_t opc);
        void opc_CB_55(uint8_t opc);
        void opc_CB_56(uint8_t opc);
        void opc_CB_57(uint8_t opc);
        void opc_CB_58(uint8_t opc);
        void opc_CB_59(uint8_t opc);
        void opc_CB_5A(uint8_t opc);
        void opc_CB_5B(uint8_t opc);
        void opc_CB_5C(uint8_t opc);
        void opc_CB_5D(uint8_t opc);
        void opc_CB_5E(uint8_t opc);
        void opc_CB_5F(uint8_t opc);

        void opc_CB_60(uint8_t opc);
        void opc_CB_61(uint8_t opc);
        void opc_CB_62(uint8_t opc);
        void opc_CB_63(uint8_t opc);
        void opc_CB_64(uint8_t opc);
        void opc_CB_65(uint8_t opc);
        void opc_CB_66(uint8_t opc);
        void opc_CB_67(uint8_t opc);
        void opc_CB_68(uint8_t opc);
        void opc_CB_69(uint8_t opc);
        void opc_CB_6A(uint8_t opc);
        void opc_CB_6B(uint8_t opc);
        void opc_CB_6C(uint8_t opc);
        void opc_CB_6D(uint8_t opc);
        void opc_CB_6E(uint8_t opc);
        void opc_CB_6F(uint8_t opc);

        void opc_CB_70(uint8_t opc);
        void opc_CB_71(uint8_t opc);
        void opc_CB_72(uint8_t opc);
        void opc_CB_73(uint8_t opc);
        void opc_CB_74(uint8_t opc);
        void opc_CB_75(uint8_t opc);
        void opc_CB_76(uint8_t opc);
        void opc_CB_77(uint8_t opc);
        void opc_CB_78(uint8_t opc);
        void opc_CB_79(uint8_t opc);
        void opc_CB_7A(uint8_t opc);
        void opc_CB_7B(uint8_t opc);
        void opc_CB_7C(uint8_t opc);
        void opc_CB_7D(uint8_t opc);
        void opc_CB_7E(uint8_t opc);
        void opc_CB_7F(uint8_t opc);

        void opc_CB_80(uint8_t opc);
        void opc_CB_81(uint8_t opc);
        void opc_CB_82(uint8_t opc);
        void opc_CB_83(uint8_t opc);
        void opc_CB_84(uint8_t opc);
        void opc_CB_85(uint8_t opc);
        void opc_CB_86(uint8_t opc);
        void opc_CB_87(uint8_t opc);
        void opc_CB_88(uint8_t opc);
        void opc_CB_89(uint8_t opc);
        void opc_CB_8A(uint8_t opc);
        void opc_CB_8B(uint8_t opc);
        void opc_CB_8C(uint8_t opc);
        void opc_CB_8D(uint8_t opc);
        void opc_CB_8E(uint8_t opc);
        void opc_CB_8F(uint8_t opc);

        void opc_CB_90(uint8_t opc);
        void opc_CB_91(uint8_t opc);
        void opc_CB_92(uint8_t opc);
        void opc_CB_93(uint8_t opc);
        void opc_CB_94(uint8_t opc);
        void opc_CB_95(uint8_t opc);
        void opc_CB_96(uint8_t opc);
        void opc_CB_97(uint8_t opc);
        void opc_CB_98(uint8_t opc);
        void opc_CB_99(uint8_t opc);
        void opc_CB_9A(uint8_t opc);
        void opc_CB_9B(uint8_t opc);
        void opc_CB_9C(uint8_t opc);
        void opc_CB_9D(uint8_t opc);
        void opc_CB_9E(uint8_t opc);
        void opc_CB_9F(uint8_t opc);

        void opc_CB_A0(uint8_t opc);
        void opc_CB_A1(uint8_t opc);
        void opc_CB_A2(uint8_t opc);
        void opc_CB_A3(uint8_t opc);
        void opc_CB_A4(uint8_t opc);
        void opc_CB_A5(uint8_t opc);
        void opc_CB_A6(uint8_t opc);
        void opc_CB_A7(uint8_t opc);
        void opc_CB_A8(uint8_t opc);
        void opc_CB_A9(uint8_t opc);
        void opc_CB_AA(uint8_t opc);
        void opc_CB_AB(uint8_t opc);
        void opc_CB_AC(uint8_t opc);
        void opc_CB_AD(uint8_t opc);
        void opc_CB_AE(uint8_t opc);
        void opc_CB_AF(uint8_t opc);

        void opc_CB_B0(uint8_t opc);
        void opc_CB_B1(uint8_t opc);
        void opc_CB_B2(uint8_t opc);
        void opc_CB_B3(uint8_t opc);
        void opc_CB_B4(uint8_t opc);
        void opc_CB_B5(uint8_t opc);
        void opc_CB_B6(uint8_t opc);
        void opc_CB_B7(uint8_t opc);
        void opc_CB_B8(uint8_t opc);
        void opc_CB_B9(uint8_t opc);
        void opc_CB_BA(uint8_t opc);
        void opc_CB_BB(uint8_t opc);
        void opc_CB_BC(uint8_t opc);
        void opc_CB_BD(uint8_t opc);
        void opc_CB_BE(uint8_t opc);
        void opc_CB_BF(uint8_t opc);

        void opc_CB_C0(uint8_t opc);
        void opc_CB_C1(uint8_t opc);
        void opc_CB_C2(uint8_t opc);
        void opc_CB_C3(uint8_t opc);
        void opc_CB_C4(uint8_t opc);
        void opc_CB_C5(uint8_t opc);
        void opc_CB_C6(uint8_t opc);
        void opc_CB_C7(uint8_t opc);
        void opc_CB_C8(uint8_t opc);
        void opc_CB_C9(uint8_t opc);
        void opc_CB_CA(uint8_t opc);
        void opc_CB_CB(uint8_t opc);
        void opc_CB_CC(uint8_t opc);
        void opc_CB_CD(uint8_t opc);
        void opc_CB_CE(uint8_t opc);
        void opc_CB_CF(uint8_t opc);

        void opc_CB_D0(uint8_t opc);
        void opc_CB_D1(uint8_t opc);
        void opc_CB_D2(uint8_t opc);
        void opc_CB_D3(uint8_t opc);
        void opc_CB_D4(uint8_t opc);
        void opc_CB_D5(uint8_t opc);
        void opc_CB_D6(uint8_t opc);
        void opc_CB_D7(uint8_t opc);
        void opc_CB_D8(uint8_t opc);
        void opc_CB_D9(uint8_t opc);
        void opc_CB_DA(uint8_t opc);
        void opc_CB_DB(uint8_t opc);
        void opc_CB_DC(uint8_t opc);
        void opc_CB_DD(uint8_t opc);
        void opc_CB_DE(uint8_t opc);
        void opc_CB_DF(uint8_t opc);

        void opc_CB_E0(uint8_t opc);
        void opc_CB_E1(uint8_t opc);
        void opc_CB_E2(uint8_t opc);
        void opc_CB_E3(uint8_t opc);
        void opc_CB_E4(uint8_t opc);
        void opc_CB_E5(uint8_t opc);
        void opc_CB_E6(uint8_t opc);
        void opc_CB_E7(uint8_t opc);
        void opc_CB_E8(uint8_t opc);
        void opc_CB_E9(uint8_t opc);
        void opc_CB_EA(uint8_t opc);
        void opc_CB_EB(uint8_t opc);
        void opc_CB_EC(uint8_t opc);
        void opc_CB_ED(uint8_t opc);
        void opc_CB_EE(uint8_t opc);
        void opc_CB_EF(uint8_t opc);

        void opc_CB_F0(uint8_t opc);
        void opc_CB_F1(uint8_t opc);
        void opc_CB_F2(uint8_t opc);
        void opc_CB_F3(uint8_t opc);
        void opc_CB_F4(uint8_t opc);
        void opc_CB_F5(uint8_t opc);
        void opc_CB_F6(uint8_t opc);
        void opc_CB_F7(uint8_t opc);
        void opc_CB_F8(uint8_t opc);
        void opc_CB_F9(uint8_t opc);
        void opc_CB_FA(uint8_t opc);
        void opc_CB_FB(uint8_t opc);
        void opc_CB_FC(uint8_t opc);
        void opc_CB_FD(uint8_t opc);
        void opc_CB_FE(uint8_t opc);
        void opc_CB_FF(uint8_t opc);

        /* Opcode Function Array */

        opcode_function opcode_arr[0x100] = {
            opc_00, opc_01, opc_02, opc_03, opc_04, opc_05, opc_06, opc_07, opc_08, opc_09, opc_0A, opc_0B, opc_0C, opc_0D, opc_0E, opc_0F,
            opc_10, opc_11, opc_12, opc_13, opc_14, opc_15, opc_16, opc_17, opc_18, opc_19, opc_1A, opc_1B, opc_1C, opc_1D, opc_1E, opc_1F,
            opc_20, opc_21, opc_22, opc_23, opc_24, opc_25, opc_26, opc_27, opc_28, opc_29, opc_2A, opc_2B, opc_2C, opc_2D, opc_2E, opc_2F,
            opc_30, opc_31, opc_32, opc_33, opc_34, opc_35, opc_36, opc_37, opc_38, opc_39, opc_3A, opc_3B, opc_3C, opc_3D, opc_3E, opc_3F,
            opc_40, opc_41, opc_42, opc_43, opc_44, opc_45, opc_46, opc_47, opc_48, opc_49, opc_4A, opc_4B, opc_4C, opc_4D, opc_4E, opc_4F,
            opc_50, opc_51, opc_52, opc_53, opc_54, opc_55, opc_56, opc_57, opc_58, opc_59, opc_5A, opc_5B, opc_5C, opc_5D, opc_5E, opc_5F,
            opc_60, opc_61, opc_62, opc_63, opc_64, opc_65, opc_66, opc_67, opc_68, opc_69, opc_6A, opc_6B, opc_6C, opc_6D, opc_6E, opc_6F,
            opc_70, opc_71, opc_72, opc_73, opc_74, opc_75, opc_76, opc_77, opc_78, opc_79, opc_7A, opc_7B, opc_7C, opc_7D, opc_7E, opc_7F,
            opc_80, opc_81, opc_82, opc_83, opc_84, opc_85, opc_86, opc_87, opc_88, opc_89, opc_8A, opc_8B, opc_8C, opc_8D, opc_8E, opc_8F,
            opc_90, opc_91, opc_92, opc_93, opc_94, opc_95, opc_96, opc_97, opc_98, opc_99, opc_9A, opc_9B, opc_9C, opc_9D, opc_9E, opc_9F,
            opc_A0, opc_A1, opc_A2, opc_A3, opc_A4, opc_A5, opc_A6, opc_A7, opc_A8, opc_A9, opc_AA, opc_AB, opc_AC, opc_AD, opc_AE, opc_AF,
            opc_B0, opc_B1, opc_B2, opc_B3, opc_B4, opc_B5, opc_B6, opc_B7, opc_B8, opc_B9, opc_BA, opc_BB, opc_BC, opc_BD, opc_BE, opc_BF,
            opc_C0, opc_C1, opc_C2, opc_C3, opc_C4, opc_C5, opc_C6, opc_C7, opc_C8, opc_C9, opc_CA, opc_CB, opc_CC, opc_CD, opc_CE, opc_CF,
            opc_D0, opc_D1, opc_D2, opc_D3, opc_D4, opc_D5, opc_D6, opc_D7, opc_D8, opc_D9, opc_DA, opc_DB, opc_DC, opc_DD, opc_DE, opc_DF,
            opc_E0, opc_E1, opc_E2, opc_E3, opc_E4, opc_E5, opc_E6, opc_E7, opc_E8, opc_E9, opc_EA, opc_EB, opc_EC, opc_ED, opc_EE, opc_EF,
            opc_F0, opc_F1, opc_F2, opc_F3, opc_F4, opc_F5, opc_F6, opc_F7, opc_F8, opc_F9, opc_FA, opc_FB, opc_FC, opc_FD, opc_FE, opc_FF
        };

        /* CB Prefixed Opcode Function Array */

        opcode_function opcode_CB_arr[0x100] = {
            opc_CB_00, opc_CB_01, opc_CB_02, opc_CB_03, opc_CB_04, opc_CB_05, opc_CB_06, opc_CB_07, opc_CB_08, opc_CB_09, opc_CB_0A, opc_CB_0B, opc_CB_0C, opc_CB_0D, opc_CB_0E, opc_CB_0F,
            opc_CB_10, opc_CB_11, opc_CB_12, opc_CB_13, opc_CB_14, opc_CB_15, opc_CB_16, opc_CB_17, opc_CB_18, opc_CB_19, opc_CB_1A, opc_CB_1B, opc_CB_1C, opc_CB_1D, opc_CB_1E, opc_CB_1F,
            opc_CB_20, opc_CB_21, opc_CB_22, opc_CB_23, opc_CB_24, opc_CB_25, opc_CB_26, opc_CB_27, opc_CB_28, opc_CB_29, opc_CB_2A, opc_CB_2B, opc_CB_2C, opc_CB_2D, opc_CB_2E, opc_CB_2F,
            opc_CB_30, opc_CB_31, opc_CB_32, opc_CB_33, opc_CB_34, opc_CB_35, opc_CB_36, opc_CB_37, opc_CB_38, opc_CB_39, opc_CB_3A, opc_CB_3B, opc_CB_3C, opc_CB_3D, opc_CB_3E, opc_CB_3F,
            opc_CB_40, opc_CB_41, opc_CB_42, opc_CB_43, opc_CB_44, opc_CB_45, opc_CB_46, opc_CB_47, opc_CB_48, opc_CB_49, opc_CB_4A, opc_CB_4B, opc_CB_4C, opc_CB_4D, opc_CB_4E, opc_CB_4F,
            opc_CB_50, opc_CB_51, opc_CB_52, opc_CB_53, opc_CB_54, opc_CB_55, opc_CB_56, opc_CB_57, opc_CB_58, opc_CB_59, opc_CB_5A, opc_CB_5B, opc_CB_5C, opc_CB_5D, opc_CB_5E, opc_CB_5F,
            opc_CB_60, opc_CB_61, opc_CB_62, opc_CB_63, opc_CB_64, opc_CB_65, opc_CB_66, opc_CB_67, opc_CB_68, opc_CB_69, opc_CB_6A, opc_CB_6B, opc_CB_6C, opc_CB_6D, opc_CB_6E, opc_CB_6F,
            opc_CB_70, opc_CB_71, opc_CB_72, opc_CB_73, opc_CB_74, opc_CB_75, opc_CB_76, opc_CB_77, opc_CB_78, opc_CB_79, opc_CB_7A, opc_CB_7B, opc_CB_7C, opc_CB_7D, opc_CB_7E, opc_CB_7F,
            opc_CB_80, opc_CB_81, opc_CB_82, opc_CB_83, opc_CB_84, opc_CB_85, opc_CB_86, opc_CB_87, opc_CB_88, opc_CB_89, opc_CB_8A, opc_CB_8B, opc_CB_8C, opc_CB_8D, opc_CB_8E, opc_CB_8F,
            opc_CB_90, opc_CB_91, opc_CB_92, opc_CB_93, opc_CB_94, opc_CB_95, opc_CB_96, opc_CB_97, opc_CB_98, opc_CB_99, opc_CB_9A, opc_CB_9B, opc_CB_9C, opc_CB_9D, opc_CB_9E, opc_CB_9F,
            opc_CB_A0, opc_CB_A1, opc_CB_A2, opc_CB_A3, opc_CB_A4, opc_CB_A5, opc_CB_A6, opc_CB_A7, opc_CB_A8, opc_CB_A9, opc_CB_AA, opc_CB_AB, opc_CB_AC, opc_CB_AD, opc_CB_AE, opc_CB_AF,
            opc_CB_B0, opc_CB_B1, opc_CB_B2, opc_CB_B3, opc_CB_B4, opc_CB_B5, opc_CB_B6, opc_CB_B7, opc_CB_B8, opc_CB_B9, opc_CB_BA, opc_CB_BB, opc_CB_BC, opc_CB_BD, opc_CB_BE, opc_CB_BF,
            opc_CB_C0, opc_CB_C1, opc_CB_C2, opc_CB_C3, opc_CB_C4, opc_CB_C5, opc_CB_C6, opc_CB_C7, opc_CB_C8, opc_CB_C9, opc_CB_CA, opc_CB_CB, opc_CB_CC, opc_CB_CD, opc_CB_CE, opc_CB_CF,
            opc_CB_D0, opc_CB_D1, opc_CB_D2, opc_CB_D3, opc_CB_D4, opc_CB_D5, opc_CB_D6, opc_CB_D7, opc_CB_D8, opc_CB_D9, opc_CB_DA, opc_CB_DB, opc_CB_DC, opc_CB_DD, opc_CB_DE, opc_CB_DF,
            opc_CB_E0, opc_CB_E1, opc_CB_E2, opc_CB_E3, opc_CB_E4, opc_CB_E5, opc_CB_E6, opc_CB_E7, opc_CB_E8, opc_CB_E9, opc_CB_EA, opc_CB_EB, opc_CB_EC, opc_CB_ED, opc_CB_EE, opc_CB_EF,
            opc_CB_F0, opc_CB_F1, opc_CB_F2, opc_CB_F3, opc_CB_F4, opc_CB_F5, opc_CB_F6, opc_CB_F7, opc_CB_F8, opc_CB_F9, opc_CB_FA, opc_CB_FB, opc_CB_FC, opc_CB_FD, opc_CB_FE, opc_CB_FF
        };
};