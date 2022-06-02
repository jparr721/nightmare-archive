#pragma once

#include "../geometry/tet_mesh.h"
#include "../math.h"
#include <vector>

namespace nm::integrators {
    struct Integrator {
        real dt;

        vec position;
        vec positionOld;
        vec velocity;
        vec forces;

        spmat massMatrix;
        spmat constraints;
        spmat stiffnessMatrix;

        std::unique_ptr<geometry::TetMesh> activeTetMesh;
        std::vector<geometry::TetMesh *> kinematicsCollisionObjects;

        void initialize();
        void solve();

    private:
        void buildDampingMatrix();
        void buildMassMatrix();
        void buildStiffnessMatrix();
    };
}// namespace nm::integrators
