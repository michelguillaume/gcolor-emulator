#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUSubTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for simple subtraction without any flag triggered
TEST_F(CPUSubTest, SUB_A_B_NoFlags) {
    cpu.setA(5);
    cpu.setB(3);
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 2);                     // A should be 2
    EXPECT_EQ(cpu.getFlags(), SUBTRACT_FLAG_MASK);  // Only subtract flag should be set
}

// Test for subtraction that triggers the carry flag (borrow from 8 bits)
TEST_F(CPUSubTest, SUB_A_B_CarryFlag) {
    cpu.setA(0x00);  // Minimum 8-bit value
    cpu.setB(0x01);  // Subtracting 1 causes an underflow (borrow)
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0xFF);                  // Result should wrap to 0xFF
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for subtraction that triggers the half-carry flag (borrow from bit 4)
TEST_F(CPUSubTest, SUB_A_B_HalfCarryFlag) {
    cpu.setA(0x10);  // 00010000
    cpu.setB(0x01);  // Subtracting 1 causes half-carry
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x0F);                  // A should be 0x0F
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for subtraction that results in zero (sets zero flag)
TEST_F(CPUSubTest, SUB_A_B_ZeroFlag) {
    cpu.setA(0x05);
    cpu.setB(0x05);
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for subtraction that sets carry but not half-carry flags
TEST_F(CPUSubTest, SUB_A_B_CarryWithoutHalfCarry) {
    cpu.setA(0x10);  // 00010000
    cpu.setB(0x20);  // Subtracting 0x20 causes carry but no half-carry
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0xF0);                  // A should be 0xF0
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for subtracting zero (should not affect any flags or change A)
TEST_F(CPUSubTest, SUB_A_B_SubtractZero) {
    cpu.setA(0x45);  // Any non-zero value
    cpu.setB(0x00);  // Subtracting zero
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x45);                  // A should remain unchanged
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for subtracting small values (no flags triggered)
TEST_F(CPUSubTest, SUB_A_B_SmallValues) {
    cpu.setA(3);
    cpu.setB(1);
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 2);                     // A should be 2
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for subtracting maximum value without borrow (carry flag should not be set)
TEST_F(CPUSubTest, SUB_A_B_MaxValueWithoutCarry) {
    cpu.setA(0xFF);  // 255
    cpu.setB(0x01);  // Subtracting 1
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0xFE);                  // A should be 0xFE
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for clearing flags after they are previously set
TEST_F(CPUSubTest, SUB_A_B_ClearFlagsAfterSet) {
    cpu.setA(0x00);  // First trigger flags
    cpu.setB(0x01);
    cpu.execute(0x90);

    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set

    cpu.setA(5);  // Now clear flags by doing a simple subtraction
    cpu.setB(2);
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 3);                     // A should be 3
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should be cleared
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should be cleared
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should be cleared
}

// Test for edge case: subtracting values that result in borrow but not zero
TEST_F(CPUSubTest, SUB_A_B_CarryWithoutZero) {
    cpu.setA(0x80);  // 10000000
    cpu.setB(0x81);  // Subtracting a larger value
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0xFF);                  // A should wrap to 0xFF
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for carry and half-carry on small subtraction (nibble borrow)
TEST_F(CPUSubTest, SUB_A_B_HalfCarryOnSmallSubtraction) {
    cpu.setA(0x10);  // 00010000
    cpu.setB(0x01);  // Subtracting 1 causes half-carry
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x0F);                  // A should be 0x0F
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for simple subtraction without any flag triggered for SUB_A_A
TEST_F(CPUSubTest, SUB_A_A_NoFlags) {
    cpu.setA(5);
    cpu.sub_a_a();

    EXPECT_EQ(cpu.getA(), 0);                     // A should be 0
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_EQ(cpu.getFlags(), SUBTRACT_FLAG_MASK | ZERO_FLAG_MASK);  // Only zero and subtract flags should be set
}

// Test for subtraction that triggers the carry flag for SUB_A_A
TEST_F(CPUSubTest, SUB_A_A_CarryFlag) {
    cpu.setA(0x01);  // Minimum non-zero value
    cpu.sub_a_a();

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should not be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
}

// Test for subtraction that triggers the half-carry flag for SUB_A_A
TEST_F(CPUSubTest, SUB_A_A_HalfCarryFlag) {
    cpu.setA(0x10);
    cpu.sub_a_a();

    EXPECT_EQ(cpu.getA(), 0);                     // A should be 0x00
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should not be set (no half carry)
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
}

// Test for clearing flags after they are previously set for SUB_A_A
TEST_F(CPUSubTest, SUB_A_A_ClearFlagsAfterSet) {
    cpu.setA(0x01);  // First trigger flags
    cpu.sub_a_a();

    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should not be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set

    cpu.setA(5);  // Now clear flags by doing a simple subtraction
    cpu.sub_a_a();

    EXPECT_EQ(cpu.getA(), 0);                     // A should be 0
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should be cleared
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
}

// Test for SUB_A_A with subtracting small values
TEST_F(CPUSubTest, SUB_A_B_SmallSubtraction) {
    cpu.setA(3);  // Small value
    cpu.sub_a_a();  // Subtract A from A

    EXPECT_EQ(cpu.getA(), 0);                    // A should become 0
    EXPECT_TRUE(cpu.getZeroFlag());              // Zero flag should be set
    EXPECT_EQ(cpu.getFlags(), SUBTRACT_FLAG_MASK | ZERO_FLAG_MASK);  // Subtract and zero flags should be set
}

TEST_F(CPUSubTest, SUB_A_B_MaxValuesWithCarryAndNoHalfCarry) {
    cpu.setA(0xFF);  // Set A to 255 (0xFF)
    cpu.setB(0xFE);  // Set B to 254 (0xFE)

    cpu.execute(0x90);  // Perform A = A - B

    EXPECT_EQ(cpu.getA(), 0x01);                  // A should be 0x01
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set (A > B, no borrow)
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set (no nibble borrow)
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set (result is not zero)
}

TEST_F(CPUSubTest, SUB_A_B_SubtractZeroOnZero) {
    cpu.setA(0x00);
    cpu.setB(0x00);
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should remain 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

TEST_F(CPUSubTest, SUB_A_B_SignedOverflowBehavior) {
    cpu.setA(0x80);  // -128 en signé
    cpu.setB(0x01);  // Soustraction de 1
    cpu.execute(0x90);

    EXPECT_EQ(cpu.getA(), 0x7F);                  // A should be 127
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}
