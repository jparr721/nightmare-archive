/**
 * Implements the SNH algorithms from "Stable Neo-Hookean Flesh Simulation"
 */

#pragma once

#include "../math.h"

namespace nm::hyperelastic {
    /**
     * Computes the strain energy density of the stable neo hookean energy function.
     */
    auto psi(const mat3 &F, real lambda, real mu) -> real;

    /**
     * Computes the first Piola-Kirchoff stress
     */
    auto pk1(const mat3 &F, real lambda, real mu) -> mat3;

    /**
     * Description is stolen from "Dynamic Deformables" HOBAK Codebase:
     *
     * These are from Section 4.6 in "Stable Neo-Hookean Flesh Simulation"
     *
     * Not exactly though! We drop the \frac{\mu}{2} \log(I_C +1) term,
     * because that was just put in to make a reviewer happy. It's not
     * actually needed to keep the energy stable, but they wouldn't believe
     * us, so we had to add it in order to guard against a case that never
     * actually happens in practice. (The mesh collapsing to a perfectly
     * zero-volume point.)
     *
     * We prefer to use the simpler model that omits the term, and it is what
     * is used in production at Pixar.
     **/
    auto dpk1(const mat3 &U, const vec3 &sigma, const mat3 &V, real lambda, real mu) -> mat9;
}// namespace nm::hyperelastic
