#include "neo_hookean.h"

namespace nm::hyperelastic {
    auto psi(const mat3 &F, real lambda, real mu) -> real {
        const real J = F.determinant();
        const real logJ = std::log(J);

        // Invariant 2
        const real I2 = doubleContraction(F, F);
        return (mu / 2) * (I2 - 3) - mu * logJ + (lambda / 2) * (logJ);
    }

    auto pk1() -> mat3 { return mat3(); }

    auto dpk1() -> mat9 { return mat9(); }

    auto clampedDpk1() -> mat9 { return mat9(); }
}// namespace nm::hyperelastic
