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
#include <array>

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

        void execute(uint8_t opcode)
        {
            instruction_table[opcode](this);
        }; // Execute the decoded instruction

        // Method to reset the CPU (initial state)
        void reset();

        void inc16(uint16_t&);

        void inc8(uint8_t&);

        void dec8(uint8_t&);

        void dec16(uint16_t&);

        void ld(uint8_t&, uint8_t);

        void ldFromMem(uint8_t&, uint16_t);

        void ldToMem(uint16_t, uint8_t);

        void add(uint8_t);
        void add_a_a();

        void adc(uint8_t);
        void adc_a_a();

        void sub(uint8_t);
        void sub_a_a();

        void sbc(uint8_t);
        void sbc_a_a();

        void and_op(uint8_t);
        void and_a_a();

        void xor_op(uint8_t);
        void xor_a_a();

        void or_op(uint8_t);
        void or_a_a();

        void cp(uint8_t);
        void cp_a_a();


        void ADD_A_n() { /* uint8_t immediateValue = fetchNextByte(); add(immediateValue); */ }
        void ADC_A_n() { /* uint8_t immediateValue = fetchNextByte(); adc(immediateValue); */ }
        void SUB_A_n() { /* uint8_t immediateValue = fetchNextByte(); adc(immediateValue); */ }
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

        void clearFlags() { F = 0; }

    private:
        static std::array<void (*)(CPU*), 256> instruction_table;

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

        uint8_t readMemory(const uint8_t reg) { return 0; }
        void writeMemory(uint16_t addr, uint8_t val) { /* memory[addr] = val; */}

        // Helper methods for instruction decoding
        // void handle_opcodes(uint8_t opcode);
    };
}

#endif // CPU_HPP
