#pragma once

#include "nm_math.h"
namespace nm {
    struct SimulationState {
        real youngsModulus;
        real poissonsRatio;

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
        SimulationState(real youngsModulus, real poissonsRatio, real dt, real density);

        void initializeSimRuntimeMembers(const matXr &vertices);
        void initializeSimAuxiliaryMembers(const matXr &vertices, const matXi &tets);
    };

    auto simulationStateFactory(const matXr &vertices, const matXi &tets, real youngsModulus, real poissonsRatio,
                                real dt, real density) -> SimulationState;

    void simulate(SimulationState &simulationState, const matXr &vertices, const matXi &tets);
}// namespace nm