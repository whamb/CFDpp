#include <cmath>
#include <numbers>

#include <BurgersEqn.hpp>

// Initialise solution field and face fluxes
void BurgersEqn::initialiseSolution(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells         = mesh.cells();
    const auto& faces         = mesh.faces();
    const auto& cellCenter    = mesh.cellCenter();
    const auto& interiorFaces = mesh.interiorFaces();
    const auto& boundaryFaces = mesh.boundaryFaces();
    const auto& faceArea      = mesh.faceArea();

    for(const auto& cell : cells){
        const auto& cellId = cell->id();
        const auto& faceIds = cell->faceIds();
        const Double x = cellCenter[cellId];

        // Build indexMap
        for(const auto& fId : faceIds){
            const auto& face = *(faces[fId]);
            const auto& neighbourId = cell->nghbrCell(face);
            
            if(neighbourId == -1){
                continue; // skip boundary faces
            }
            
        // Add contributions to sparse matrix
        tripletSystem.buildLHSMap(cellId, cellId, 1.0);
        tripletSystem.buildLHSMap(cellId, neighbourId, 1.0);

        }

        // Initialise solution in each cell
        // Example initial condition (currently constant since sin(pi)=0)
        m_u[cellId] = exp(-x * x); 
        //m_u[cellId] = std::sin(std::numbers::pi * x); 
    }
    
    // Compute flux on interior faces (average of neighbouring cells)
    for(const auto& f : interiorFaces){
        const FaceID fId = f->id();
        const Double area = faceArea[fId];

        auto [c0, c1] = f->cellId(); // adjacent cells

        Double u0 = m_u[c0];
        Double u1 = m_u[c1];

        // Central interpolation of flux
        m_uf[fId] = 0.5 * (u0 + u1) * area;
    }

    // Build indexMap for boundaries
    const auto& bndCells     = mesh.boundaryCells();
    const auto& cell0        = bndCells[0];
    const auto& cell1        = bndCells[1];
    const auto& cell0Id      = cell0->id();
    const auto& cell1Id      = cell1->id();

    //Cell 0
    tripletSystem.buildLHSMap(cell0Id, cell0Id, 1.0);
    tripletSystem.buildLHSMap(cell0Id, cell1Id, 1.0);
    //Cell 1
    tripletSystem.buildLHSMap(cell1Id, cell1Id, 1.0);
    tripletSystem.buildLHSMap(cell1Id, cell0Id, 1.0);

    // Compute flux on boundary faces (single adjacent cell)
    for(const auto& bF : boundaryFaces){
        const FaceID bFId = bF->id();
        const Double area = faceArea[bFId];

        auto c = bF->cellId()[0]; // boundary has one valid cell
       
        Double u = m_u[c];

        // Boundary flux (Dirichlet-like)
        m_uf[bFId] = u * area;
    }


}

// Build full Burgers system (LHS + RHS)
void BurgersEqn::buildBurgers(const Mesh& mesh, TripletSystem& tripletSystem){
    updateFaceFlux    (mesh);                 // update nonlinear flux u*u
    buildAdvectionTerm(mesh, tripletSystem);  // convective term
    buildViscousTerm  (mesh, tripletSystem);  // diffusion term
    buildTransientTerm(mesh, tripletSystem);  // time term
    updateBc          (mesh, tripletSystem);  // boundary conditions
}

// Build advection term using upwind scheme
void BurgersEqn::buildAdvectionTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells   = mesh.cells();
    const auto& faces   = mesh.faces();
    const auto& normals = mesh.faceNormal();
    
    for(const auto& cell : cells){
        const auto& cellId = cell->id();
        const auto& faceIds = cell->faceIds();

        // Loop over faces of current cell
        for(const auto& fId : faceIds){
            const auto& face = *(faces[fId]);
            const auto& neighbourId = cell->nghbrCell(face);

            // Compute outward normal direction
            const auto& n = (cellId > neighbourId) ? normals[fId] 
                                                   : -normals[fId];

            if(neighbourId == -1){
                continue; // skip boundary faces
            }
            
            // Convective flux through face
            const Double flux = m_uf[fId] * n;

            // Upwind discretisation
            Double upwindCoeff   = 0.5 * posMax(flux);
            Double downwindCoeff = -0.5 * posMax(flux);
            
            // Add contributions to sparse matrix
            tripletSystem.addToLHS(cellId, cellId, upwindCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, downwindCoeff);
        }
    }
}

