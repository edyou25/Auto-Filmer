#!/bin/bash

echo "=== Checking CMake Build Configuration ==="

if [ -f "build/CMakeCache.txt" ]; then
    echo "Build type:"
    grep "CMAKE_BUILD_TYPE" build/CMakeCache.txt
    echo
    echo "CXX Flags:"
    grep "CMAKE_CXX_FLAGS" build/CMakeCache.txt | head -5
    echo
    echo "Compiler:"
    grep "CMAKE_CXX_COMPILER" build/CMakeCache.txt
else
    echo "CMakeCache.txt not found. Please run catkin_make first."
fi