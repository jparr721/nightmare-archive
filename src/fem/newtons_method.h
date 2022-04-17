#pragma once

#include "../nm_math.h"
#include "../simulation.h"
#include <spdlog/spdlog.h>
#include <type_traits>

namespace nm::fem {
    template<typename Energy, typename Force, typename Stiffness>
    void newtonsMethod(const Energy &energy, const Force &force, const Stiffness &stiffness, unsigned int maxIterations,
                       vecXr &x0) {
        constexpr real kMinimumValueThreshold = std::numeric_limits<real>::epsilon();

        // Begin iterating newton's method.
        for (int ii = 0; ii < maxIterations; ++ii) {
            // First, check for convergence
            const vecXr gradient = force(x0);

            // Convergence reached! Woo!
            if (gradient.squaredNorm() < kMinimumValueThreshold) { return; }

            // Compute the search direction by solving for d in Hd = -g where H is the hessian and g is the gradient.
            const spmatr hessian = stiffness(x0);

            Eigen::SimplicialLDLT<spmatr, Eigen::Upper> solver(hessian);

            if (solver.info() == Eigen::NumericalIssue) {
                spdlog::error("Matrix could not be facotorized, running diagnostic and exiting");
                checkSpdMatrix(hessian);
                std::exit(1);
            }

            if (solver.info() != Eigen::Success) {
                spdlog::error("Decomposition failed");
                return;
            }

            // Compute d to get the direction vector.
            vecXr d = -solver.solve(gradient);
            if (solver.info() != Eigen::Success) {
                spdlog::error("Solving the linear system has failed.");
                return;
            }

            // Line search for optimum alpha value.
            real alpha = 1.0;
            real c = 1e-8;
            real p = 0.5;

            for (;;) {
                if (energy(x0 + alpha * d) <= energy(x0) + c * d.transpose() * gradient) { break; }

                alpha *= p;

                if (alpha < kMinimumValueThreshold) { return; }
            }

            x0 += alpha * d;
        }
    }
}// namespace nm::fem