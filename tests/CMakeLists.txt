include(GoogleTest)

# Example test for Mesh
add_executable(TestMesh TestMesh.cpp)
target_link_libraries(TestMesh gtest_main CFDppLib)

# Automatically discover and register all TEST()s
gtest_discover_tests(TestMesh)