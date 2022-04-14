#include "strain_energy_density_neo_hookean.h"

namespace nm::fem {
    auto strainEnergyDensityNeoHookean(const mat3r &F, real mu, real lambda) -> real {
        const real J = F.determinant();
        return mu * (std::pow(J, -2.0 / 3.0) * (F.transpose() * F).trace() - 3) + lambda * std::pow(J - 1, 2);
    }
}// namespace nm::fem
