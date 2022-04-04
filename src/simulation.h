#pragma once

#include "nm_math.h"
#include <vector>

namespace nm {
    struct SimulationConstraint {
        std::vector<int> indices;
        spmatXr selectionMatrix;
        vecXr positions;
    };

    struct SimulationState {
        // Neohookean material coefficients.
        real mu;
        real lambda;

        // Mass Matrix
        spmatXr massMatrix;

        // Tetrahedral volumes
        vecXr tetVolumes;

        // Simulation control params.
        real dt;
        real density;

        vecXr q;
        vecXr qdot;

        SimulationState() = default;
        SimulationState(real youngsModulus, real poissonsRatio, real dt, real density)
            : dt(dt), density(density), mu(0.5 * youngsModulus / (2.0 * (1.0 + poissonsRatio))),
              lambda(0.5 * (youngsModulus * poissonsRatio) / ((1.0 + poissonsRatio) * (1.0 - 2.0 * poissonsRatio))){};

        void setSimulationConstraint(const SimulationConstraint &constraint);
        void initializeSimRuntimeMembers(const matXr &vertices);
        void initializeSimAuxiliaryMembers(const matXr &vertices, const matXi &tets);

        auto isInit() -> bool;

    private:
        SimulationConstraint simulationConstraint_;
    };

    auto simulationStateFactory(const matXr &vertices, const matXi &tets, real youngsModulus, real poissonsRatio,
                                real dt, real density) -> SimulationState;

    auto simulationConstraintFactory(const matXr &vertices, real selectionTolerance) -> SimulationConstraint;

    void simulate(SimulationState &simulationState, const matXr &vertices, const matXi &tets);
}// namespace nm