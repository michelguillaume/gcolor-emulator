#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUXorTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for XOR between two registers, with result non-zero
TEST_F(CPUXorTest, XOR_A_B_NonZero) {
    cpu.setA(0xF0);  // A = 11110000
    cpu.setB(0x0F);  // B = 00001111
    cpu.execute(0xA8);  // XOR A, B (A = A ^ B)

    EXPECT_EQ(cpu.getA(), 0xFF);                  // A should become 0xFF
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for XOR that results in zero
TEST_F(CPUXorTest, XOR_A_B_ZeroFlag) {
    cpu.setA(0xFF);  // A = 11111111
    cpu.setB(0xFF);  // B = 11111111
    cpu.execute(0xA8);  // XOR A, B (A = A ^ B)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

/*
// Test for XOR with immediate value
TEST_F(CPUXorTest, XOR_A_Immediate) {
    cpu.setA(0x0F);  // A = 00001111
    cpu.execute(0xEE);  // XOR A, #0xF0 (A = A ^ 0xF0)

    EXPECT_EQ(cpu.getA(), 0xFF);                  // A should become 0xFF
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}
*/

// Test for XOR with itself (XOR A, A)
TEST_F(CPUXorTest, XOR_A_A_ZeroResult) {
    cpu.setA(0x45);  // A = 01000101
    cpu.xor_a_a();   // XOR A, A (A = A ^ A)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for XOR between two registers that do not result in zero
TEST_F(CPUXorTest, XOR_A_B_DifferentPattern) {
    cpu.setA(0xAA);  // A = 10101010
    cpu.setB(0x55);  // B = 01010101
    cpu.execute(0xA8);  // XOR A, B (A = A ^ B)

    EXPECT_EQ(cpu.getA(), 0xFF);                  // A should become 0xFF
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

/*

// Test for XOR with zero (XOR A, 0)
TEST_F(CPUXorTest, XOR_A_Zero) {
    cpu.setA(0x45);  // A = 01000101
    cpu.execute(0xEE);  // XOR A, #0x00 (A = A ^ 0x00)

    EXPECT_EQ(cpu.getA(), 0x45);                  // A should remain unchanged
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

*/
