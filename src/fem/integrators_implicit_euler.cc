#include "integrators_implicit_euler.h"
#include "linear_energy_function.h"
#include "newtons_method.h"
#include <memory>

namespace nm::fem {
    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets) -> void {
        simulationState.qdot =
                newtonsMethod(linearEnergyFunction, linearEnergyFunctionGradient, linearEnergyFunctionHessian,
                              simulationState, vertices, tets, 5, simulationState.qdot);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem