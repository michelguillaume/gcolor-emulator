/* ================================================================
 * Project: Gameboy Color Emulator
 * File: cpu.cpp
 * Description: This file contains the implementation of the CPU
 *              class for the Gameboy emulator.
 *
 * Author: Guillaume MICHEL
 * Created on: September 24, 2024
 *
 * License: MIT License
 *
 * Copyright (c) 2024 Guillaume MICHEL
 * ================================================================
 */


#include "cpu.hpp"

namespace emulator
{
    CPU::CPU(): AF(), BC(), DE(),
    HL(),     // Program starts at 0x0100 in most GB ROMs
    PC(0x0100),      // Stack Pointer initialization
    SP(0xFFFE)
    {

    }

    std::array<void (*)(CPU*), 256> CPU::instruction_table = {
        [](CPU *cpu) { /* Does nothing, just consumes one CPU cycle */ },  // 0x00 NOP
        [](CPU *cpu) { cpu->ldReg16_d16(cpu->BC); },                    // 0x01 LD BC,d16
        [](CPU *cpu) { cpu->ldMemReg16_A(cpu->BC); },                   // 0x02 LD (BC),A
        [](CPU *cpu) { cpu->incReg16(cpu->BC); },                       // 0x03 INC BC
        [](CPU *cpu) { cpu->incReg8(cpu->B); },                         // 0x04 INC B
        [](CPU *cpu) { cpu->decReg8(cpu->B); },                         // 0x05 DEC B
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->D); },                       // 0x06 LD B,d8
        [](CPU *cpu) {},                            // 0x07 RLCA

