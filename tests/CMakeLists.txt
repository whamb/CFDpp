# tests/CMakeLists.txt

enable_testing()
include(GoogleTest)

# Add CsrSystem test
add_executable(TestCsrSystem TestCsrSystem.cpp)
target_link_libraries(TestCsrSystem PRIVATE gtest gtest_main CFDppLib)
gtest_discover_tests(TestCsrSystem)

# Add Mesh test
add_executable(TestMesh TestMesh.cpp)
target_link_libraries(TestMesh PRIVATE gtest gtest_main CFDppLib)
gtest_discover_tests(TestMesh)