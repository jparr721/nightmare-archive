#include "integrators_implicit_euler.h"
#include "V_linear_tetrahedron.h"
#include "assemble.h"

namespace nm::fem {
    auto energyFunction(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                        const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu, real lambda,
                        real dt) -> real {
        // This is the q + dt * v part of the expression.
        const vecXr new_q = q + dt * guess;
        real E = 0;
        for (int ii = 0; ii < tets.rows(); ++ii) {
            E += VlinearTetrahedron(q, vertices, tets.row(ii), mu, lambda, tet_volumes(ii));
        }

        // Compute the rest of the energy function value.
        E += 0.5 * (guess - qdot).transpose() * mass_matrix * (guess - qdot);

        return E;
    }

    auto energyFunctionGradient(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                                const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu,
                                real lambda, real dt) -> vecXr {
        const vecXr force = -assembleForces(q, vertices, tets, tet_volumes, mu, lambda);
        return mass_matrix * (guess - qdot) + dt * force;
    }

    auto energyFunctionHessian(const vecXr &q, const vecXr &qdot, const vecXr &guess, const matXr &vertices,
                               const spmatXr &mass_matrix, const matXi &tets, const vecXr &tet_volumes, real mu,
                               real lambda, real dt) -> spmatXr {
        const spmatXr stiffness = -assembleStiffness(q, vertices, tets, tet_volumes, mu, lambda);
        return mass_matrix + dt * dt * stiffness;
    }

}// namespace nm::fem