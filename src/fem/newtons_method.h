#pragma once

#include "../nm_math.h"
#include "../simulation.h"
#include <spdlog/spdlog.h>
#include <type_traits>

namespace nm::fem {
    template<typename Energy, typename Force, typename Stiffness>
    void newtonsMethod(const Energy &energy, const Force &force, const Stiffness &stiffness, unsigned int maxIterations,
                       vecXr &x0) {
        // Begin iterating newton's method.
        for (int ii = 0; ii < maxIterations; ++ii) {
            // First, check for convergence
            const vecXr gradient = force(x0);

            // Convergence reached! Woo!
            if (gradient.squaredNorm() < 1e-8) { return; }

            // Compute the search direction by solving for d in Hd = -g where H is the hessian and g is the gradient.
            const spmatXr hessian = stiffness(x0);

            Eigen::SimplicialLDLT<spmatXr, Eigen::Upper> solver;
            solver.analyzePattern(hessian);
            solver.factorize(hessian);

            // Compute d to get our direction vector.
            vecXr d = -solver.solve(gradient);

            // Line search for optimum alpha value.
            real alpha = 1.0;
            real c = 1e-8;
            real p = 0.5;

            for (;;) {
                if (energy(x0 + alpha * d) <= energy(x0) + c * d.transpose() * gradient) { break; }

                alpha *= p;

                if (alpha < 1e-8) { return; }
            }

            x0 += alpha * d;
        }
    }
}// namespace nm::fem