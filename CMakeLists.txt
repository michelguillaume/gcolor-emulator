cmake_minimum_required(VERSION 3.29)
project(GColorEmulator VERSION 1.0 LANGUAGES CXX)

# Set the directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add subdirectories
add_subdirectory(app)
# add_subdirectory(tests)

# Set compilation options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 (if needed)
# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2 to the target (if needed)
# target_link_libraries(GColorEmulator ${SDL2_LIBRARIES})
