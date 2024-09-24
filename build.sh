#!/bin/bash

# This script configures and builds the project using CMake

# Default build type (Debug by default)
BUILD_TYPE="Debug"

# Parse command-line arguments for build type (Release if specified)
while getopts "r" opt; do
  case ${opt} in
    r )
      BUILD_TYPE="Release"
      ;;
    \? )
      echo "Usage: $0 [-r]"
      echo "  -r    Use Release build type (default is Debug)"
      exit 1
      ;;
  esac
done

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating the build directory..."
    mkdir build
fi

# Navigate to the build directory, exit if cd fails
if ! cd build; then
    echo "Failed to change directory to 'build'. Exiting..."
    exit 1
fi

# Run CMake to generate build files
echo "Configuring the project with CMake (Build type: ${BUILD_TYPE})..."
if ! cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..; then
    echo "CMake configuration failed. Please check the errors above."
    exit 1
fi

# Run the build process (use make for Unix-like systems)
echo "Building the project..."
if ! make; then
    echo "Build failed. Please check the errors above."
    exit 1
fi

# If everything succeeded
echo "Build completed successfully!"
