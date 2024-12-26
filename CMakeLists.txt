cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CoolingLoopControl VERSION 1.1)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ensure consistent runtime library
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

# Google Test
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)
enable_testing()

# Main application
add_executable(CoolingLoopControl src/CoolingLoopControl_V1.1.cpp)

# Unit tests
add_executable(CoolingLoopControlTest tests/CoolingLoopControlTest.cpp)

# Link Google Test libraries to the test executable
target_link_libraries(CoolingLoopControlTest gtest gtest_main)

# Define UNIT_TEST macro for the test target
target_compile_definitions(CoolingLoopControlTest PRIVATE UNIT_TEST)
