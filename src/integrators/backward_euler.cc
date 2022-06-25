#include "backward_euler.h"

namespace nm::integrators {
    void backwardEuler_solve(IntegratorState *integratorState) {
        // stiffness matrix
        const spmat K =
                integratorState->activeTetMesh->computeStiffnessMatrix(integratorState->lambda, integratorState->mu);

        // Damping matrix
        const spmat C = integratorState->activeTetMesh->computeDampingMatrix(
                integratorState->massMatrix, K, integratorState->lambda, integratorState->mu);

        // Cache the old position
        integratorState->positionOld = integratorState->position;

        // Compute the new deformation gradients
        integratorState->activeTetMesh->computeDeformationGradients();

        // Compute the forces
        const vec R = integratorState->activeTetMesh->computeForces(integratorState->lambda, integratorState->mu);

        // Assemble linear system
        const real dt = integratorState->dt;
        const vec b = dt * (R + dt * K * integratorState->velocity + integratorState->externalForces);

        // Assemble the matrix system A for the LHS of the implicit integration
        const spmat A = integratorState->massMatrix - dt * C - dt * dt * K;

        // This will make more sense once we've added contraints
        // const vec c = b - A /* * z */;

        // Congugate gradient solver
        Eigen::ConjugateGradient<spmat, Eigen::Lower | Eigen::Upper> cgSolver(A);

        // Get the new velocity value
        const vec dv = cgSolver.solve(b);

        // Set velocity and position values
        integratorState->velocity += dv;
        integratorState->position += dt * integratorState->velocity;

        integratorState->time += dt;
        ++integratorState->currentTimestep;
    }
}// namespace nm::integrators
