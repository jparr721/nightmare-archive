#pragma once

#include "../nm_math.h"

namespace nm::fem {
    auto springPotentialEnergy(const vecXr &u, const vecXr &v, real l0, real stiffness) -> real;
    auto springPotentialEnergyGradient(const vecXr &u, const vecXr &v, real l0, real stiffness) -> vec6r;
}// namespace nm::fem