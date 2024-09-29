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
    CPU::CPU():
    instruction_table(initialize_instruction_table()), AF(), BC(), DE(),
    HL(),     // Program starts at 0x0100 in most GB ROMs
    PC(0x0100),      // Stack Pointer initialization
    SP(0xFFFE)
    {

    }

    constexpr std::array<CPU::Instruction, 256> CPU::initialize_instruction_table() {
        std::array<Instruction, 256> table = {};

        table[0x80] = &CPU::ADD_A_B;
        table[0x81] = &CPU::ADD_A_C;
        table[0x82] = &CPU::ADD_A_D;
        table[0x83] = &CPU::ADD_A_E;
        table[0x84] = &CPU::ADD_A_H;
        table[0x85] = &CPU::ADD_A_L;
        table[0x86] = &CPU::ADD_A_HL;
        table[0x87] = &CPU::ADD_A_A;

        table[0x88] = &CPU::ADC_A_B;
        table[0x89] = &CPU::ADC_A_C;
        table[0x8A] = &CPU::ADC_A_D;
        table[0x8B] = &CPU::ADC_A_E;
        table[0x8C] = &CPU::ADC_A_H;
        table[0x8D] = &CPU::ADC_A_L;
        table[0x8E] = &CPU::ADC_A_HL;
        table[0x8F] = &CPU::ADC_A_A;

        table[0x90] = &CPU::SUB_A_B;
        table[0x91] = &CPU::SUB_A_C;
        table[0x92] = &CPU::SUB_A_D;
        table[0x93] = &CPU::SUB_A_E;
        table[0x94] = &CPU::SUB_A_H;
        table[0x95] = &CPU::SUB_A_L;
        table[0x96] = &CPU::SUB_A_HL;
        table[0x97] = &CPU::SUB_A_A;

        table[0x98] = &CPU::SBC_A_B;
        table[0x99] = &CPU::SBC_A_C;
        table[0x9A] = &CPU::SBC_A_D;
        table[0x9B] = &CPU::SBC_A_E;
        table[0x9C] = &CPU::SBC_A_H;
        table[0x9D] = &CPU::SBC_A_L;
        table[0x9E] = &CPU::SBC_A_HL;
        table[0x9F] = &CPU::SBC_A_A;

        table[0xC6] = &CPU::ADD_A_n;

        return table;
    }

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
        fetch();
        decode();
        execute();
    }

    void CPU::decode()
    {

    }

    inline void CPU::add(const uint8_t& reg) {
        const uint16_t result = A + reg;

        //newFlags |= ((A ^ reg ^ result) & 0x10) ? HALF_CARRY_FLAG_MASK : 0;
        uint8_t newFlags = (((A & 0xF) + (reg & 0xF)) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }


    inline void CPU::ADD_A_A() {
        const uint16_t result = A + A;

        uint8_t newFlags = ((A & 0xF) > 0x07) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }



    inline void CPU::adc(const uint8_t& reg) {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A + reg + carry;

        uint8_t newFlags = (((A & 0xF) + (reg & 0xF) + carry) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }


    inline void CPU::ADC_A_A() {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A + A + carry;

        uint8_t newFlags = (((2 * (A & 0xF)) + carry) > 0xF) ? HALF_CARRY_FLAG_MASK : 0;

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0) |
            ((result & 0x100) ? CARRY_FLAG_MASK : 0);
    }

    inline void CPU::sub(const uint8_t& reg) {
        const uint16_t result = A - reg;
        uint8_t newFlags = SUBTRACT_FLAG_MASK |
            (((A & 0xF) < (reg & 0xF)) ? HALF_CARRY_FLAG_MASK : 0) |
            ((A < reg) ? CARRY_FLAG_MASK : 0);

        A = result & 0xFF;

        F = newFlags |
            ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    inline void CPU::SUB_A_A() {
        A = 0;
        F = SUBTRACT_FLAG_MASK | ZERO_FLAG_MASK;
    }

    inline void CPU::sbc(const uint8_t& reg) {
        const uint8_t carry = getCarryFlag();
        const uint16_t result = A - reg - carry;

        uint8_t newFlags = SUBTRACT_FLAG_MASK |
            (((A & 0xF) < ((reg & 0xF) + carry)) ? HALF_CARRY_FLAG_MASK : 0) |
            ((A < (reg + carry)) ? CARRY_FLAG_MASK : 0);

        A = result & 0xFF;

        F = newFlags | ((A == 0) ? ZERO_FLAG_MASK : 0);
    }

    inline void CPU::SBC_A_A() {
        const uint8_t carry = getCarryFlag();

        A = carry ? 0xFF : 0x00;

        F = SUBTRACT_FLAG_MASK |
            (carry ? CARRY_FLAG_MASK : 0) |
            ((A == 0) ? ZERO_FLAG_MASK : 0);
    }


}
