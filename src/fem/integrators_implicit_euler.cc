#include "integrators_implicit_euler.h"
#include "V_linear_tetrahedron.h"
#include "assemble.h"
#include <memory>

namespace nm::fem {
    auto NewtonsMethod::solve(unsigned int maxIterations, const vecXr &initialGuess) -> vecXr {
        // Copy the initial guess into x0 so we can update it.
        vecXr x0 = initialGuess;
        vecXr noOpResult = x0;

        // Begin iterating newton's method.
        for (int ii = 0; ii < maxIterations; ++ii) {
            // First, check for convergence
            const vecXr gradient = energyFunctionGradient(x0);

            // Convergence reached! Woo!
            if (gradient.squaredNorm() < std::numeric_limits<real>::epsilon()) { return noOpResult; }

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

                if (alpha < std::numeric_limits<real>::epsilon()) { return noOpResult; }
            }

            x0 += alpha * d;
        }

        return x0;
    }

    auto NewtonsMethod::energyFunction(const vecXr &guess) -> real {
        // This is the q + dt * v part of the expression.
        const vecXr newq = q_ + dt_ * guess;
        real E = 0;
        for (int ii = 0; ii < tets_.rows(); ++ii) {
            E += VlinearTetrahedron(q_, vertices_, tets_.row(ii), mu_, lambda_, tetVolumes_(ii));
        }

        // Compute the rest of the energy function value.
        E += 0.5 * (guess - qdot_).transpose() * massMatrix_ * (guess - qdot_);
        return E;
    }

    auto NewtonsMethod::energyFunctionGradient(const vecXr &guess) -> vecXr {
        const vecXr force = -assembleForces(q_, vertices_, tets_, tetVolumes_, mu_, lambda_);
        return massMatrix_ * (guess - qdot_) + dt_ * force;
    }

    auto NewtonsMethod::energyFunctionHessian(const vecXr &guess) -> spmatXr {
        const spmatXr stiffness = -assembleStiffness(q_, vertices_, tets_, tetVolumes_, mu_, lambda_);
        return massMatrix_ + dt_ * dt_ * stiffness;
    }

    auto implicitEuler(vecXr &q, vecXr &qdot, const matXr &vertices, const spmatXr &massMatrix, const matXi &tets,
                       const vecXr &tetVolumes, real mu, real lambda, real dt) -> void {
        const auto newtonsMethodSolver =
                std::make_unique<NewtonsMethod>(q, qdot, vertices, massMatrix, tets, tetVolumes, mu, lambda, dt);
        qdot = newtonsMethodSolver->solve(5, qdot);
        q += dt * qdot;
    }
}// namespace nm::fem