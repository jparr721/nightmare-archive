#pragma once

#include "../nm_math.h"

namespace nm::fem {
    /**
     * Compute the energy function for newtons method. We define this in terms of our generalized coordinates which
     * is computed as:\n
     * E(v) = 1/2 * (v - qdot)^T * M(v - qdot) + V(q + dt * v)\n
     * Where v is our initial guess.
     * @param q The deformed vector of points.
     * @param qdot The deformed vector of velocities.
     * @param guess The initial guess for the qdot update
     * @param vertices The vertices of the mesh
     * @param mass_matrix The sparse mass matrix
     * @param tets The tets of the mesh
     * @param tet_volumes The volumes of the tets of the mesh
     * @param mu Lame's mu parameter
     * @param lambda Lame's lambda parameter
     * @param dt The dt of the simulation.
     * @return The value of the energy function at the guess.
     */
    auto energyFunction(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                        const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu, real lambda,
                        real dt) -> real;

    auto energyFunctionGradient(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                                const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu,
                                real lambda, real dt) -> vecXr;
    auto energyFunctionHessian(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                               const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu,
                               real lambda, real dt) -> spmatXr;

    auto newtonsMethod(const vecXr &v0) -> real;

    auto implicitEuler() -> void;
}// namespace nm::fem