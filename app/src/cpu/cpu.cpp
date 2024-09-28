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
    AF(), BC(), DE(), HL(),
    PC(0x0100),     // Program starts at 0x0100 in most GB ROMs
    SP(0xFFFE)      // Stack Pointer initialization
    {

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
