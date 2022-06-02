#include "stable_neo_hookean.h"

namespace nm::hyperelastic {
    auto psi(const mat3 &F, real lambda, real mu) -> real {
        const real alpha = 1.0 + mu / lambda;
        const real Ic = F.squaredNorm();
        const real J = F.determinant();

        // As specified in HOBAK, we drop (mu/2) * (log(Ic + 1)) as it was mostly
        // included as a formality for a situation that never happens.
        // Equation here is from "Stable Neo-Hookean Flesh Simulation" eq. 14
        return 0.5 * (mu * (Ic - 3) + lambda * (J - alpha) * (J - alpha));
    }

    auto pk1(const mat3 &F, real lambda, real mu) -> mat3 {
        const real Ic = F.squaredNorm();
        const real J = F.determinant();
        const mat3 pJpF = identities::partialJpartialF(F);
        const real alpha = 1.0 + mu / lambda;

        // First Piola Kirchoff Stress Tensor
        // Equation here is from "Stable Neo-Hookean Flesh Simulation" eq. 18
        return mu * (1 - 1 / (Ic + 1)) * F + lambda * (J - alpha) * pJpF;
    }

    auto dpk1(const mat3 &F, real lambda, real mu) -> mat9 {}

    auto clampedDpk1(const mat3 &U, const vec3 &sigma, const mat3 &V) -> mat9 {}
}// namespace nm::hyperelastic
