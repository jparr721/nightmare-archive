#pragma once

#include <utility>

#include "../nm_math.h"
#include "../simulation.h"
#include "newtons_method.h"

namespace nm::fem {
    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                       std::optional<unsigned int> selectedVertex) -> void;
}// namespace nm::fem