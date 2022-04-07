#include "spring_potential.h"

namespace nm::fem {
    auto springPotentialEnergy(const nm::vecXr &u, const nm::vecXr &v, nm::real l0, nm::real stiffness) -> nm::real {
        return 0.5 * stiffness * std::pow(((u - v).norm() - l0), 2);
    }

    auto springPotentialEnergyGradient(const nm::vecXr &u, const nm::vecXr &v, nm::real l0, nm::real stiffness)
            -> nm::vec6r {
        const auto k = stiffness * (u - v).norm();
        const auto direction = (u - v).normalized();

        vec6r ret;
        ret.segment<3>(0) = k * direction;
        ret.segment<3>(3) = -k * direction;
        return ret;
    }
}// namespace nm::fem