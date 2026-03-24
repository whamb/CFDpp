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

find_package(OpenMP REQUIRED)

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
# Windows local copy
target_include_directories(CFDppLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen
)

# macOS Homebrew (works on both Intel and Apple Silicon)
if(APPLE)
    find_path(EIGEN3_PATH Eigen/Core
              PATHS /opt/homebrew/opt/eigen/include/eigen3
                    /usr/local/opt/eigen/include/eigen3
              NO_DEFAULT_PATH)
    if(EIGEN3_PATH)
        target_include_directories(CFDppLib PUBLIC ${EIGEN3_PATH})
    endif()
endif()


# -------------------------------
# Link PETSc
# -------------------------------
if(USE_PETSC)
  target_link_libraries(CFDppLib PUBLIC PkgConfig::PETSc)
endif()

target_link_libraries(CFDppLib PRIVATE OpenMP::OpenMP_CXX)

# -------------------------------
# Main solver executable
# -------------------------------
add_executable(CFDpp src/main.cpp)
target_link_libraries(CFDpp PRIVATE CFDppLib)


# -------------------------------
# Parallel execution
# -------------------------------
target_compile_features(CFDppLib PRIVATE cxx_std_20) 
# Parallel execution — both targets need OpenMP flags
target_compile_options(CFDppLib PRIVATE 
    "$<$<CXX_COMPILER_ID:MSVC>:/openmp>"
)
target_compile_options(CFDpp PRIVATE /openmp)

# -------------------------------
# Unit tests
# -------------------------------
add_subdirectory(tests)