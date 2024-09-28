#include <gtest/gtest.h>
#include "cpu.hpp"


class CPUTest : public ::testing::Test {
protected:
    emulator::CPU cpu;

    void SetUp() override {
        cpu.reset();
    }
};

TEST_F(CPUTest, RegisterA_GetterSetter) {
    cpu.setA(0x55);
    EXPECT_EQ(cpu.getA(), 0x55);

    cpu.setA(0xAB);
    EXPECT_EQ(cpu.getA(), 0xAB);
}

TEST_F(CPUTest, RegisterB_GetterSetter) {
    cpu.setB(0x99);
    EXPECT_EQ(cpu.getB(), 0x99);

    cpu.setB(0x33);
    EXPECT_EQ(cpu.getB(), 0x33);
}

TEST_F(CPUTest, ADD_A_B_NoCarry) {
    cpu.setA(5);
    cpu.setB(3);
    cpu.ADD_A_B();

    EXPECT_EQ(cpu.getA(), 8);
    EXPECT_EQ(cpu.getFlags(), 0);
}

TEST_F(CPUTest, ADD_A_B_ZeroFlag) {
    cpu.setA(0);
    cpu.setB(0);
    cpu.ADD_A_B();

    EXPECT_EQ(cpu.getA(), 0);
    EXPECT_TRUE(cpu.getZeroFlag());
}

TEST_F(CPUTest, ADD_A_B_HalfCarryFlag) {
    cpu.setA(0x0F);
    cpu.setB(0x01);
    cpu.ADD_A_B();

    EXPECT_EQ(cpu.getA(), 0x10);
    EXPECT_TRUE(cpu.getHalfCarryFlag());
    EXPECT_FALSE(cpu.getCarryFlag());
}

TEST_F(CPUTest, ADD_A_B_CarryFlag) {
    cpu.setA(0xFF);
    cpu.setB(0x01);
    cpu.ADD_A_B();

    EXPECT_EQ(cpu.getA(), 0x00);
    EXPECT_TRUE(cpu.getCarryFlag());
    EXPECT_TRUE(cpu.getZeroFlag());
}

TEST_F(CPUTest, ADD_A_B_CarryAndHalfCarryFlags) {
    cpu.setA(0x8F);
    cpu.setB(0x91);
    cpu.ADD_A_B();

    EXPECT_EQ(cpu.getA(), 0x20);
    EXPECT_TRUE(cpu.getCarryFlag());
    EXPECT_TRUE(cpu.getHalfCarryFlag());
    EXPECT_FALSE(cpu.getZeroFlag());
}
