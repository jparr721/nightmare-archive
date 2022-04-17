#include "simulation.h"
#include "fem/integrators_implicit_euler.h"
#include "fem/linear_spring_potential_energy.h"
#include "fem/linear_tetrahedron_potential_energy.h"
#include "fem/mass_matrix_linear_tetmesh.h"
#include "geometry.h"
#include "visualization.h"
#include <igl/volume.h>
#include <spdlog/fmt/ostr.h>
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

    spmatr selectionMatrix;
    spmatr massMatrix;

    constexpr real kSelectionSpringStiffness = 1e8;
    constexpr real kDensity = 0.1;
    constexpr real kFixedVertexSelectionTolerance = 3;
    constexpr real kSpringRestLength = 0.0;

    void draw(const vecXr &q) { viz::updateVertexPositions(selectionMatrix.transpose() * q + fixedPointVertices); }

    void simulate(vecXr &q, vecXr &qdot, real dt) {
        springPoints.clear();

        real currentSpringStiffness = viz::getIsMouseDragging() ? kSelectionSpringStiffness : 0.0;

        if (viz::getPickedVertex() >= 0) {
            springPoints.emplace_back(
                    (selectionMatrix.transpose() * q + fixedPointVertices).segment<3>(3 * viz::getPickedVertex()) +
                            viz::getMouseDragWorld() + vec3r::Constant(1e-6),
                    3 * viz::getPickedVertex());
        }

        const auto energy = [&](const vecXr &initialGuess) -> real {
            real energyScore = 0;
            vecXr newq = selectionMatrix.transpose() * (q + dt * initialGuess) + fixedPointVertices;

            for (auto ii = 0u; ii < viz::getMeshInstance().tetrahedra.rows(); ++ii) {
                const auto tetrahedronPotentialEnergy = fem::linearTetrahedronPotentialEnergy(
                        newq, viz::getMeshInstance().vertices, viz::getMeshInstance().tetrahedra.row(ii), mu, lambda,
                        tetrahedronVolumes(ii));
                energyScore += tetrahedronPotentialEnergy;
            }

            for (const auto &[u, v] : springPoints) {
                const auto springPotentialEnergy =
                        fem::springPotentialEnergy(u, newq.segment<3>(v), kSpringRestLength, currentSpringStiffness);
                energyScore += springPotentialEnergy;
            }

            energyScore += 0.5 * (initialGuess - qdot).transpose() * massMatrix * (initialGuess - qdot);
            return energyScore;
        };

        const auto force = [&](const vecXr &newq) -> vecXr {
            auto forces = fem::assembleForces(selectionMatrix.transpose() * newq + fixedPointVertices,
                                              viz::getMeshInstance().vertices, viz::getMeshInstance().tetrahedra,
                                              tetrahedronVolumes, mu, lambda);

            if (!springPoints.empty() && viz::getPickedVertex() >= 0) {
                const auto u = springPoints.at(0).first;
                const auto v =
                        (selectionMatrix.transpose() * newq + fixedPointVertices).segment<3>(springPoints.at(0).second);
                const auto dV = fem::springPotentialEnergyGradient(u, v, kSpringRestLength, kSelectionSpringStiffness);
                forces.segment<3>(3 * viz::getPickedVertex()) -= dV.segment<3>(3);
            }

            return selectionMatrix * forces;
        };

        const auto stiffness = [&](const vecXr &newq) -> spmatr {
            spmatr stiffnessMatrix = fem::assembleStiffness(
                    selectionMatrix.transpose() * newq + fixedPointVertices, viz::getMeshInstance().vertices,
                    viz::getMeshInstance().tetrahedra, tetrahedronVolumes, mu, lambda);
            stiffnessMatrix = selectionMatrix * stiffnessMatrix * selectionMatrix.transpose();
            return stiffnessMatrix;
        };

        fem::implicitEuler(energy, force, stiffness, viz::getMeshInstance().vertices, viz::getMeshInstance().tetrahedra,
                           massMatrix, dt, q, qdot);
    }

    auto setupVariables(vecXr &q, vecXr &qdot, Mesh &mesh) -> bool {
        spdlog::info("=======RUNTIME VARS=========");
        spdlog::info("Youngs Modulus: {}", youngsModulus);
        spdlog::info("Poissons Ratio: {}", poissonsRatio);
        spdlog::info("Lame's Lambda: {}", lambda);
        spdlog::info("Lame's Mu: {}", mu);
        spdlog::info("============================");

        if (!tetrahedralizeMesh(mesh)) {
            spdlog::error("Tetrahedralization failed.");
            return false;
        }

        spdlog::info("=======MESH STATS=========");
        spdlog::info("Vertices: {}", mesh.vertices.rows());
        spdlog::info("Tetrahedra: {}", mesh.tetrahedra.rows());
        spdlog::info("==========================");

        // Initialize position tracking variables
        setupSimulationVariables(mesh.vertices, q, qdot);

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

        // Fill the constraint matrix
        selectionMatrix = setupFixedPointConstraints(q.rows(), fixedPointIndices);

        fixedPointVertices = q - selectionMatrix.transpose() * selectionMatrix * q;

        // Resize the selection matrix and position vectors
        q = selectionMatrix * q;
        qdot = selectionMatrix * qdot;
        massMatrix = selectionMatrix * massMatrix * selectionMatrix.transpose();

        spdlog::info("Simulation variables initialized");

        return true;
    }

    auto setupFixedPointConstraints(unsigned int qSize, const std::vector<int> &indices) -> spmatr {
        spmatr fixedPointSelectionMatrix;
        fixedPointSelectionMatrix.resize(qSize - indices.size() * 3, qSize);
        fixedPointSelectionMatrix.setIdentity();

        std::vector<Eigen::Triplet<real>> triplets;
        triplets.reserve(qSize - indices.size() * 3);

        unsigned int count = 0;
        for (auto ii = 0u; ii < qSize; ii += 3) {
            if (std::find(indices.begin(), indices.end(), (unsigned int) (ii / 3)) != indices.end()) { continue; }
            triplets.emplace_back(count, ii, 1.0);
            triplets.emplace_back(count + 1, ii + 1, 1.0);
            triplets.emplace_back(count + 2, ii + 2, 1.0);
            count += 3;
        }

        fixedPointSelectionMatrix.setFromTriplets(triplets.begin(), triplets.end());
        return fixedPointSelectionMatrix;
    }

    void setupSimulationVariables(const matXr &vertices, vecXr &q, vecXr &qdot) {
        q.resize(vertices.size());
        qdot.resize(vertices.size());
        matXr vt = vertices.transpose();
        q = Eigen::Map<vecXr>(vt.data(), vt.size());
        qdot.setZero();
    }
}// namespace nm