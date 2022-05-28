#include "neo_hookean.h"

namespace nm::hyperelastic {
    auto psi(const mat3 &F, real lambda, real mu) -> real {
        const real J = F.determinant();
        const real logJ = std::log(J);

        // Invariant 2
        const real I2 = doubleContraction(F, F);
        return mu * 0.5 * (I2 - 3) - mu * logJ + lambda * 0.5 * logJ * logJ;
    }

    auto pk1(const mat3 &F, real lambda, real mu) -> mat3 {
        const real J = F.determinant();
        const real logJ = std::log(J);
        const mat3 gradJ = identities::gradientJ(F);
        return mu * (F - (1 / J) * gradJ) + lambda * (logJ / J) * gradJ;
    }

    auto dpk1() -> mat9 { return mat9(); }

    auto clampedDpk1() -> mat9 { return mat9(); }
}// namespace nm::hyperelastic
