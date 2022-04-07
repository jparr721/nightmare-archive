#include "simulation.h"
#include "fem/integrators_implicit_euler.h"
#include "fem/mass_matrix_linear_tetmesh.h"
#include "geometry.h"
#include <igl/volume.h>
#include <spdlog/spdlog.h>

namespace nm {
    void SimulationState::setSimulationConstraint(const SimulationConstraint &simulationConstraint) {
        if (!isInit()) {
            spdlog::error("System is not yet initialized!!");
            return;
        }

        constraint = simulationConstraint;

        // The positions vector contains all the fixed nodes and zeroes for everything else.
        constraint.positions = q - constraint.selectionMatrix.transpose() * constraint.selectionMatrix * q;

        // We need to resize the vectors and matrices so they are the correct size.
        q = constraint.selectionMatrix * q;
        qdot = constraint.selectionMatrix * qdot;
        massMatrix = constraint.selectionMatrix * massMatrix * constraint.selectionMatrix.transpose();
    }

    void SimulationState::initializeSimRuntimeMembers(const matXr &vertices) {
        spdlog::debug("Initializing deformed coordinates and vectors");
        q.resize(vertices.rows() * vertices.cols());
        qdot.resize(vertices.rows() * vertices.cols());

        Eigen::MatrixXd vt = vertices.transpose();
        q = matrixToVector(vt);
        qdot.setZero();
    }

    void SimulationState::initializeSimAuxiliaryMembers(const matXr &vertices, const matXi &tets) {
        spdlog::debug("Initializing tetrahedral volumes");
        igl::volume(vertices, tets, tetVolumes);

        spdlog::debug("Initializing mass matrix");
        massMatrix = fem::massMatrixLinearTetmesh(tets, qdot, tetVolumes, density);
    }

    auto SimulationState::isInit() const -> bool {
        return q.size() > 0 && qdot.size() > 0 && massMatrix.size() > 0 && tetVolumes.size() > 0;
    }

    auto SimulationState::getSelectedVertexPositions() const -> vecXr {
        return constraint.selectionMatrix.transpose() * q;
    }

    auto SimulationState::getSelectedVertexVelocities() const -> vecXr {
        return constraint.selectionMatrix.transpose() * qdot;
    }

    auto SimulationState::computeExternalForceForSelectedPositions(const vec3r &force) const -> vecXr {
        assert(isInit());
        vecXr externalForce;
        externalForce.resize(getSelectedVertexPositions().rows());

        for (int ii = 0; ii < externalForce.rows(); ii += 3) { externalForce.segment<3>(ii) = force; }

        return externalForce;
    }

    auto simulationStateFactory(const matXr &vertices, const matXi &tets, real youngsModulus, real poissonsRatio,
                                real dt, real density) -> SimulationState {
        spdlog::info("Initializing simulation runtime...");
        auto simulationState = SimulationState(youngsModulus, poissonsRatio, dt, density);
        simulationState.initializeSimRuntimeMembers(vertices);
        simulationState.initializeSimAuxiliaryMembers(vertices, tets);
        spdlog::info("Runtime has been initialized successfully.");
        return simulationState;
    }

    auto simulationConstraintFactory(const matXr &vertices, real selectionTolerance) -> SimulationConstraint {
        spdlog::debug("Initializing simulation constraint");
        // Get the rows from q to populate the selection matrix
        const auto qSize = vertices.size();
        auto simulationConstraint = SimulationConstraint{};
        simulationConstraint.indices = findMinVertices(vertices, selectionTolerance);

        // Set the fixed point constraints into the selection matrix.
        const auto selectionMatrixRows = qSize - simulationConstraint.indices.size() * kNumberOfColumns;
        simulationConstraint.selectionMatrix.resize(selectionMatrixRows, qSize);

        // Set up triplets to populate the selection matrix.
        std::vector<tripletr> triplets;
        triplets.reserve(selectionMatrixRows);

        unsigned int count = 0;
        // For each triplet pair in the indices.
        for (auto ii = 0u; ii < qSize; ii += 3) {
            // If this item is not divisible by 3, skip it
            if (std::find(simulationConstraint.indices.begin(), simulationConstraint.indices.end(), ii / 3) !=
                simulationConstraint.indices.end()) {
                continue;
            }

            // Construct a banded matrix of selections so we can select 3-vertex groups from q.
            triplets.emplace_back(count, ii, 1.0);
            triplets.emplace_back(count + 1, ii + 1, 1.0);
            triplets.emplace_back(count + 2, ii + 2, 1.0);
            count += 3;
        }

        simulationConstraint.selectionMatrix.setFromTriplets(triplets.begin(), triplets.end());

        spdlog::debug("Simulation constraint ready");
        return simulationConstraint;
    }

    void simulate(SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                  std::optional<unsigned int> selectedVertex) {
        fem::implicitEuler(simulationState, vertices, tets, selectedVertex);
    }
}// namespace nm