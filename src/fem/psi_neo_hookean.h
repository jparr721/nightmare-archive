#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * Computes the neo-hookean strain energy density function for elastic materials. It's pretty easy, we just gotta
     * do:\n
     * psi(F) = mu * (J ^-2/3 * trace(F^T*F) - 3) + D(J - 1)^2
     * @param F The deformation gradient of the tetrahedral.
     * @param mu The lame's mu parameter from the provided E and v.
     * @param lambda The lame's lambda parameter from the provided E and v.
     * @return The neohookean energy density score.
     */
    auto psiNeoHookean(const mat3r &F, real mu, real lambda) -> real;
}