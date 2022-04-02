#pragma once

#include <utility>

#include "../nm_math.h"

namespace nm::fem {
    class NewtonsMethod {
    public:
        NewtonsMethod(vecXr q, vecXr qdot, matXr vertices, const spmatXr &mass_matrix, matXi tets, vecXr tet_volumes,
                      const real mu, const real lambda, const real dt)
            : q_(std::move(q)), qdot_(std::move(qdot)), vertices_(std::move(vertices)), mass_matrix_(mass_matrix),
              tets_(std::move(tets)), tet_volumes_(std::move(tet_volumes)), mu_(mu), lambda_(lambda), dt_(dt) {}

        auto solve(unsigned int max_iterations, const vecXr &initial_guess) -> real;

    private:
        const vecXr q_;
        const vecXr qdot_;
        const matXr vertices_;
        const spmatXr mass_matrix_;
        const matXi tets_;
        const vecXr tet_volumes_;
        const real mu_;
        const real lambda_;
        const real dt_;

        auto energyFunction(const vecXr &guess) -> real;
        auto energyFunctionGradient(const vecXr &guess) -> vecXr;
        auto energyFunctionHessian(const vecXr &guess) -> spmatXr;
    };

    auto implicitEuler() -> void;
}// namespace nm::fem