/* ================================================================
 * Project: Gameboy Color Emulator
 * File: cpu.hpp
 * Description: This file contains the declaration of the CPU
 *              class, which emulates the Z80-like processor used
 *              in the Gameboy and Gameboy Color. It includes the
 *              core CPU registers, flags, and the instruction set
 *              needed to simulate the behavior of the processor.
 *
 * Author: Guillaume MICHEL
 * Created on: September 24, 2024
 *
 * License: MIT License
 *
 * Copyright (c) 2024 Guillaume MICHEL
 * ================================================================
 */


#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>
#include <iostream>

constexpr uint8_t ZERO_FLAG_MASK = 0x80;  // Bit 7
constexpr uint8_t SUBTRACT_FLAG_MASK = 0x40;  // Bit 6
constexpr uint8_t HALF_CARRY_FLAG_MASK = 0x20;  // Bit 5
constexpr uint8_t CARRY_FLAG_MASK = 0x10;  // Bit 4

namespace emulator
{
    class CPU
    {
    public:
        CPU();
        ~CPU() = default;

        // Method to execute a single instruction
        void executeInstruction();

        // Method to reset the CPU (initial state)
        void reset();

        void add(const uint8_t&);

        void ADD_A_B() { add(B); }
        void ADD_A_C() { add(C); }
        void ADD_A_D() { add(D); }
        void ADD_A_E() { add(E); }
        void ADD_A_H() { add(H); }
        void ADD_A_L() { add(L); }
        void ADD_A_HL() { /* uint8_t valueAtHL = readMemory(HL); add(valueAtHL); */ }
        void ADD_A_A();
        void ADD_A_n() { /* uint8_t immediateValue = fetchNextByte(); add(immediateValue); */ }

        void adc(const uint8_t&);

        void ADC_A_B() {adc(B);}
        void ADC_A_C() {adc(C);}
        void ADC_A_D() {adc(D);}
        void ADC_A_E() {adc(E);}
        void ADC_A_H() {adc(H);}
        void ADC_A_L() {adc(L);}
        void ADC_A_HL() { /* uint8_t valueAtHL = readMemory(HL); adc(valueAtHL); */ }
        void ADC_A_A();
        void ADC_A_n() { /* uint8_t immediateValue = fetchNextByte(); adc(immediateValue); */ }

        void sub(const uint8_t&);

        void SUB_A_B() {sub(B);}
        void SUB_A_C() {sub(C);}
        void SUB_A_D() {sub(D);}
        void SUB_A_E() {sub(E);}
        void SUB_A_H() {sub(H);}
        void SUB_A_L() {sub(L);}
        void SUB_A_HL() { /* uint8_t valueAtHL = readMemory(HL); adc(valueAtHL); */ }
        void SUB_A_A();
        void SUB_A_n() { /* uint8_t immediateValue = fetchNextByte(); adc(immediateValue); */ }

        void sbc(const uint8_t&);

        void SBC_A_B() {sbc(B);}
        void SBC_A_C() {sbc(C);}
        void SBC_A_D() {sbc(D);}
        void SBC_A_E() {sbc(E);}
        void SBC_A_H() {sbc(H);}
        void SBC_A_L() {sbc(L);}
        void SBC_A_HL() { /* uint8_t valueAtHL = readMemory(HL); adc(valueAtHL); */ }
        void SBC_A_A();
        void SBC_A_n() { /* uint8_t immediateValue = fetchNextByte(); adc(immediateValue); */ }


        [[nodiscard]] uint8_t getA() const { return A; }
        void setA(const uint8_t value) { A = value; }

        [[nodiscard]] uint8_t getB() const { return B; }
        void setB(const uint8_t value) { B = value; }

        [[nodiscard]] bool getZeroFlag() const { return F & ZERO_FLAG_MASK; }
        [[nodiscard]] bool getSubtractFlag() const { return F & SUBTRACT_FLAG_MASK; }
        [[nodiscard]] bool getHalfCarryFlag() const { return F & HALF_CARRY_FLAG_MASK; }
        [[nodiscard]] bool getCarryFlag() const { return F & CARRY_FLAG_MASK; }


        [[nodiscard]] uint8_t getFlags() const { return F; }

