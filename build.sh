#!/bin/bash

# This script configures and builds the project using CMake

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating the build directory..."
    mkdir build
fi

# Navigate to the build directory, exit if cd fails
cd build || { echo "Failed to change directory to 'build'. Exiting..."; exit 1; }

# Run CMake to generate build files
echo "Configuring the project with CMake..."
cmake ..

# Check if CMake succeeded
if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Please check the errors above."
    exit 1
fi

# Run the build process (use make for Unix-like systems)
echo "Building the project..."
make

# Check if make succeeded
if [ $? -ne 0 ]; then
    echo "Build failed. Please check the errors above."
    exit 1
fi

# If everything succeeded
echo "Build completed successfully!"
