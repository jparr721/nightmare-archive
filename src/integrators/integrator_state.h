#pragma once

#include "../geometry/tet_mesh.h"
#include "../math.h"
#include <utility>
#include <vector>

namespace nm::integrators {
    /**
     * IntegratorState is a helper struct for bookkeeping for various time integration schemes
     */
    struct IntegratorState {
        // Initialization status
        u8 isInit = 0;

        // Runtime coefficients
        u64 currentTimestep = 0;
        real dt = 1.0 / 60.0;
        real time = 0.0;

        // Damping coefficients
        real rayleighAlpha = 0.01;
        real rayleighBeta = 0.01;

        // Collider stuff
        real collisionStiffness = 1.0;
        real collisionDampingBeta = 0.001;
        std::vector<bool> collisionStates;

        // Material coefficients
        real mu = 0.0;
        real lambda = 0.0;

        // Material state holders
        vec position;
        vec positionOld;
        vec velocity;
        vec nextVelocity;
        vec forces;
        vec externalForces;
        vec constraintTargets;

        // Solver stuff
        spmat massMatrix;
        spmat constraints;
        spmat stiffnessMatrix;

        // Geometry state
        std::shared_ptr<geometry::TetMesh> activeTetMesh;
        std::vector<std::shared_ptr<geometry::TetMesh>> kinematicsCollisionObjects;

        IntegratorState(std::shared_ptr<geometry::TetMesh> tetMesh) : activeTetMesh(std::move(tetMesh)) {
            forces = vec::Zero(activeTetMesh->ndofs());
            externalForces = vec::Zero(activeTetMesh->ndofs());
            constraintTargets = vec::Zero(activeTetMesh->ndofs());
            collisionStates = std::vector<bool>(activeTetMesh->ndofs(), false);

            position.resize(activeTetMesh->ndofs());
            positionOld.resize(activeTetMesh->ndofs());
            velocity.resize(activeTetMesh->ndofs());
            nextVelocity.resize(activeTetMesh->ndofs());

            // Mass matrix only needs to be computed once
            massMatrix = activeTetMesh->computeMassMatrix();
        }
    };

    void computeCollisionDetection(IntegratorState *integratorState);
    void computeCollisionResponse(IntegratorState *integratorState, vec &R, spmat &K, spmat &collisionC);

}// namespace nm::integrators
