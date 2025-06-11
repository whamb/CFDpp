cmake_minimum_required(VERSION 3.15)

# Set the project name
project(CFDpp LANGUAGES C CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set_property(TARGET CFDpp PROPERTY CXX_STANDARD 17)

# Add include directories
include_directories(Geometry)
include_directories(Utilities)

# Add the executable
add_executable(CFDpp src/main.cpp Geometry/Face.cpp Geometry/Mesh.cpp)