// Build viscous (diffusion) term
void BurgersEqn::buildViscousTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells  = mesh.cells();
    const auto& faces  = mesh.faces();
    const auto& center = mesh.cellCenter();
    const auto& area   = mesh.faceArea();

    for(const auto& cell : cells){
        const auto& cellId  = cell->id();
        const auto& faceIds = cell->faceIds();

        for(const auto& fId : faceIds){
            const auto& face = *(faces[fId]);
            const auto& neighbourId = cell->nghbrCell(face);

            if(neighbourId == -1){
                continue; // skip boundary faces
            }
            
            // Distance between cell centers
            Double dx = std::abs(center[cellId] - center[neighbourId]);

            // Diffusion coefficient (nu * area / dx)
            Double diffCoeff = m_nu * area[fId] / dx;
            
            // Standard finite volume Laplacian stencil
            tripletSystem.addToLHS(cellId, cellId, diffCoeff);
            tripletSystem.addToLHS(cellId, neighbourId, -diffCoeff);
        }
    }
}

// Build transient (time) term
void BurgersEqn::buildTransientTerm(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& cells = mesh.cells();
    const auto& vol   = mesh.cellVolume();

    for(const auto& cell : cells){
        const CellID cellId = cell->id();

        // Time discretisation coefficient (implicit Euler)
        const Double transientCoeff = vol[cellId] / m_dt; 

        // Add diagonal contribution
        tripletSystem.addToLHS(cellId, cellId, transientCoeff);

        // Add RHS contribution (previous timestep solution)
        tripletSystem.addToRHS(cellId, transientCoeff * m_u[cellId]);  
    }
}

// Apply boundary conditions (for the moment only periodic)
void BurgersEqn::updateBc(const Mesh& mesh, TripletSystem& tripletSystem){
    const auto& bndCells     = mesh.boundaryCells();
    const auto& cell0        = bndCells[0];
    const auto& cell1        = bndCells[1];
    const auto& cell0Id      = cell0->id();
    const auto& cell1Id      = cell1->id();
    const auto& cellCenter   = mesh.cellCenter();
    const auto& bndFaces     = mesh.boundaryFaces();
    const auto& face0        = bndFaces[0];
    const auto& face1        = bndFaces[1];
    const auto& face0Id      = face0->id();
    const auto& face1Id      = face1->id();
    const auto& faceCenter   = mesh.faceCenter();
    const auto& area         = mesh.faceArea();
    const auto& normals      = mesh.faceNormal();

    // Advection part
    const auto& n0 = -normals[face0Id];
    const auto& n1 = -normals[face1Id];
    const Double advectionFlux0 = m_uf[face0Id] * n0;
    Double upwindCoeff0   = 0.5 * posMax(advectionFlux0);
    Double downwindCoeff0 = -0.5 * posMax(advectionFlux0);
    const Double advectionFlux1 = m_uf[face1Id] * n1;
    Double upwindCoeff1   = 0.5 * posMax(advectionFlux1);
    Double downwindCoeff1 = -0.5 * posMax(advectionFlux1);
    
    //Cell 0
    tripletSystem.addToLHS(cell0Id, cell0Id, upwindCoeff0);
    tripletSystem.addToLHS(cell0Id, cell1Id, downwindCoeff0);
    //Cell 1
    tripletSystem.addToLHS(cell1Id, cell1Id, upwindCoeff1);
    tripletSystem.addToLHS(cell1Id, cell0Id, downwindCoeff1);

    // Diffusion part
    const Double dx = std::abs(cellCenter[0] - faceCenter[0])
                    + std::abs(cellCenter[1] - faceCenter[1]);
    //TODO: check if area[face0] == area[face1]
    Double diffCoeff = m_nu * area[face0->id()] / dx;

    // Cell0
    tripletSystem.addToLHS(cell0Id, cell0Id, diffCoeff);
    tripletSystem.addToLHS(cell0Id, cell1Id, -diffCoeff);
    // Cell1
    tripletSystem.addToLHS(cell1Id, cell1Id, diffCoeff);
    tripletSystem.addToLHS(cell1Id, cell0Id, -diffCoeff);
    
}

void BurgersEqn::updateFaceFlux(const Mesh& mesh){
    const auto& interiorFace       = mesh.interiorFaces();
    const auto& boundaryFace       = mesh.boundaryFaces();
    const auto& area               = mesh.faceArea();

    for(const auto& face : interiorFace){
        const FaceID faceId = face->id();
        const CellID c0 = face->cellId()[0];
        const CellID c1 = face->cellId()[1];
        const Double fArea = area[faceId];
        const Double uf0 = m_u[c0] * fArea;
        const Double uf1 = m_u[c1] * fArea;
        m_uf[faceId] = 0.5 * (uf0 + uf1);
    }

    //Boundary faces 
    // TODO: duplicated code
    for(const auto& face : boundaryFace){
        const FaceID faceId = face->id();
        const CellID c0 = face->cellId()[0];
        const CellID c1 = face->cellId()[1];
        const Double fArea = area[faceId];
        if(c0 >= 0)
            m_uf[faceId] = m_u[c0] * fArea;
        if(c1 >= 0)
            m_uf[faceId] = m_u[c1] * fArea;
        ;
    }
}
