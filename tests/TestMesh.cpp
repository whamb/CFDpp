#include <gtest/gtest.h>

#include <Mesh.hpp>

TEST(MeshTest, MeshHasExpectedNumberOfFaces) {
    Mesh mesh(0.0, 1.0, 0.1);
    EXPECT_EQ(mesh.nFaces(), 11);
}

TEST(MeshTest, MeshHasExpectedNumberOfCells) {
    Mesh mesh(0.0, 1.0, 0.1);
    EXPECT_EQ(mesh.nCells(), 10);
}

TEST(MeshTest, MeshValidationSucceeds) {
    Mesh mesh(0.0, 1.0, 0.1);
    EXPECT_TRUE(mesh.validate());
}