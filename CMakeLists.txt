cmake_minimum_required(VERSION 3.10)
project(ScientificCalculator)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically grab all .cpp files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(scientific_calc ${SOURCES})

# Include headers
target_include_directories(scientific_calc PRIVATE src)