#warning need to be fixed
        [](CPU *cpu) { cpu->ldMemA16_SP(); },                             // 0x08 LD (a16),SP
        [](CPU *cpu) { cpu->addHL_Reg16(cpu->BC); },                      // 0x09 ADD HL,BC
        [](CPU *cpu) { cpu->ldA_MemReg16(cpu->BC); },                     // 0x0A LD A,(BC)
        [](CPU *cpu) { cpu->decReg16(cpu->BC); },                      // 0x0B DEC BC
        [](CPU *cpu) { cpu->incReg8(cpu->C); },                        // 0x0C INC C
        [](CPU *cpu) { cpu->decReg8(cpu->C); },                        // 0x0D DEC C
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->C); },                      // 0x0E LD C,d8
        [](CPU *cpu) {},                            // 0x0F RRCA

        [](CPU *cpu) {},                            // 0x10 STOP 0
        [](CPU *cpu) { cpu->ldReg16_d16(cpu->DE); },                   // 0x11 LD DE,d16
        [](CPU *cpu) { cpu->ldMemReg16_A(cpu->DE); },                  // 0x12 LD (DE),A
        [](CPU *cpu) { cpu->incReg16(cpu->DE); },                      // 0x13 INC DE
        [](CPU *cpu) { cpu->incReg8(cpu->D); },                        // 0x14 INC D
        [](CPU *cpu) { cpu->decReg8(cpu->D); },                        // 0x15 DEC D
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->D); },                      // 0x16 LD D,d8
        [](CPU *cpu) {},                            // 0x17 RLA
        [](CPU *cpu) {},                            // 0x18 JR r8
        [](CPU *cpu) { cpu->addHL_Reg16(cpu->DE); },                      // 0x19 ADD HL,DE
        [](CPU *cpu) { cpu->ldA_MemReg16(cpu->DE); },                  // 0x1A LD A,(DE)
        [](CPU *cpu) { cpu->decReg16(cpu->DE); },                      // 0x1B DEC DE
        [](CPU *cpu) { cpu->incReg8(cpu->E); },                        // 0x1C INC E
        [](CPU *cpu) { cpu->decReg8(cpu->E); },                        // 0x1D DEC E
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->E); },                      // 0x1E LD E,d8
        [](CPU *cpu) {},                            // 0x1F RRA

        [](CPU *cpu) {},                            // 0x20 JR NZ,r8
        [](CPU *cpu) { cpu->ldReg16_d16(cpu->HL); },                   // 0x21 LD HL,d16
        [](CPU *cpu) { cpu->ldMemHLplus_A(); },                           // 0x22 LD (HL+),A
        [](CPU *cpu) { cpu->incReg16(cpu->HL); },                      // 0x23 INC HL
        [](CPU *cpu) { cpu->incReg8(cpu->H); },                        // 0x24 INC H
        [](CPU *cpu) { cpu->decReg8(cpu->H); },                        // 0x25 DEC H
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->H); },                      // 0x26 LD H,d8
        [](CPU *cpu) {  },                            // 0x27 DAA

        [](CPU *cpu) {  },                            // 0x28 JR Z,r8
        [](CPU *cpu) { cpu->addHL_HL(); },                                // 0x29 ADD HL,HL
        [](CPU *cpu) { cpu->ldA_MemHLplus(); },                           // 0x2A LD A,(HL+)
        [](CPU *cpu) { cpu->decReg16(cpu->HL); },                      // 0x2B DEC HL
        [](CPU *cpu) { cpu->incReg8(cpu->L); },                        // 0x2C INC L
        [](CPU *cpu) { cpu->decReg8(cpu->L); },                        // 0x2D DEC L
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->L); },                      // 0x2E LD L,d8
        [](CPU *cpu) {},                            // 0x2F CPL

        [](CPU *cpu) {},                            // 0x30 JR NC,r8
        [](CPU *cpu) { cpu->ldReg16_d16(cpu->SP); },                   // 0x31 LD SP,d16
        [](CPU *cpu) { cpu->ldMemHLminus_A(); },                          // 0x32 LD (HL-),A
        [](CPU *cpu) { cpu->incReg16(cpu->SP); },                      // 0x33 INC SP
        [](CPU *cpu) { cpu->incMemHL(); },                                // 0x34 INC (HL)
        [](CPU *cpu) { cpu->decMemHL(); },                                // 0x35 DEC (HL)
        [](CPU *cpu) { cpu->ldMemHL_d8(); },                              // 0x36 LD (HL),d8
        [](CPU *cpu) {},                            // 0x37 SCF

        [](CPU *cpu) {},                            // 0x38 JR C,r8
        [](CPU *cpu) { cpu->addHL_Reg16(cpu->SP); },                      // 0x39 ADD HL,SP
        [](CPU *cpu) { cpu->ldA_MemHLminus(); },                          // 0x3A LD A,(HL-)
        [](CPU *cpu) { cpu->decReg16(cpu->SP); },                      // 0x3B DEC SP
        [](CPU *cpu) { cpu->incReg8(cpu->A); },                        // 0x3C INC A
        [](CPU *cpu) { cpu->decReg8(cpu->A); },                        // 0x3D DEC A
        [](CPU *cpu) { cpu->ldReg8_d8(cpu->A); },                      // 0x3E LD A,d8
        [](CPU *cpu) {},                            // 0x3F CCF
