#include "simulation.h"
#include "fem/mass_matrix_linear_tetmesh.h"
#include "geometry.h"
#include <igl/volume.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace nm {
    real youngsModulus = 6e6;
    real poissonsRatio = 0.4;
    real lambda = 0.5 * (youngsModulus * poissonsRatio) / ((1.0 + poissonsRatio) * (1.0 - 2.0 * poissonsRatio));
    real mu = 0.5 * youngsModulus / (2.0 * (1.0 + poissonsRatio));

    std::vector<int> fixedPointIndices;

    vecXr fixedPointVertices;
    vecXr tetrahedronVolumes;
    vecXr q;
    vecXr qdot;

    spmatXr selectionMatrix;
    spmatXr massMatrix;

    constexpr real kSelectionSpringStiffness = 1e8;
    constexpr real kDensity = 0.1;
    constexpr real kFixedVertexSelectionTolerance = 3;

    auto refreshViewerData() -> vecXr { return selectionMatrix.transpose() * q + fixedPointVertices; }

    auto setupVariables(Mesh &mesh) -> bool {
        if (!tetrahedralizeMesh(mesh)) {
            spdlog::error("Tetrahedralization failed.");
            return false;
        }

        // Initialize position tracking variables
        q.resize(mesh.vertices.size());
        qdot.resize(mesh.vertices.size());
        q = matrixToVector(mesh.vertices.transpose().eval());
        qdot.setZero();

        // Get volumes of all elements
        igl::volume(mesh.vertices, mesh.tetrahedra, tetrahedronVolumes);

        // Compute the mass matrix
        massMatrix = fem::massMatrixLinearTetmesh(mesh.tetrahedra, qdot, tetrahedronVolumes, kDensity);

        if (massMatrix.rows() == 0) {
            spdlog::error("Mass matrix did not initialize correctly!");
            return false;
        }

        // Set up the constraint matrix
        fixedPointIndices = findMinVertices(mesh.vertices, kFixedVertexSelectionTolerance);
        selectionMatrix.resize(q.rows(), q.rows());
        selectionMatrix.setIdentity();

        // Fill the constraint matrix
        selectionMatrix.resize(q.rows() - fixedPointIndices.size() * 3, q.rows());
        std::vector<Eigen::Triplet<real>> triplets;
        triplets.reserve(selectionMatrix.rows());

        unsigned int count = 0;
        for (auto ii = 0u; ii < q.rows(); ++ii) {
            // If this item is not divisible by 3, skip it
            if (std::find(fixedPointIndices.begin(), fixedPointIndices.end(), ii / 3) != fixedPointIndices.end()) {
                continue;
            }

            // Construct a banded matrix of selections so we can select 3-vertex groups from q.
            triplets.emplace_back(count, ii, 1.0);
            triplets.emplace_back(count + 1, ii + 1, 1.0);
            triplets.emplace_back(count + 2, ii + 2, 1.0);
            count += 3;
        }
        selectionMatrix.setFromTriplets(triplets.begin(), triplets.end());

        fixedPointVertices = q - selectionMatrix.transpose() * selectionMatrix * q;

        // Resize the selection matrix and position vectors
        q = selectionMatrix * q;
        qdot = selectionMatrix * qdot;
        massMatrix = selectionMatrix * massMatrix * selectionMatrix.transpose();

        spdlog::info("Simulation variables initialized");

        return true;
    }
}// namespace nm