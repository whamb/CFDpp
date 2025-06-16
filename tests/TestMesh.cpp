#include <gtest/gtest.h>
#include "Mesh.hpp"

TEST(MeshTest, MeshHasExpectedNumberOfCells) {
    Mesh mesh(0.0, 1.0, 0.1);
    EXPECT_EQ(mesh.getCells().size(), 10);
}

TEST(MeshTest, MeshValidationSucceeds) {
    Mesh mesh(0.0, 1.0, 0.1);
    EXPECT_TRUE(mesh.validate());
}