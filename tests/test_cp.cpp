#include <gtest/gtest.h>
#include "cpu.hpp"

class CPUCpTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

// Test for CP between two registers, A > B
TEST_F(CPUCpTest, CP_A_B_A_GreaterThan_B) {
    cpu.setA(0x10);  // A = 0x10
    cpu.setB(0x08);  // B = 0x08
    cpu.execute(0xB8);  // CP A, B (compare A with B)

    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set (A != B)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set (A > B)
    EXPECT_TRUE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

// Test for CP where A == B
TEST_F(CPUCpTest, CP_A_B_A_EqualTo_B) {
    cpu.setA(0x10);  // A = 0x10
    cpu.setB(0x10);  // B = 0x10
    cpu.execute(0xB8);  // CP A, B (compare A with B)

    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set (A == B)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

// Test for CP where A < B (borrow occurs)
TEST_F(CPUCpTest, CP_A_B_A_LessThan_B) {
    cpu.setA(0x08);  // A = 0x08
    cpu.setB(0x10);  // B = 0x10
    cpu.execute(0xB8);  // CP A, B (compare A with B)

    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set (A != B)
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set (A < B)
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

// Test for CP A with itself (A == A)
TEST_F(CPUCpTest, CP_A_A) {
    cpu.setA(0x20);  // A = 0x20
    cpu.cp_a_a();    // CP A, A (compare A with itself)

    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set (A == A)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

/*

// Test for CP with an immediate value (A == immediate)
TEST_F(CPUCpTest, CP_A_Immediate_Equal) {
    cpu.setA(0x10);  // A = 0x10
    cpu.execute(0xFE);  // CP A, 0x10 (compare A with immediate 0x10)

    EXPECT_TRUE(cpu.getZeroFlag());               // Zero flag should be set (A == immediate)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

// Test for CP with an immediate value (A > immediate)
TEST_F(CPUCpTest, CP_A_Immediate_A_GreaterThan_Immediate) {
    cpu.setA(0x20);  // A = 0x20
    cpu.execute(0xFE);  // CP A, 0x10 (compare A with immediate 0x10)

    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set (A != immediate)
    EXPECT_FALSE(cpu.getCarryFlag());             // Carry flag should NOT be set (A > immediate)
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}

// Test for CP with an immediate value (A < immediate)
TEST_F(CPUCpTest, CP_A_Immediate_A_LessThan_Immediate) {
    cpu.setA(0x08);  // A = 0x08
    cpu.execute(0xFE);  // CP A, 0x10 (compare A with immediate 0x10)

    EXPECT_FALSE(cpu.getZeroFlag());              // Zero flag should NOT be set (A != immediate)
    EXPECT_TRUE(cpu.getCarryFlag());              // Carry flag should be set (A < immediate)
    EXPECT_FALSE(cpu.getHalfCarryFlag());         // Half-carry flag should NOT be set
    EXPECT_TRUE(cpu.getSubtractFlag());           // Subtract flag should be set
}
*/
