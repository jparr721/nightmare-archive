#pragma once

#include "../math.h"

namespace nm::hyperelastic {
    /**
     * Computes the strain energy density of the neo hookean energy function.
     */
    auto psi(const mat3 &F, real lambda, real mu) -> real;

    /**
     * Computes the first Piola-Kirchoff stress
     */
    auto pk1(const mat3 &F, real lambda, real mu) -> mat3;

    /**
     * Computes the derivative of the pk1
     */
    auto dpk1() -> mat9;

    /**
     * Computes the derivative of pk1, but clamps it to semi-positive definite
     */
    auto clampedDpk1() -> mat9;
}// namespace nm::hyperelastic
