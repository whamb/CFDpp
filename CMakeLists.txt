cmake_minimum_required(VERSION 3.15)
project(CFDpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing and GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Silence gtest warnings on older compilers
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# -------------------------------
# CFDppLib: Core CFD library
# -------------------------------
add_library(CFDppLib
    Equation/Burgers.cpp
    Geometry/Face.cpp
    Geometry/Mesh.cpp
    LinearAlgebra/CsrSystem.cpp
    LinearAlgebra/TridiagonalSystem.cpp
)

# Public headers for consumers
target_include_directories(CFDppLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Equation
    ${CMAKE_CURRENT_SOURCE_DIR}/Fields
    ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
    ${CMAKE_CURRENT_SOURCE_DIR}/LinearAlgebra
    ${CMAKE_CURRENT_SOURCE_DIR}/Solver
    ${CMAKE_CURRENT_SOURCE_DIR}/Utilities
)

# -------------------------------
# Main solver executable
# -------------------------------
add_executable(CFDpp src/main.cpp)
target_link_libraries(CFDpp PRIVATE CFDppLib)

# -------------------------------
# Unit tests
# -------------------------------
add_subdirectory(tests)