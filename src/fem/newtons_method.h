#pragma once

#include "../nm_math.h"
#include "../simulation.h"
#include <type_traits>

namespace nm::fem {
    template<typename EnergyFn, typename GradFn, typename HessianFn>
    auto newtonsMethod(const EnergyFn &energyFn, const GradFn &gradFn, const HessianFn &hessianFn,
                       const SimulationState &simulationState, const matXr &vertices, const matXi &tets,
                       unsigned int maxIterations, const vecXr &initialGuess, const vecXr &externalForces) -> vecXr {
        static_assert(std::is_function_v<EnergyFn>, "Energy function must be a function pointer");
        static_assert(std::is_function_v<GradFn>, "Gradient function must be a function pointer");
        static_assert(std::is_function_v<HessianFn>, "Hessian function must be a function pointer");

        // Copy the initial guess into x0 so we can update it.
        vecXr x0 = initialGuess;
        vecXr noOpResult = x0;

        // Begin iterating newton's method.
        for (int ii = 0; ii < maxIterations; ++ii) {
            // First, check for convergence
            const vecXr gradient = gradFn(simulationState, vertices, tets, x0, externalForces);

            // Convergence reached! Woo!
            if (gradient.squaredNorm() < 1e-8) { return noOpResult; }

            // Compute the search direction by solving for d in Hd = -g where H is the hessian and g is the gradient.
            const spmatXr hessian = hessianFn(simulationState, vertices, tets, x0);

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
                if (energyFn(simulationState, vertices, tets, x0 + alpha * d) <=
                    energyFn(simulationState, vertices, tets, x0) + c * d.transpose() * gradient) {
                    break;
                }

                alpha *= p;

                if (alpha < 1e-8) { return noOpResult; }
            }

            x0 += alpha * d;
        }

        return x0;
    }
}// namespace nm::fem