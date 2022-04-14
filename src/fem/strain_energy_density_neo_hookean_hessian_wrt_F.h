#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * Computes the hessian of the potential energy with respect to the deformation gradient.
     *
     * @param F The deformation gradient
     * @param mu The lame's mu parameter from the provided E and v.
     * @param lambda The lame's lambda parameter from the provided E and v.
     * @return The 9x9 matrix hessian of the potential energy.
     */
    auto strainEnergyDensityNeoHookeanHessianWrtF(const mat3r &F, real mu, real lambda) -> mat99r;
}// namespace nm::fem