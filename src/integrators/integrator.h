#pragma once

#include "../geometry/tet_mesh.h"
#include "../math.h"
#include <vector>

namespace nm::integrators {
    /**
     * IntegratorState is a helper struct for bookkeeping for various time integration schemes
     */
    struct IntegratorState {
        // Runtime coefficients
        int currentTimestep = 0;
        real dt = 1.0 / 60.0;
        real time = 0.0;

        // Damping coefficients
        real rayleighAlpha = 0.01;
        real rayleighBeta = 0.01;

        // Collider stuff
        real collisionStiffness = 1.0;
        real collisionDampingBeta = 0.001;

        // Material state holders
        vec position;
        vec positionOld;
        vec velocity;
        vec forces;

        // Solver stuff
        spmat massMatrix;
        spmat constraints;
        spmat stiffnessMatrix;

        // Geometry state
        std::unique_ptr<geometry::TetMesh> activeTetMesh;
        std::vector<geometry::TetMesh *> kinematicsCollisionObjects;
    };
}// namespace nm::integrators
