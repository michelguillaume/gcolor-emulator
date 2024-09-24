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

    void CPU::execute_instruction()
    {
        fetch();
        decode();
        execute();
    }

    void CPU::decode()
    {

    }

    void CPU::ADD_A_B()
    {
        uint8_t newFlags = 0;

        const uint16_t result = A + B;

        newFlags |= ((A ^ B ^ result) & 0x10) ? HALF_CARRY_FLAG_MASK : 0;

        A = result;

        newFlags |= (A == 0) ? ZERO_FLAG_MASK : 0;

        newFlags |= (result & 0x100) ? CARRY_FLAG_MASK : 0;

        F = newFlags;
    }


    inline void CPU::set_zero_flag(const bool value)
    {
        if (value) {
            this->F |= ZERO_FLAG_MASK;
        } else {
            this->F &= ~ZERO_FLAG_MASK;
        }
    }

    inline void CPU::set_subtract_flag(const bool value)
    {
        if (value) {
            this->F |= SUBTRACT_FLAG_MASK;
        } else {
            this->F &= ~SUBTRACT_FLAG_MASK;
        }
    }

    inline void CPU::set_carry_flag(const bool value)
    {
        if (value) {
            this->F |= CARRY_FLAG_MASK;
        } else {
            this->F &= ~CARRY_FLAG_MASK;
        }
    }


    inline bool CPU::get_zero_flag() const
    {
        return (this->F & ZERO_FLAG_MASK) != 0;
    }

    inline bool CPU::get_subtract_flag() const
    {
        return (this->F & SUBTRACT_FLAG_MASK) != 0;
    }

    inline bool CPU::get_half_carry_flag() const
    {
        return (this->F & HALF_CARRY_FLAG_MASK) != 0;
    }

    inline bool CPU::get_carry_flag() const
    {
        return (this->F & CARRY_FLAG_MASK) != 0;
    }
}
