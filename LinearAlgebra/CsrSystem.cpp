#include <algorithm>
#include <iterator>

#include <CsrSystem.hpp>

void CsrSystem::convertTripletToCsr(const TripletSystem& tripletSystem){
    assert(m_values.size() == m_columnIdx.size() && "CSR value/column index size mismatch.");

    // Zip into a vector of tuples
    std::vector<std::tuple<CellID, CellID, Double >> combined;
    for (size_t i = 0; i < tripletSystem.lhsSize(); ++i) {
        combined.emplace_back(tripletSystem.rows()[i], 
                              tripletSystem.columns()[i],
                              tripletSystem.values()[i]);
    }

    // Sort by the first element (rows)
    std::sort(combined.begin(), combined.end(), [](const auto& a, const auto& b) {
        if (std::get<0>(a) != std::get<0>(b))
            return std::get<0>(a) < std::get<0>(b);
        else 
            return std::get<1>(a) < std::get<1>(b);
    });

    CellID currRow = 0;
    CellID cRow = 0;
    //Initialise m_compressedRow (convention)
    m_compressedRow.push_back(0);

    for (const auto& tup : combined) {
        m_values.push_back    (std::get<2>(tup));
        m_columnIdx.push_back(std::get<1>(tup));

        while(currRow < std::get<0>(tup)){
            if(cRow != 0){
                m_compressedRow.push_back(
                                m_compressedRow[m_compressedRow.size() - 1] + cRow);
                cRow = 0;
                ++currRow;
                continue;
            }
            ++currRow;
            m_compressedRow.push_back(
                            m_compressedRow[m_compressedRow.size() - 1]);
        }
        if(currRow == std::get<0>(tup)){
            ++cRow;
        }   
    }
    if(cRow != 0){
                m_compressedRow.push_back(
                            m_compressedRow[m_compressedRow.size() - 1] + cRow);
    }

    // Build RHS of the CSR system
    std::copy(tripletSystem.rhs().begin(), 
              tripletSystem.rhs().end(),
              std::back_inserter(m_csrRhs));
}
