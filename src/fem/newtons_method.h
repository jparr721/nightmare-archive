#pragma once

#include "../nm_math.h"
#include "../simulation.h"

namespace nm::fem {
    class NewtonsMethod {
    public:
        NewtonsMethod(SimulationState simulationState, matXr vertices, matXi tets)
            : state_(std::move(simulationState)), vertices_(std::move(vertices)), tets_(std::move(tets)) {}

        auto solve(unsigned int maxIterations, const vecXr &initialGuess) -> vecXr;

    private:
        const SimulationState state_;

        const matXr vertices_;
        const matXi tets_;

        auto energyFunction(const vecXr &guess) -> real;
        auto energyFunctionGradient(const vecXr &guess) -> vecXr;
        auto energyFunctionHessian(const vecXr &guess) -> spmatXr;
    };
}// namespace nm::fem