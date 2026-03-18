#include <gtest/gtest.h>
#include <TripletSystem.hpp>
#include <CsrSystem.hpp>

// A mock class to expose protected members for test validation (optional)
class CsrSystemTestable : public CsrSystem {
public:
    using CsrSystem::CsrSystem;
    const std::vector<Double>& values()        const { return CsrSystem::values(); }
    const std::vector<CellID>& columnIdx()     const { return CsrSystem::columnIdx(); }
    const std::vector<CellID>& compressedRow() const { return CsrSystem::compressedRow(); }
    const std::vector<Double>& csrRhs()        const { return CsrSystem::csrRhs(); }
};

TEST(CsrSystemTest, ConvertSimpleTriplet) {
    // Build a dummy mesh with 3 cells
    Mesh mesh(0.0, 3.0, 1.0); // Will create 3 cells: [0,1], [1,2], [2,3]

    TripletSystem triplet(mesh);

    // Matrix:
    // [ 2 3 0 ]
    // [ 3 0 4 ]
    // [ 0 4 5 ]
    triplet.addToLHS(0, 0, 2.0);
    triplet.addToLHS(0 ,1 ,3.0);
    triplet.addToLHS(1, 0, 3.0);
    triplet.addToLHS(1, 2, 4.0);
    triplet.addToLHS(2, 1, 4.0);
    triplet.addToLHS(2, 2, 5.0);

    // RHS = [1, 2, 3]
    triplet.addToRHS(0, 1.0);
    triplet.addToRHS(1, 2.0);
    triplet.addToRHS(2, 3.0);

    // Convert to CSR
    CsrSystemTestable csr(triplet);

    // Expected results
    std::vector<Double> expectedValues   {2.0, 3.0, 3.0, 4.0, 4.0, 5.0};
    std::vector<CellID> expectedColumns  {0,   1,   0,   2,   1,   2};
    std::vector<CellID> expectedRowStart {0,   2,   4,   6};
    std::vector<Double> expectedRhs      {1.0, 2.0, 3.0};

    EXPECT_EQ(csr.values(),   expectedValues);
    EXPECT_EQ(csr.columnIdx(),  expectedColumns);
    EXPECT_EQ(csr.compressedRow(), expectedRowStart);
    EXPECT_EQ(csr.csrRhs(),      expectedRhs);
}

TEST(CsrSystemTest, HandlesZeroRow) {
    Mesh mesh(0.0, 3.0, 1.0); // 3 cells → size 3
    TripletSystem triplet(mesh);

    // Add non-zero values to row 0 and row 2
    triplet.addToLHS(0, 0, 1.0);
    triplet.addToLHS(0, 2, 2.0);
    // row 1 is left entirely zero
    triplet.addToLHS(2, 0, 4.0);
    triplet.addToLHS(2, 1, 5.0);
    triplet.addToLHS(2, 2, 6.0);

    // RHS: just to match system size
    triplet.addToRHS(0, 7.0);
    triplet.addToRHS(1, 0.0); // could be anything, but often 0 for empty rows
    triplet.addToRHS(2, 8.0);

    CsrSystemTestable csr(triplet);

    std::vector<Double> expectedValues    = {1.0, 2.0, 4.0, 5.0, 6.0};
    std::vector<CellID> expectedColumns   = {0,   2,   0,   1,   2};
    std::vector<CellID> expectedRowStart  = {0,   2,   2,   5};
    std::vector<Double> expectedRhs       = {7.0, 0.0, 8.0};

    EXPECT_EQ(csr.values(), expectedValues);
    EXPECT_EQ(csr.columnIdx(), expectedColumns);
    EXPECT_EQ(csr.compressedRow(), expectedRowStart);
    EXPECT_EQ(csr.csrRhs(), expectedRhs);
}