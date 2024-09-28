#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUAdcTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for simple ADC with no flags triggered
TEST_F(CPUAdcTest, ADC_A_B_NoFlags) {
    cpu.setA(5);
    cpu.setB(3);
    cpu.setCarryFlag(false);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 8);                     // A should be 8
    EXPECT_EQ(cpu.getFlags(), 0);                 // No flags should be set
}

// Test ADC that triggers carry flag
TEST_F(CPUAdcTest, ADC_A_B_CarryFlag) {
    cpu.setA(0xFF);  // Maximum 8-bit value
    cpu.setB(0x01);
    cpu.setCarryFlag(false);  // No carry initially
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x00);                  // Wrap around to 0
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set
}

// Test ADC with carry from previous operation
TEST_F(CPUAdcTest, ADC_A_B_WithCarry) {
    cpu.setA(0x7F);  // 127 in decimal
    cpu.setB(0x01);  // Add 1
    cpu.setCarryFlag(true);  // Carry from previous operation
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x81);                  // A should be 0x81
    EXPECT_FALSE(cpu.getCarryFlag());             // No carry
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test ADC triggering half-carry flag
TEST_F(CPUAdcTest, ADC_A_B_HalfCarryFlag) {
    cpu.setA(0x0F);  // Lower nibble full
    cpu.setB(0x01);  // Adding 1 causes half-carry
    cpu.setCarryFlag(true);  // Carry already set
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x11);                  // A should be 0x11
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getCarryFlag());             // No carry
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test ADC resulting in zero (sets zero flag)
TEST_F(CPUAdcTest, ADC_A_B_ZeroFlag) {
    cpu.setA(0x00);
    cpu.setB(0x00);
    cpu.setCarryFlag(true);  // Carry from previous operation
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x01);                  // A should be 0x01
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test ADC that sets both carry and half-carry flags
TEST_F(CPUAdcTest, ADC_A_B_CarryAndHalfCarryFlags) {
    cpu.setA(0x8F);  // 10001111
    cpu.setB(0x91);  // 10010001
    cpu.setCarryFlag(true);  // Carry already set
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x21);                  // A should be 0x21
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
}

// Test ADC adding zero (should not affect flags)
TEST_F(CPUAdcTest, ADC_A_B_AddZero) {
    cpu.setA(0x45);  // Some value
    cpu.setB(0x00);  // Adding zero
    cpu.setCarryFlag(false);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x45);                  // A should remain the same
    EXPECT_FALSE(cpu.getCarryFlag());             // No carry
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test ADC with small values (no flags triggered)
TEST_F(CPUAdcTest, ADC_A_B_SmallValues) {
    cpu.setA(0x02);
    cpu.setB(0x02);
    cpu.setCarryFlag(false);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x04);                  // A should be 0x04
    EXPECT_FALSE(cpu.getCarryFlag());             // No carry
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test ADC triggering carry without zero
TEST_F(CPUAdcTest, ADC_A_B_CarryWithoutZero) {
    cpu.setA(0x80);  // 10000000
    cpu.setB(0x80);  // 10000000
    cpu.setCarryFlag(true);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x01);                  // A should be 0x01
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
}

// Test ADC on A with no flags
TEST_F(CPUAdcTest, ADC_A_A_NoFlags) {
    cpu.setA(5);
    cpu.setCarryFlag(false);
    cpu.ADC_A_A();

    EXPECT_EQ(cpu.getA(), 10);                    // A should be 10
    EXPECT_EQ(cpu.getFlags(), 0);                 // No flags should be set
}

// Test ADC on A that triggers carry flag
TEST_F(CPUAdcTest, ADC_A_A_CarryFlag) {
    cpu.setA(0xFF);  // Maximum 8-bit value
    cpu.setCarryFlag(true);
    cpu.ADC_A_A();

    EXPECT_EQ(cpu.getA(), 0xFF);                  // Result should be 0xFF
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
}

// Test ADC on A that triggers half-carry flag
TEST_F(CPUAdcTest, ADC_A_A_HalfCarryFlag) {
    cpu.setA(0x0F);
    cpu.setCarryFlag(true);
    cpu.ADC_A_A();

    EXPECT_EQ(cpu.getA(), 0x1F);                  // A should be 0x1F
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag should be set
}

// Test ADC on A with no carry or half-carry
TEST_F(CPUAdcTest, ADC_A_A_NoCarryNoHalfCarry) {
    cpu.setA(0x01);
    cpu.setCarryFlag(true);
    cpu.ADC_A_A();

    EXPECT_EQ(cpu.getA(), 0x03);                  // A should be 0x03
    EXPECT_FALSE(cpu.getCarryFlag());             // No carry
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // No half-carry
}

// Test ADC on A with carry but no half-carry
TEST_F(CPUAdcTest, ADC_A_A_CarryNoHalfCarry) {
    cpu.setA(0x80);
    cpu.setCarryFlag(true);
    cpu.ADC_A_A();

    EXPECT_EQ(cpu.getA(), 0x01);                  // A should be 0x01
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // No half-carry
}

TEST_F(CPUAdcTest, ADC_A_B_MaxValuesWithCarryAndHalfCarry) {
    cpu.setA(0xFF);
    cpu.setB(0xFF);
    cpu.setCarryFlag(true);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0xFF);                  // Résultat devrait être 0xFF
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag devrait être activé
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag devrait être activé
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag ne devrait pas être activé
}

TEST_F(CPUAdcTest, ADC_A_B_OnlyCarryFlag) {
    cpu.setA(0x00);
    cpu.setB(0x00);
    cpu.setCarryFlag(true);  // Carry flag déjà activé
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x01);                  // A devrait être 1 (car il y a juste le carry)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag ne devrait pas être réactivé
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag ne devrait pas être activé
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag ne devrait pas être activé
}

TEST_F(CPUAdcTest, ADC_A_B_OverflowBehavior) {
    cpu.setA(0x7F);  // Plus grand nombre signé positif
    cpu.setB(0x01);  // Additionne 1, ce qui causera un overflow en signant
    cpu.setCarryFlag(false);
    cpu.ADC_A_B();

    EXPECT_EQ(cpu.getA(), 0x80);                  // A devrait être 0x80 (overflow de signe)
    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag ne devrait pas être activé
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag ne devrait pas être activé
    EXPECT_TRUE(cpu.getHalfCarryFlag());          // Half-carry flag devrait être activé
}
