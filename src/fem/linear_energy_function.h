#pragma once

#include "../simulation.h"
#include "V_linear_tetrahedron.h"
#include "assemble.h"

namespace nm::fem {
    auto linearEnergyFunction(const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                              const vecXr &guess) -> real;

    auto linearEnergyFunctionGradient(const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                                      const vecXr &guess) -> vecXr;

    auto linearEnergyFunctionHessian(const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                                     const vecXr &guess) -> spmatXr;
}// namespace nm::fem