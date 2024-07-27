cmake_minimum_required(VERSION 3.15)

# Set the project name
project(SPECTRALMETHOD)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(Geometry)

# Add the executable
add_executable(SPECTRALMETHOD src/main.cpp)