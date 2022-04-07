#include "integrators_implicit_euler.h"
#include "linear_energy_function.h"
#include "newtons_method.h"

namespace nm::fem {
    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                       std::optional<unsigned int> selectedVertex) -> void {
        simulationState.qdot = newtonsMethod(computeLinearTetrahedralEnergy, computeLinearTetrahedralForce,
                                             computeLinearTetrahedralStiffness, simulationState, vertices, tets, 5,
                                             simulationState.qdot, selectedVertex);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem