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
# add_subdirectory(tests)

# Set compilation options

# Find SDL2 (if needed)
# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 to the target (if needed)
# target_link_libraries(GColorEmulator ${SDL2_LIBRARIES})
