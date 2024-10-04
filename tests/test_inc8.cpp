#include <gtest/gtest.h>
#include "cpu.hpp"  // Inclure ta classe CPU

class CPUIncATest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();  // Réinitialiser les registres et les flags avant chaque test
    }
};

// Test pour l'incrémentation qui affecte le Half Carry flag (A = 0x0F -> 0x10)
TEST_F(CPUIncATest, INC8_A_HalfCarryFlag) {
    cpu.setA(0x0F);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x10);              // A doit être 0x10
    EXPECT_TRUE(cpu.getHalfCarryFlag());      // Half Carry flag doit être activé
    EXPECT_FALSE(cpu.getZeroFlag());          // Zero flag ne doit pas être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}

// Test pour l'incrémentation qui affecte le Zero flag (A = 0xFF -> 0x00)
TEST_F(CPUIncATest, INC8_A_ZeroFlag) {
    cpu.setA(0xFF);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x00);              // A doit être 0x00
    EXPECT_TRUE(cpu.getZeroFlag());           // Zero flag doit être activé
    EXPECT_TRUE(cpu.getHalfCarryFlag());      // Half Carry flag doit être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}

// Test pour l'incrémentation qui préserve le Carry flag (Carry flag est préservé)
TEST_F(CPUIncATest, INC8_A_PreservesCarryFlag) {
    cpu.setA(0x01);
    cpu.setCarryFlag(true);                   // Activer le Carry flag
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x02);              // A doit être 0x02
    EXPECT_TRUE(cpu.getCarryFlag());          // Carry flag doit être préservé
    EXPECT_FALSE(cpu.getZeroFlag());          // Zero flag ne doit pas être activé
    EXPECT_FALSE(cpu.getHalfCarryFlag());     // Half Carry flag ne doit pas être activé
}

// Test pour l'incrémentation qui affecte seulement le Half Carry flag (A = 0x0F -> 0x10)
TEST_F(CPUIncATest, INC8_A_OnlyHalfCarry) {
    cpu.setA(0x0F);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x10);              // A doit être 0x10
    EXPECT_TRUE(cpu.getHalfCarryFlag());      // Half Carry flag doit être activé
    EXPECT_FALSE(cpu.getZeroFlag());          // Zero flag ne doit pas être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}

// Test pour l'incrémentation sans affecter les flags (A = 0x01 -> 0x02)
TEST_F(CPUIncATest, INC8_A_NoFlags) {
    cpu.setA(0x01);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x02);              // A doit être 0x02
    EXPECT_FALSE(cpu.getZeroFlag());          // Zero flag ne doit pas être activé
    EXPECT_FALSE(cpu.getHalfCarryFlag());     // Half Carry flag ne doit pas être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}

// Test pour l'incrémentation à 0xFF (A = 0xFE -> 0xFF)
TEST_F(CPUIncATest, INC8_A_ToFF) {
    cpu.setA(0xFE);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0xFF);              // A doit être 0xFF
    EXPECT_FALSE(cpu.getZeroFlag());          // Zero flag ne doit pas être activé
    EXPECT_FALSE(cpu.getHalfCarryFlag());     // Half Carry flag ne doit pas être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}

// Test pour l'incrémentation qui affecte à la fois le Half Carry et le Zero flag (A = 0xFF -> 0x00)
TEST_F(CPUIncATest, INC8_A_HalfCarryAndZeroFlags) {
    cpu.setA(0xFF);
    cpu.clearFlags();
    cpu.execute(0x3C);  // Execute l'instruction INC A

    EXPECT_EQ(cpu.getA(), 0x00);              // A doit être 0x00
    EXPECT_TRUE(cpu.getZeroFlag());           // Zero flag doit être activé
    EXPECT_TRUE(cpu.getHalfCarryFlag());      // Half Carry flag doit être activé
    EXPECT_FALSE(cpu.getSubtractFlag());      // Subtract flag ne doit pas être activé
}
