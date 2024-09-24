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

# Add subdirectories
add_subdirectory(src/cpu)

# Create the executable for the application
add_executable(GColorEmulator src/main.cpp)

# Link internal libraries (like CPU)
target_link_libraries(GColorEmulator PRIVATE cpu)

# Set compile options for different configurations (Debug and Release)
target_compile_options(GColorEmulator PRIVATE
        $<$<CONFIG:Debug>:-Wall -Wextra -Wpedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types>
        $<$<CONFIG:Release>:-O3>
)