        void setZeroFlag(const bool value) { F = value ? (F | ZERO_FLAG_MASK) : (F & ~ZERO_FLAG_MASK); }
        void setSubtractFlag(const bool value) { F = value ? (F | SUBTRACT_FLAG_MASK) : (F & ~SUBTRACT_FLAG_MASK); }
        void setHalfCarryFlag(const bool value) { F = value ? (F | HALF_CARRY_FLAG_MASK) : (F & ~HALF_CARRY_FLAG_MASK); }
        void setCarryFlag(const bool value) { F = value ? (F | CARRY_FLAG_MASK) : (F & ~CARRY_FLAG_MASK); }
        // F = (F & ~CARRY_FLAG_MASK) | (value * CARRY_FLAG_MASK);

        void clearCarryFlag() { F &= ~CARRY_FLAG_MASK; }

    private:
        typedef void (CPU::*Instruction)();

        Instruction instruction_table[256] = {
            [0x80] = &CPU::ADD_A_B,   // ADD A, B
            [0x81] = &CPU::ADD_A_C,   // ADD A, C
            [0x82] = &CPU::ADD_A_D,   // ADD A, D
            [0x83] = &CPU::ADD_A_E,   // ADD A, E
            [0x84] = &CPU::ADD_A_H,   // ADD A, H
            [0x85] = &CPU::ADD_A_L,   // ADD A, L
            [0x86] = &CPU::ADD_A_HL,  // ADD A, (HL)
            [0x87] = &CPU::ADD_A_A,   // ADD A, A

            [0x88] = &CPU::ADC_A_B,   // ADC A, B
            [0x89] = &CPU::ADC_A_C,   // ADC A, C
            [0x8A] = &CPU::ADC_A_D,   // ADC A, D
            [0x8B] = &CPU::ADC_A_E,   // ADC A, E
            [0x8C] = &CPU::ADC_A_H,   // ADC A, H
            [0x8D] = &CPU::ADC_A_L,   // ADC A, L
            [0x8E] = &CPU::ADC_A_HL,  // ADC A, (HL)
            [0x8F] = &CPU::ADC_A_A,   // ADC A, A

            [0x90] = &CPU::SUB_A_B,   // SUB A, B
            [0x91] = &CPU::SUB_A_C,   // SUB A, C
            [0x92] = &CPU::SUB_A_D,   // SUB A, D
            [0x93] = &CPU::SUB_A_E,   // SUB A, E
            [0x94] = &CPU::SUB_A_H,   // SUB A, H
            [0x95] = &CPU::SUB_A_L,   // SUB A, L
            [0x96] = &CPU::SUB_A_HL,  // SUB A, (HL)
            [0x97] = &CPU::SUB_A_A,   // SUB A, A

            [0x98] = &CPU::SBC_A_B,   // SBC A, B
            [0x99] = &CPU::SBC_A_C,   // SBC A, C
            [0x9A] = &CPU::SBC_A_D,   // SBC A, D
            [0x9B] = &CPU::SBC_A_E,   // SBC A, E
            [0x9C] = &CPU::SBC_A_H,   // SBC A, H
            [0x9D] = &CPU::SBC_A_L,   // SBC A, L
            [0x9E] = &CPU::SBC_A_HL,  // SBC A, (HL)
            [0x9F] = &CPU::SBC_A_A,   // SBC A, A

            [0xC6] = &CPU::ADD_A_n,   // ADD A, d8
        };

        // Registers
        union {
            struct {
                uint8_t F; // Flags
                uint8_t A; // Accumulator
            };
            uint16_t AF; // 16-bit pair register
        };

        union {
            struct {
                uint8_t C;
                uint8_t B;
            };
            uint16_t BC;
        };

        union {
            struct {
                uint8_t E;
                uint8_t D;
            };
            uint16_t DE;
        };

        union {
            struct {
                uint8_t L;
                uint8_t H;
            };
            uint16_t HL;
        };

        uint16_t PC; // Program counter
        uint16_t SP; // Stack pointer

        // Methods to handle CPU instructions
        void fetch() {}; // Fetch the next instruction
        void decode(); // Decode the fetched instruction
        void execute() {}; // Execute the decoded instruction

        // Helper methods for instruction decoding
        void handle_opcodes(uint8_t opcode);
    };
}

#endif // CPU_HPP
