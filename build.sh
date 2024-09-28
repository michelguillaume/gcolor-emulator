#!/bin/bash

# This script configures, builds, runs tests for the project using CMake

# Default build type (Debug by default)
BUILD_TYPE="Debug"
NUM_THREADS=""  # Default to empty, uses all available cores when running `make -j`

# Parse command-line arguments for build type, threads, and coverage (-r for Release, -j for specific thread count)
while getopts "rj:" opt; do
  case ${opt} in
    r )
      BUILD_TYPE="Release"
      ;;
    j )
      NUM_THREADS=$OPTARG
      ;;
    \? )
      echo "Usage: $0 [-r] [-j <num_threads>]"
      exit 1
      ;;
  esac
done

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating the build directory..."
    mkdir build
fi

# Navigate to the build directory
cd build || exit

# Run CMake to generate build files
echo "Configuring the project with CMake (Build type: ${BUILD_TYPE})..."
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} .. || exit 1

# Build the project with make (using all cores by default, or a specified number of threads)
if [ -z "$NUM_THREADS" ]; then
    echo "Building the project with all available cores..."
    make -j || exit 1
else
    echo "Building the project with ${NUM_THREADS} threads..."
    make -j"${NUM_THREADS}" || exit 1
fi

# Run tests using ctest
echo "Running tests..."
ctest -V --output-on-failure || exit 1

# If everything succeeded
echo "Build, tests, completed successfully!"
