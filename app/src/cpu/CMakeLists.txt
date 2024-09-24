# ================================================================
# Project: Gameboy Color Emulator
# File: CMakeLists.txt
# Description: CMake configuration file for building the
#              Gameboy Color Emulator. This file defines the
#              compilation of source files and linking of libraries.
#
# Author: Guillaume MICHEL
# Created on: [Date]
#
# License: [Specify License - e.g., MIT License]
#
# Copyright (c) [Year] Guillaume MICHEL
# ================================================================

add_library(cpu STATIC
        cpu.cpp
)

target_include_directories(cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
