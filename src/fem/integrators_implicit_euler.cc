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
        const vecXr newq = simulationState_.q + simulationState_.dt * guess;
        real energy = 0.0;
        for (int ii = 0; ii < tets_.rows(); ++ii) {
            energy += VlinearTetrahedron(simulationState_.q, vertices_, tets_.row(ii), simulationState_.mu,
                                         simulationState_.lambda, simulationState_.tetVolumes(ii));
        }

        // Compute the rest of the energy function value.
        energy += 0.5 * (guess - simulationState_.qdot).transpose() * simulationState_.massMatrix *
                  (guess - simulationState_.qdot);
        return energy;
    }

    auto NewtonsMethod::energyFunctionGradient(const vecXr &guess) -> vecXr {
        const vecXr force = -assembleForces(simulationState_.q, vertices_, tets_, simulationState_.tetVolumes,
                                            simulationState_.mu, simulationState_.lambda);
        return simulationState_.massMatrix * (guess - simulationState_.qdot) + simulationState_.dt * force;
    }

    auto NewtonsMethod::energyFunctionHessian(const vecXr &guess) -> spmatXr {
        const spmatXr stiffness = -assembleStiffness(simulationState_.q, vertices_, tets_, simulationState_.tetVolumes,
                                                     simulationState_.mu, simulationState_.lambda);
        return simulationState_.massMatrix + simulationState_.dt * simulationState_.dt * stiffness;
    }

    auto implicitEuler(SimulationState &simulationState, const matXr &vertices, const matXi &tets) -> void {
        const auto newtonsMethodSolver = std::make_unique<NewtonsMethod>(simulationState, vertices, tets);
        simulationState.qdot = newtonsMethodSolver->solve(5, simulationState.qdot);
        simulationState.q += simulationState.dt * simulationState.qdot;
    }
}// namespace nm::fem