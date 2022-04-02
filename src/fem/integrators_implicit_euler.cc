#include "integrators_implicit_euler.h"
#include "V_linear_tetrahedron.h"
#include "assemble.h"

namespace nm::fem {
    auto NewtonsMethod::solve(unsigned int max_iterations, const vecXr &initial_guess) -> real {
        // Copy the initial guess into x0 so we can update it.
        vecXr x0 = initial_guess;

        // Begin iterating newton's method.
        for (int ii = 0; ii < max_iterations; ++ii) {
            // First, check for convergence
            const vecXr gradient = energyFunctionGradient(x0);

            // Convergence reached! Woo!
            if (gradient.squaredNorm() < std::numeric_limits<real>::epsilon()) { return 0.0; }

            // Compute the search direction by solving for d in Hd = -g where H is the hessian and g is the gradient.
            const spmatXr hessian = energyFunctionHessian(x0);

            Eigen::SimplicialLDLT<spmatXr> solver;
            solver.analyzePattern(hessian);
            solver.factorize(hessian);

            // Compute d to get our direction vector.
            vecXr d = -solver.solve(gradient);

            // Line search for optimum alpha value.
            real alpha = 0.0;
            real c = 1e-8;
            real p = 0.5;

            for (;;) {
                if (energyFunction(x0 + alpha * d) <= energyFunction(x0) + c * d.transpose() * gradient) { break; }

                alpha *= p;

                if (alpha < std::numeric_limits<real>::epsilon()) { return 0.0; }
            }

            x0 += alpha * d;
        }

        return 0.0;
    }

    auto NewtonsMethod::energyFunction(const vecXr &guess) -> real {
        // This is the q + dt * v part of the expression.
        const vecXr new_q = q_ + dt_ * guess;
        real E = 0;
        for (int ii = 0; ii < tets_.rows(); ++ii) {
            E += VlinearTetrahedron(q_, vertices_, tets_.row(ii), mu_, lambda_, tet_volumes_(ii));
        }

        // Compute the rest of the energy function value.
        E += 0.5 * (guess - qdot_).transpose() * mass_matrix_ * (guess - qdot_);
        return E;
    }

    auto NewtonsMethod::energyFunctionGradient(const vecXr &guess) -> vecXr {
        const vecXr force = -assembleForces(q_, vertices_, tets_, tet_volumes_, mu_, lambda_);
        return mass_matrix_ * (guess - qdot_) + dt_ * force;
    }

    auto NewtonsMethod::energyFunctionHessian(const vecXr &guess) -> spmatXr {
        const spmatXr stiffness = -assembleStiffness(q_, vertices_, tets_, tet_volumes_, mu_, lambda_);
        return mass_matrix_ + dt_ * dt_ * stiffness;
    }
}// namespace nm::fem