#warning ld B in B look for optimisation
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->B); },  // 0x40 LD B,B
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->C); },  // 0x41 LD B,C
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->D); },  // 0x42 LD B,D
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->E); },  // 0x43 LD B,E
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->H); },  // 0x44 LD B,H
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->L); },  // 0x45 LD B,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->B); },            // 0x46 LD B,(HL)
        [](CPU *cpu) { cpu->ld(cpu->B, cpu->A); },  // 0x47 LD B,A

        [](CPU *cpu) { cpu->ld(cpu->C, cpu->B); },  // 0x48 LD C,B
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->C); },  // 0x49 LD C,C
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->D); },  // 0x4A LD C,D
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->E); },  // 0x4B LD C,E
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->H); },  // 0x4C LD C,H
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->L); },  // 0x4D LD C,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->C); },            // 0x4E LD C,(HL)
        [](CPU *cpu) { cpu->ld(cpu->C, cpu->A); },  // 0x4F LD C,A

        [](CPU *cpu) { cpu->ld(cpu->D, cpu->B); },  // 0x50 LD D,B
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->C); },  // 0x51 LD D,C
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->D); },  // 0x52 LD D,D
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->E); },  // 0x53 LD D,E
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->H); },  // 0x54 LD D,H
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->L); },  // 0x55 LD D,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->D); },                 // 0x56 LD D,(HL)
        [](CPU *cpu) { cpu->ld(cpu->D, cpu->A); },  // 0x57 LD D,A

        [](CPU *cpu) { cpu->ld(cpu->E, cpu->B); },  // 0x58 LD E,B
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->C); },  // 0x59 LD E,C
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->D); },  // 0x5A LD E,D
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->E); },  // 0x5B LD E,E
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->H); },  // 0x5C LD E,H
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->L); },  // 0x5D LD E,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->E); },            // 0x5E LD E,(HL)
        [](CPU *cpu) { cpu->ld(cpu->E, cpu->A); },  // 0x5F LD E,A

        [](CPU *cpu) { cpu->ld(cpu->H, cpu->B); },  // 0x60 LD H,B
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->C); },  // 0x61 LD H,C
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->D); },  // 0x62 LD H,D
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->E); },  // 0x63 LD H,E
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->H); },  // 0x64 LD H,H
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->L); },  // 0x65 LD H,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->H); },            // 0x66 LD H,(HL)
        [](CPU *cpu) { cpu->ld(cpu->H, cpu->A); },  // 0x67 LD H,A

        [](CPU *cpu) { cpu->ld(cpu->L, cpu->B); },  // 0x68 LD L,B
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->C); },  // 0x69 LD L,C
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->D); },  // 0x6A LD L,D
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->E); },  // 0x6B LD L,E
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->H); },  // 0x6C LD L,H
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->L); },  // 0x6D LD L,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->L); },            // 0x6E LD L,(HL)
        [](CPU *cpu) { cpu->ld(cpu->L, cpu->A); },  // 0x6F LD L,A

        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->B); },                // 0x70 LD (HL),B
        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->C); },                // 0x71 LD (HL),C
        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->D); },                // 0x72 LD (HL),D
        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->E); },                // 0x73 LD (HL),E
        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->H); },                // 0x74 LD (HL),H
        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->L); },                // 0x75 LD (HL),L

        [](CPU *cpu) {},                            // 0x76 HALT

        [](CPU *cpu) { cpu->ldMemHL_Reg8(cpu->A); },                 // 0x77 LD (HL),A

        [](CPU *cpu) { cpu->ld(cpu->A, cpu->B); },  // 0x78 LD A,B
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->C); },  // 0x79 LD A,C
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->D); },  // 0x7A LD A,D
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->E); },  // 0x7B LD A,E
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->H); },  // 0x7C LD A,H
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->L); },  // 0x7D LD A,L
        [](CPU *cpu) { cpu->ldReg8_MemHL(cpu->A); },  // 0x7E LD A,(HL)
        [](CPU *cpu) { cpu->ld(cpu->A, cpu->A); },  // 0x7F LD A,A

        [](CPU *cpu) { cpu->add(cpu->B); },     // 0x80
        [](CPU *cpu) { cpu->add(cpu->C); },     // 0x81
        [](CPU *cpu) { cpu->add(cpu->D); },     // 0x82
        [](CPU *cpu) { cpu->add(cpu->E); },     // 0x83
        [](CPU *cpu) { cpu->add(cpu->H); },     // 0x84
        [](CPU *cpu) { cpu->add(cpu->L); },     // 0x85
        [](CPU *cpu) { /* uint8_t valueAtHL = readMemory(HL); add(valueAtHL); */ },     // 0x86
        [](CPU *cpu) { cpu->add_a_a(); },       // 0x87

        [](CPU *cpu) { cpu->adc(cpu->B); },     // 0x88
        [](CPU *cpu) { cpu->adc(cpu->C); },     // 0x89
        [](CPU *cpu) { cpu->adc(cpu->D); },     // 0x8A
        [](CPU *cpu) { cpu->adc(cpu->E); },     // 0x8B
        [](CPU *cpu) { cpu->adc(cpu->H); },     // 0x8C
        [](CPU *cpu) { cpu->adc(cpu->L); },     // 0x8D
        [](CPU *cpu) { /* uint8_t valueAtHL = readMemory(HL); adc(valueAtHL); */ },     // 0x8E
        [](CPU *cpu) { cpu->adc_a_a(); },       // 0x8F

        [](CPU *cpu) { cpu->sub(cpu->B); },     // 0x90
        [](CPU *cpu) { cpu->sub(cpu->C); },     // 0x91
        [](CPU *cpu) { cpu->sub(cpu->D); },     // 0x92
        [](CPU *cpu) { cpu->sub(cpu->E); },     // 0x93
        [](CPU *cpu) { cpu->sub(cpu->H); },     // 0x94
        [](CPU *cpu) { cpu->sub(cpu->L); },     // 0x95
        [](CPU *cpu) { /* uint8_t valueAtHL = readMemory(HL); adc(valueAtHL); */ },     // 0x96
        [](CPU *cpu) { cpu->sub_a_a(); },       // 0x97

        [](CPU *cpu) { cpu->sbc(cpu->B); },     // 0x98
        [](CPU *cpu) { cpu->sbc(cpu->C); },     // 0x99
        [](CPU *cpu) { cpu->sbc(cpu->D); },     // 0x9A
        [](CPU *cpu) { cpu->sbc(cpu->E); },     // 0x9B
        [](CPU *cpu) { cpu->sbc(cpu->H); },     // 0x9C
        [](CPU *cpu) { cpu->sbc(cpu->L); },     // 0x9D
        [](CPU *cpu) { },                       // 0x9E
        [](CPU *cpu) { cpu->sbc_a_a(); },       // 0x9F

        [](CPU *cpu) { cpu->and_op(cpu->B); },       // 0xA0
        [](CPU *cpu) { cpu->and_op(cpu->C); },       // 0xA1
        [](CPU *cpu) { cpu->and_op(cpu->D); },       // 0xA2
        [](CPU *cpu) { cpu->and_op(cpu->E); },       // 0xA3
        [](CPU *cpu) { cpu->and_op(cpu->H); },       // 0xA4
        [](CPU *cpu) { cpu->and_op(cpu->L); },       // 0xA5
        [](CPU *cpu) { },       // 0xA6
        [](CPU *cpu) { cpu->and_a_a(); },            // 0xA7

        [](CPU *cpu) { cpu->xor_op(cpu->B); },       // 0xA8
        [](CPU *cpu) { cpu->xor_op(cpu->C); },       // 0xA9
        [](CPU *cpu) { cpu->xor_op(cpu->D); },       // 0xAA
        [](CPU *cpu) { cpu->xor_op(cpu->E); },       // 0xAB
        [](CPU *cpu) { cpu->xor_op(cpu->H); },       // 0xAC
        [](CPU *cpu) { cpu->xor_op(cpu->L); },       // 0xAD
        [](CPU *cpu) {  },                           // 0xAE
        [](CPU *cpu) { cpu->xor_a_a(); },            // 0xAF

        [](CPU *cpu) { cpu->or_op(cpu->B); },        // 0xB0
        [](CPU *cpu) { cpu->or_op(cpu->C); },        // 0xB1
        [](CPU *cpu) { cpu->or_op(cpu->D); },        // 0xB2
        [](CPU *cpu) { cpu->or_op(cpu->E); },        // 0xB3
        [](CPU *cpu) { cpu->or_op(cpu->H); },        // 0xB4
        [](CPU *cpu) { cpu->or_op(cpu->L); },        // 0xB5
        [](CPU *cpu) {  },                           // 0xB6
        [](CPU *cpu) { cpu->or_a_a(); },             // 0xB7

        [](CPU *cpu) { cpu->cp(cpu->B); },           // 0xB8
        [](CPU *cpu) { cpu->cp(cpu->C); },           // 0xB9
        [](CPU *cpu) { cpu->cp(cpu->D); },           // 0xBA
        [](CPU *cpu) { cpu->cp(cpu->E); },           // 0xBB
        [](CPU *cpu) { cpu->cp(cpu->H); },           // 0xBC
        [](CPU *cpu) { cpu->cp(cpu->L); },           // 0xBD
        [](CPU *cpu) {  },                           // 0xBE
        [](CPU *cpu) { cpu->cp_a_a(); },             // 0xBF
    };

    void CPU::reset() {
        AF = 0x01B0;    // A = 0x01, F = 0xB0 (initial flags for Gameboy)
        BC = 0x0013;    // B = 0x00, C = 0x13
        DE = 0x00D8;    // D = 0x00, E = 0xD8
        HL = 0x014D;    // H = 0x01, L = 0x4D

        PC = 0x0100;    // Program counter starts at 0x0100 in most Gameboy ROMs
        SP = 0xFFFE;    // Stack pointer is initialized to 0xFFFE

        // Clear interrupt flags or any other control bits

        // Optionally, reset the state of internal flags in F
        F = 0xB0;  // Assuming this is the default flag register state (e.g., zero flag set)
    }

    void CPU::executeInstruction()
    {
        //fetch();
        //decode();
        //execute();
    }

    void CPU::decode()
    {

    }

    void CPU::incReg16(uint16_t &reg)
    {
        ++reg;
    }

    void CPU::incReg8(uint8_t &reg)
    {
        uint8_t newFlags = F & CARRY_FLAG_MASK;

        ++reg;
        newFlags |= ((reg == 0x00) ? ZERO_FLAG_MASK |
            HALF_CARRY_FLAG_MASK : (((reg & 0x0F) == 0x00) ? HALF_CARRY_FLAG_MASK : 0));
        F = newFlags;
    }

