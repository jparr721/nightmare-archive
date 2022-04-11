#pragma once

#include <utility>

#include "../nm_math.h"
#include "../simulation.h"
#include "newtons_method.h"

namespace nm::fem {
    template<typename Energy, typename Force, typename Stiffness>
    auto implicitEuler(const Energy &energy, const Force &force, const Stiffness &stiffness, const matXr &vertices,
                       const matXi &tets, const spmatXr &massMatrix, real dt, vecXr &q, vecXr &qdot) -> void {
        const auto grad = [&](const vecXr &x) -> vecXr {
            const auto f = force(q + dt * x);
            return massMatrix * (x - qdot) - dt * f;
        };

        const auto hessian = [&](const vecXr &x) -> spmatXr {
            const auto k = stiffness(q + dt * x);
            return massMatrix - dt * dt * k;
        };

        qdot = newtonsMethod(energy, grad, hessian, 5, qdot);
        q += dt * qdot;
    }
}// namespace nm::fem