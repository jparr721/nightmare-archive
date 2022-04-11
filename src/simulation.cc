#include "simulation.h"
#include "fem/V_linear_tetrahedron.h"
#include "fem/integrators_implicit_euler.h"
#include "fem/mass_matrix_linear_tetmesh.h"
#include "fem/spring_potential.h"
#include "geometry.h"
#include "visualization.h"
#include <igl/volume.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace nm {
    real youngsModulus = 6e6;
    real poissonsRatio = 0.4;
    real lambda = 0.5 * (youngsModulus * poissonsRatio) / ((1.0 + poissonsRatio) * (1.0 - 2.0 * poissonsRatio));
    real mu = 0.5 * youngsModulus / (2.0 * (1.0 + poissonsRatio));

    std::vector<int> fixedPointIndices;
    std::vector<std::pair<vec3r, int>> springPoints;

    vecXr fixedPointVertices;
    vecXr tetrahedronVolumes;

    spmatXr selectionMatrix;
    spmatXr massMatrix;

    constexpr real kSelectionSpringStiffness = 1e8;
    constexpr real kDensity = 0.1;
    constexpr real kFixedVertexSelectionTolerance = 3;
    constexpr real kSpringRestLength = 0.0;

    void simulate(vecXr &q, vecXr &qdot, real dt) {
        springPoints.clear();

        vec3r mouse;
        vec6r dVMouse;

        if (viz::getPickedVertex() >= 0) {
            springPoints.emplace_back(
                    (selectionMatrix.transpose() * q + fixedPointVertices).segment<3>(3 * viz::getPickedVertex()) +
                            viz::getMouseDragWorld() + vec3r::Constant(1e-6),
                    3 * viz::getPickedVertex());
        }

        const auto energy = [&](const vecXr &initialGuess) -> real {
            real energy = 0;
            vecXr newq = selectionMatrix.transpose() * (q + dt * initialGuess) * fixedPointVertices;

            for (auto ii = 0u; ii < viz::getMeshInstance().tetrahedra.rows(); ++ii) {
                energy += fem::VlinearTetrahedron(newq, viz::getMeshInstance().vertices,
                                                  viz::getMeshInstance().tetrahedra.row(ii), mu, lambda,
                                                  tetrahedronVolumes(ii));
            }

            if (!springPoints.empty()) {
                energy +=
                        fem::springPotentialEnergy(springPoints.at(0).first, newq.segment<3>(springPoints.at(0).second),
                                                   kSpringRestLength, kSelectionSpringStiffness);
            }

            energy += 0.5 * (initialGuess - qdot).transpose() * massMatrix * (initialGuess - qdot);
            return energy;
        };

        const auto force = [&](const vecXr &newq) -> vecXr {
            auto force = fem::assembleForces(selectionMatrix.transpose() * newq + fixedPointVertices,
                                             viz::getMeshInstance().vertices, viz::getMeshInstance().tetrahedra,
                                             tetrahedronVolumes, mu, lambda);

            if (!springPoints.empty()) {
                const auto u = springPoints.at(0).first;
                const auto v =
                        (selectionMatrix.transpose() * newq + fixedPointVertices).segment<3>(springPoints.at(0).second);
                const auto dV = fem::springPotentialEnergyGradient(u, v, kSpringRestLength, kSelectionSpringStiffness);
                force.segment<3>(3 * viz::getPickedVertex()) -= dV.segment<3>(3);
            }

            return selectionMatrix * force;
        };

        const auto stiffness = [&](const vecXr &newq) -> spmatXr {
            const spmatXr stiffness = fem::assembleStiffness(
                    selectionMatrix.transpose() * newq + fixedPointVertices, viz::getMeshInstance().vertices,
                    viz::getMeshInstance().tetrahedra, tetrahedronVolumes, mu, lambda);

            return selectionMatrix * stiffness * selectionMatrix.transpose();
        };

        fem::implicitEuler(energy, force, stiffness, viz::getMeshInstance().vertices, viz::getMeshInstance().tetrahedra,
                           massMatrix, dt, q, qdot);
    }

    auto setupVariables(vecXr &q, vecXr &qdot, Mesh &mesh) -> bool {
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