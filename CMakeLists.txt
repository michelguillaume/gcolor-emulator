# ================================================================
# Project: Gameboy Color Emulator
# File: CMakeLists.txt
# Description: CMake configuration file for building the
#              Gameboy Color Emulator. This file defines the
#              project structure, compilation of source files,
#              and linking of necessary libraries for the emulator.
#
# Author: Guillaume MICHEL
# Created on: September 24, 2024
#
# License: MIT License
#
# Copyright (c) 2024 Guillaume MICHEL
# ================================================================

cmake_minimum_required(VERSION 3.29)
project(GColorEmulator VERSION 1.0 LANGUAGES CXX)

# Set the directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(app)

# Add Google Test
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

# Enable testing framework
enable_testing()

# Add test executable
add_executable(runTests
        tests/test_add.cpp
        tests/test_adc.cpp
        tests/test_sub.cpp
        tests/test_sbc.cpp
        tests/test_and.cpp
        tests/test_xor.cpp
        tests/test_or.cpp
        tests/test_cp.cpp
)

# Link GoogleTest and your CPU library to the test executable
target_link_libraries(runTests gtest gtest_main cpu)

# Add a test to be run with CTest
add_test(NAME runTests COMMAND runTests)
