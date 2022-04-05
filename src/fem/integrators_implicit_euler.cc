#include "integrators_implicit_euler.h"
#include <memory>

namespace nm::fem {
    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets) -> void {
        const auto newtonsMethodSolver = std::make_unique<NewtonsMethod>(simulationState, vertices, tets);
        simulationState.qdot = newtonsMethodSolver->solve(5, simulationState.qdot);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem