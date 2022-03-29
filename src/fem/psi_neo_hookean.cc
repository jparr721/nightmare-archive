#include "psi_neo_hookean.h"

namespace nm::fem {
    auto psiNeoHookean(const mat3r &F, real mu, real lambda) -> real {
        const real J = F.determinant();
        return mu * (std::pow(J, -2.0 / 3.0) * (F.transpose() * F).trace() - 3) + lambda * std::pow(J - 1, 2);
    }
}// namespace nm::fem
