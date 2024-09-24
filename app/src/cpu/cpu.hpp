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

const uint8_t ZERO_FLAG_MASK = 0x80;  // Bit 7
const uint8_t SUBTRACT_FLAG_MASK = 0x40;  // Bit 6
const uint8_t HALF_CARRY_FLAG_MASK = 0x20;  // Bit 5
const uint8_t CARRY_FLAG_MASK = 0x10;  // Bit 4

namespace emulator
{
    class CPU
    {
    public:
        CPU();
        ~CPU() = default;

        // Method to execute a single instruction
        void execute_instruction();

        // Method to reset the CPU (initial state)
        void reset();

        void ADD_A_B();
        void ADD_A_C();
        void ADD_A_D();
        void ADD_A_E();
        void ADD_A_H();
        void ADD_A_L();
        void ADD_A_HL();  // This one operates on memory pointed by HL
        void ADD_A_A();
        void ADD_A_n();   // This one adds an immediate value to A

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
        void fetch(); // Fetch the next instruction
        void decode(); // Decode the fetched instruction
        void execute(); // Execute the decoded instruction

        // Helper methods for instruction decoding
        void handle_opcodes(uint8_t opcode);

        // Flags manipulation
        void set_zero_flag(bool);
        void set_subtract_flag(bool);
        void set_half_carry_flag(bool);
        void set_carry_flag(bool);

        bool get_zero_flag() const;
        bool get_subtract_flag() const;
        bool get_half_carry_flag() const;
        bool get_carry_flag() const;
    };
}

#endif // CPU_HPP