#warning just an exemple can be implemented well better
    void CPU::incMemHL()
    {
        uint8_t value = readMemory(HL);
        ++value;
        writeMemory(HL, value);

        F = (F & CARRY_FLAG_MASK) |
            ((value == 0) ? ZERO_FLAG_MASK : 0) |
            (((value & 0x0F) == 0x00) ? HALF_CARRY_FLAG_MASK : 0);
    }

    void CPU::decReg16(uint16_t &reg)
    {
        --reg;
    }

    void CPU::decReg8(uint8_t &reg)
    {
        uint8_t newFlags = CARRY_FLAG_MASK | SUBTRACT_FLAG_MASK;

        --reg;
        newFlags |= (reg == 0x00) ? ZERO_FLAG_MASK :
            ((reg & 0x0F) == 0x0F) ? HALF_CARRY_FLAG_MASK : 0;
        F = newFlags;
    }

    void CPU::decMemHL()
    {
        uint8_t value = readMemory(HL);
        --value;
        writeMemory(HL, value);

        F = (F & CARRY_FLAG_MASK) |
            SUBTRACT_FLAG_MASK |
            ((value == 0) ? ZERO_FLAG_MASK : 0) |
            (((value & 0x0F) == 0x00) ? HALF_CARRY_FLAG_MASK : 0);
    }

    void CPU::ld(uint8_t& dest, const uint8_t src)
    {
        dest = src;
    }

    void CPU::ldReg16_d16(uint16_t& reg)
    {
        reg = readNextWord();
    }

    void CPU::ldMemReg16_A(uint16_t &addr)
    {
        writeMemory(addr, A);
    }

    void CPU::ldReg8_d8(uint8_t &reg)
    {
        reg = readNextByte();
    }

    void CPU::ldMemA16_SP()
    {

    }

    void CPU::ldA_MemReg16(uint16_t& reg)
    {
        A = readMemory(reg);
    }

    void CPU::ldMemHLplus_A()
    {
        writeMemory(HL, A);
        ++HL;
    }

    void CPU::ldA_MemHLplus()
    {
        A = readMemory(HL);
        ++HL;
    }

    void CPU::ldMemHLminus_A() {
        writeMemory(HL, A);
        HL--;
    }

    void CPU::ldMemHL_d8()
    {
#warning maybe bad
        writeMemory(HL, readNextByte());
    }

    void CPU::ldA_MemHLminus()
    {
        A = readMemory(HL);
        --HL;
    }

    void CPU::ldReg8_MemHL(uint8_t &dest)
    {
        uint8_t valueAtMem = readMemory(HL);

        dest = valueAtMem;
    }

    void CPU::ldMemHL_Reg8(const uint8_t val)
    {
        writeMemory(HL, val);
    }

    void CPU::add(const uint8_t reg) {
        const uint16_t result = A + reg;

        //newFlags |= ((A ^ reg ^ result) & 0x10) ? HALF_CARRY_FLAG_MASK : 0;
        uint8_t newFlags = (((A & 0xF) + (reg & 0xF)) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }

    void CPU::add_a_a() {
        const uint16_t result = A + A;

        uint8_t newFlags = ((A & 0xF) > 0x07) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }

    void CPU::addHL_Reg16(const uint16_t reg)
    {
        uint32_t result = HL + reg;

        uint8_t newFlags = F & ZERO_FLAG_MASK;

        newFlags |= (((HL & 0x0FFF) + (reg & 0x0FFF) >  0x7FF) ? HALF_CARRY_FLAG_MASK : 0) |
            ((result > 0xFFFF) ? CARRY_FLAG_MASK : 0);

        HL = result & 0xFFFF;

        F = newFlags;
    }

    void CPU::addHL_HL()
    {
        const uint16_t result = HL + HL;

        uint8_t newFlags = F & ZERO_FLAG_MASK;

        newFlags |= (((HL & 0x0FFF) > 0x07FF) ? HALF_CARRY_FLAG_MASK : 0) |
            ((result > 0xFFFF) ? CARRY_FLAG_MASK : 0);

        HL = result & 0xFFFF;

        F = newFlags;
    }

    void CPU::adc(const uint8_t reg) {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A + reg + carry;

        uint8_t newFlags = (((A & 0xF) + (reg & 0xF) + carry) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }

    void CPU::adc_a_a() {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A + A + carry;

        uint8_t newFlags = (((2 * (A & 0xF)) + carry) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }

    void CPU::sub(const uint8_t reg) {
        const uint16_t result = A - reg;
        uint8_t newFlags = SUBTRACT_FLAG_MASK |
            (((A & 0xF) < (reg & 0xF)) ? HALF_CARRY_FLAG_MASK : 0) |
            ((A < reg) ? CARRY_FLAG_MASK : 0);

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::sub_a_a() {
        A = 0;
        F = SUBTRACT_FLAG_MASK | ZERO_FLAG_MASK;
    }

    void CPU::sbc(const uint8_t reg) {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A - reg - carry;

        uint8_t newFlags = SUBTRACT_FLAG_MASK |
            (((A & 0xF) < ((reg & 0xF) + carry)) ? HALF_CARRY_FLAG_MASK : 0) |
            ((A < (reg + carry)) ? CARRY_FLAG_MASK : 0);

        A = result & 0xFF;

        F = newFlags | ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::sbc_a_a() {
        const uint8_t carry = getCarryFlag();

        A = carry ? 0xFF : 0x00;

        F = SUBTRACT_FLAG_MASK |
            (carry ? CARRY_FLAG_MASK : 0) |
            ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::and_op(const uint8_t reg)
    {
        A &= reg;

        F = HALF_CARRY_FLAG_MASK |
            ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::and_a_a()
    {
        F = HALF_CARRY_FLAG_MASK | ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::xor_op(const uint8_t reg)
    {
        A ^= reg;
        F = ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::xor_a_a()
    {
        A = 0;
        F = ZERO_FLAG_MASK;
    }

    void CPU::or_op(const uint8_t reg)
    {
        A |= reg;
        F = ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::or_a_a()
    {
        F = ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::cp(const uint8_t reg)
    {
        // Implicit comparison of A with reg, without modifying A
        F = SUBTRACT_FLAG_MASK |
            (((A & 0xF) < (reg & 0xF)) ? HALF_CARRY_FLAG_MASK : 0) |
            ((A < reg) ? CARRY_FLAG_MASK : 0) |
            ((A == reg) ? ZERO_FLAG_MASK : 0);
    }

    void CPU::cp_a_a()
    {
        F = SUBTRACT_FLAG_MASK | ZERO_FLAG_MASK;
    }
}
