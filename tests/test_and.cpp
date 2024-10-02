#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUAndTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for AND with two registers, no flags triggered
TEST_F(CPUAndTest, AND_A_B_NoFlags) {
    cpu.setA(0xF0);  // A = 11110000
    cpu.setB(0x0F);  // B = 00001111
    cpu.execute(0xA0);  // AND A, B (A = A & B)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_EQ(cpu.getFlags(), ZERO_FLAG_MASK | HALF_CARRY_FLAG_MASK);  // Z and H flags set
}

// Test for AND with result non-zero
TEST_F(CPUAndTest, AND_A_B_NonZero) {
    cpu.setA(0xF0);  // A = 11110000
    cpu.setB(0xF0);  // B = 11110000
    cpu.execute(0xA0);  // AND A, B (A = A & B)

    EXPECT_EQ(cpu.getA(), 0xF0);                  // A should remain 0xF0
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test for AND resulting in zero
TEST_F(CPUAndTest, AND_A_B_ZeroFlag) {
    cpu.setA(0x00);  // A = 00000000
    cpu.setB(0xFF);  // B = 11111111
    cpu.execute(0xA0);  // AND A, B (A = A & B)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test for AND with a different pattern
TEST_F(CPUAndTest, AND_A_B_DifferentPattern) {
    cpu.setA(0xAA);  // A = 10101010
    cpu.setB(0x55);  // B = 01010101
    cpu.execute(0xA0);  // AND A, B (A = A & B)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test for AND with itself (AND A, A)
TEST_F(CPUAndTest, AND_A_A_NoChange) {
    cpu.setA(0x45);  // A = 01000101
    cpu.execute(0xA7);  // AND A, A (A = A & A)

    EXPECT_EQ(cpu.getA(), 0x45);                  // A should remain unchanged
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test for AND with itself resulting in zero
TEST_F(CPUAndTest, AND_A_A_Zero) {
    cpu.setA(0x00);  // A = 00000000
    cpu.execute(0xA7);  // AND A, A (A = A & A)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should remain 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

/*

// Test for AND with immediate value
TEST_F(CPUAndTest, AND_A_Immediate) {
    cpu.setA(0xF0);  // A = 11110000
    cpu.execute(0xE6);  // AND A, #0x0F (A = A & 0x0F)

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should become 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}
*/
