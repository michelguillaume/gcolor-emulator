# ================================================================
# Project: Gameboy Color Emulator
# File: CMakeLists.txt
#
# Author: Guillaume MICHEL
# Created on: September 24, 2024
#
# License: MIT License
#
# Copyright (c) 2024 Guillaume MICHEL
# ================================================================

add_library(cpu STATIC
        cpu.cpp
)

target_include_directories(cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
