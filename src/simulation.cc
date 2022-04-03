#include "simulation.h"
#include "fem/integrators_implicit_euler.h"
#include "fem/mass_matrix_linear_tetmesh.h"
#include <igl/volume.h>
#include <spdlog/spdlog.h>

namespace nm {
    SimulationState::SimulationState(real youngsModulus, real poissonsRatio, real dt, real density)
        : youngsModulus(youngsModulus), poissonsRatio(poissonsRatio), dt(dt), density(density) {
        lambda = 0.5 * (youngsModulus * poissonsRatio) / ((1.0 + poissonsRatio) * (1.0 - 2.0 * poissonsRatio));
        mu = 0.5 * youngsModulus / (2.0 * (1.0 + poissonsRatio));
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

    auto simulationStateFactory(const matXr &vertices, const matXi &tets, real youngsModulus, real poissonsRatio,
                                real dt, real density) -> SimulationState {
        spdlog::info("Initializing simulation runtime...");
        auto simulationState = SimulationState(youngsModulus, poissonsRatio, dt, density);
        simulationState.initializeSimRuntimeMembers(vertices);
        simulationState.initializeSimAuxiliaryMembers(vertices, tets);
        spdlog::info("Runtime has been initialized successfully.");
        return simulationState;
    }

    void simulate(SimulationState &simulationState, const matXr &vertices, const matXi &tets) {
        fem::implicitEuler(simulationState.q, simulationState.qdot, vertices, simulationState.massMatrix, tets,
                           simulationState.tetVolumes, simulationState.mu, simulationState.lambda, simulationState.dt);
    }
}// namespace nm