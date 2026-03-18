cmake_minimum_required(VERSION 3.15)
project(CFDpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# Backend selection
# -------------------------------
option(USE_PETSC "Use PETSc linear algebra backend" ON)

if (WIN32)
    set(USE_PETSC OFF)
endif()

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
# Include PETSc
# -------------------------------
# Must be before `add_executable(...)` or any targets use PETSc
if(USE_PETSC)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(PETSc REQUIRED IMPORTED_TARGET petsc)
  if (NOT PETSc_FOUND)
    message(FATAL_ERROR "PETSc not found. Make sure PKG_CONFIG_PATH is set correctly.")
  endif()
endif()

# -------------------------------
# CFDppLib: Core CFD library
# -------------------------------
add_library(CFDppLib
    Equation/BurgersEqn.cpp
    Equation/SolveBurgers.cpp
    Geometry/Cell.cpp
    Geometry/Face.cpp
    Geometry/Mesh.cpp
    LinearAlgebra/CsrSystem.cpp
    LinearAlgebra/EigenSolve.cpp
    LinearAlgebra/TridiagonalSystem.cpp
    LinearAlgebra/TripletSystem.cpp
)

if(USE_PETSC)
  target_sources(CFDppLib PRIVATE 
                 LinearAlgebra/PetscCsrSolve.cpp
                )
endif()

if(USE_PETSC)
    target_compile_definitions(CFDppLib PUBLIC USE_PETSC)
endif()

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
# Link Eigen (header-only)
# -------------------------------
target_include_directories(CFDppLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen
)


# -------------------------------
# Link PETSc
# -------------------------------
if(USE_PETSC)
  target_link_libraries(CFDppLib PUBLIC PkgConfig::PETSc)
endif()

# -------------------------------
# Main solver executable
# -------------------------------
add_executable(CFDpp src/main.cpp)
target_link_libraries(CFDpp PRIVATE CFDppLib)

# -------------------------------
# Unit tests
# -------------------------------
add_subdirectory(tests)