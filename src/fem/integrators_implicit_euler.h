#pragma once

#include <utility>

#include "../nm_math.h"
#include "../simulation.h"

namespace nm::fem {
    class NewtonsMethod {
    public:
        NewtonsMethod(SimulationState simulationState, matXr vertices, matXi tets)
            : simulationState_(std::move(simulationState)), vertices_(std::move(vertices)), tets_(std::move(tets)) {}

        auto solve(unsigned int maxIterations, const vecXr &initialGuess) -> vecXr;

    private:
        const SimulationState simulationState_;

        const matXr vertices_;
        const matXi tets_;

        auto energyFunction(const vecXr &guess) -> real;
        auto energyFunctionGradient(const vecXr &guess) -> vecXr;
        auto energyFunctionHessian(const vecXr &guess) -> spmatXr;
    };

    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets) -> void;
}// namespace nm::fem