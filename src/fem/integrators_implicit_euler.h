#pragma once

#include <utility>

#include "../nm_math.h"

namespace nm::fem {
    class NewtonsMethod {
    public:
        NewtonsMethod(vecXr q, vecXr qdot, matXr vertices, const spmatXr &massMatrix, matXi tets, vecXr tetVolumes,
                      const real mu, const real lambda, const real dt)
            : q_(std::move(q)), qdot_(std::move(qdot)), vertices_(std::move(vertices)), massMatrix_(massMatrix),
              tets_(std::move(tets)), tetVolumes_(std::move(tetVolumes)), mu_(mu), lambda_(lambda), dt_(dt) {}

        auto solve(unsigned int maxIterations, const vecXr &initialGuess) -> vecXr;

    private:
        const vecXr q_;
        const vecXr qdot_;
        const matXr vertices_;
        const spmatXr massMatrix_;
        const matXi tets_;
        const vecXr tetVolumes_;
        const real mu_;
        const real lambda_;
        const real dt_;

        auto energyFunction(const vecXr &guess) -> real;
        auto energyFunctionGradient(const vecXr &guess) -> vecXr;
        auto energyFunctionHessian(const vecXr &guess) -> spmatXr;
    };

    auto implicitEuler(vecXr &q, vecXr &qdot, const matXr &vertices, const spmatXr &massMatrix, const matXi &tets,
                       const vecXr &tetVolumes, real mu, real lambda, real dt) -> void;
}// namespace nm::fem