#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUAddTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for simple addition without any flag triggered
TEST_F(CPUAddTest, ADD_A_B_NoFlags) {
    cpu.setA(5);
    cpu.setB(3);
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 8);                     // A should be 8
    EXPECT_EQ(cpu.getFlags(), 0);                 // No flags should be set
}

// Test for addition that triggers the carry flag (overflow from 8 bits)
TEST_F(CPUAddTest, ADD_A_B_CarryFlag) {
    cpu.setA(0xFF);  // Maximum 8-bit value
    cpu.setB(0x01);  // Adding 1 causes an overflow
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x00);                  // Result should wrap to 0
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
}

// Test for addition that triggers the half-carry flag (carry from bit 3 to bit 4)
TEST_F(CPUAddTest, ADD_A_B_HalfCarryFlag) {
    cpu.setA(0x0F);  // Lower nibble is full (00001111)
    cpu.setB(0x01);  // Adding 1 causes half-carry
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x10);                  // A should be 0x10
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for addition that results in zero (sets zero flag)
TEST_F(CPUAddTest, ADD_A_B_ZeroFlag) {
    cpu.setA(0x00);
    cpu.setB(0x00);
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for addition that sets both carry and half-carry flags
TEST_F(CPUAddTest, ADD_A_B_CarryAndHalfCarryFlags) {
    cpu.setA(0x8F);  // 10001111
    cpu.setB(0x91);  // 10010001
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x20);                  // A should be 0x20
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for adding zero (should not affect any flags or change A)
TEST_F(CPUAddTest, ADD_A_B_AddZero) {
    cpu.setA(0x45);  // Any non-zero value
    cpu.setB(0x00);  // Adding zero
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x45);                  // A should remain unchanged
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for adding small values (no flags triggered)
TEST_F(CPUAddTest, ADD_A_B_SmallValues) {
    cpu.setA(1);
    cpu.setB(1);
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 2);                     // A should be 2
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for adding maximum value without overflow (should not set carry)
TEST_F(CPUAddTest, ADD_A_B_MaxValueWithoutOverflow) {
    cpu.setA(0xFE);  // 254
    cpu.setB(0x01);  // Adding 1
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0xFF);                  // A should be 0xFF
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for clearing flags after they are previously set
TEST_F(CPUAddTest, ADD_A_B_ClearFlagsAfterSet) {
    cpu.setA(0xFF);  // First trigger flags
    cpu.setB(0x01);
    cpu.execute(0x80);

    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set

    cpu.setA(2);  // Now clear flags by doing simple addition
    cpu.setB(3);
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 5);                     // A should be 5
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should be cleared
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should be cleared
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should be cleared
}

// Test for edge case: adding values that result in carry but not zero
TEST_F(CPUAddTest, ADD_A_B_CarryWithoutZero) {
    cpu.setA(0x80);  // 10000000
    cpu.setB(0x80);  // 10000000
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for carry and half-carry on small addition (nibble overflow)
TEST_F(CPUAddTest, ADD_A_B_HalfCarryOnSmallAddition) {
    cpu.setA(0x0F);  // Lower nibble full
    cpu.setB(0x01);  // Adding 1 causes half-carry
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x10);                  // A should be 0x10
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for simple addition without any flag triggered for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_NoFlags) {
    cpu.setA(5);
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 10);                    // A should be 10
    EXPECT_EQ(cpu.getFlags(), 0);                 // No flags should be set
}

// Test for addition that triggers the carry flag (overflow from 8 bits) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_CarryFlag) {
    cpu.setA(0xFF);  // Maximum 8-bit value
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0xFE);                  // Result should be 0xFE
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for addition that triggers the half-carry flag (carry from bit 3 to bit 4) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_HalfCarryFlag) {
    cpu.setA(0x0F);  // Lower nibble is full (00001111)
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0x1E);                  // A should be 0x1E
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for addition that results in zero (sets zero flag) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_ZeroFlag) {
    cpu.setA(0x00);
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for addition that sets both carry and half-carry flags for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_CarryAndHalfCarryFlags) {
    cpu.setA(0x8F);  // 10001111
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0x1E);                  // A should be 0x1E
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test for adding zero (should not affect any flags or change A) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_AddZero) {
    cpu.setA(0x00);  // Adding 0
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should remain unchanged
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for adding small values (no flags triggered) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_SmallValues) {
    cpu.setA(1);
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 2);                     // A should be 2
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test for adding maximum value without overflow (should not set carry) for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_MaxValueWithoutOverflow) {
    cpu.setA(0x7F);  // 127, the maximum value that doesn't trigger carry
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0xFE);                  // A should be 0xFE
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test for clearing flags after they are previously set for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_ClearFlagsAfterSet) {
    cpu.setA(0xFF);  // First trigger flags
    cpu.add_a_a();

    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set

    cpu.setA(2);  // Now clear flags by doing simple addition
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 4);                     // A should be 4
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should be cleared
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should be cleared
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should be cleared
}

// Test for edge case: adding values that result in carry but not zero for ADD_A_A
TEST_F(CPUAddTest, ADD_A_A_CarryWithoutZero) {
    cpu.setA(0x80);  // 10000000
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0x00);                  // A should be 0x00
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

TEST_F(CPUAddTest, ADD_A_B_MaxValuesWithCarryAndHalfCarry) {
    cpu.setA(0xFF);
    cpu.setB(0xFF);
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0xFE);                  // A should be 0xFE
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

TEST_F(CPUAddTest, ADD_A_A_MaxValuesWithCarryAndHalfCarry) {
    cpu.setA(0xFF);
    cpu.add_a_a();

    EXPECT_EQ(cpu.getA(), 0xFE);                  // A should be 0xFE
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

TEST_F(CPUAddTest, ADD_A_B_SignedOverflowBehavior) {
    cpu.setA(0x7F);  // Plus grand nombre positif signé
    cpu.setB(0x01);  // Ajout de 1 cause un overflow en signant
    cpu.execute(0x80);

    EXPECT_EQ(cpu.getA(), 0x80);                  // A should be 0x80 (overflow signed)
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}
