#pragma once

#include <utility>

#include "../nm_math.h"
#include "../simulation.h"
#include "newtons_method.h"

namespace nm::fem {
    template<typename Energy, typename Force, typename Stiffness>
    auto implicitEuler(const Energy &energy, const Force &force, const Stiffness &stiffness, const matXr &vertices,
                       const matXi &tets, std::optional<unsigned int> selectedVertex, SimulationState &simulationState)
            -> void {
        simulationState.qdot = newtonsMethod(energy, force, stiffness, simulationState, vertices, tets, 5,
                                             simulationState.qdot, selectedVertex);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem