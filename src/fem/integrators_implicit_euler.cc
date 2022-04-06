#include "integrators_implicit_euler.h"
#include "linear_energy_function.h"
#include "newtons_method.h"

namespace nm::fem {
    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                       const vecXr &externalForces) -> void {
        simulationState.qdot =
                newtonsMethod(linearEnergyFunction, linearEnergyFunctionGradient, linearEnergyFunctionHessian,
                              simulationState, vertices, tets, 5, simulationState.qdot, externalForces);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem