#pragma once

#include "../nm_math.h"
namespace nm::fem {
    /**
     * Computes the gradient of the strain energy density with respect to F. This is a nightmare to do by hand, so the
     * code was generated by matlab, which I stole from https://github.com/dilevin/Bartels/blob/master/src/dpsi_neohookean_dF.cpp
     *\n
     * This function computes the first Piola-Kirchoff stress vector, which we use to measure force on a given
     * tetrahedral element. By taking the derivative of the strain energy density function with respect to the
     * deformation gradient, we're able to obtain the gradient of this result, giving us a numerical representation
     * of the amount of force being exerted on the system by quantifying how rapidly (or violently) the system is
     * deforming.
     *
     * @param F The 3x3 deformation gradient
     * @param mu The lame's mu coefficient.
     * @param lambda The lame's lambda coefficient
     * @return The first Piola-Kirchoff stress value, P.
     */
    auto dpsiNeoHookeanDf(const mat3r &F, real mu, real lambda) -> vec9r;
}// namespace nm::fem