#pragma once

#include "../geometry/tet_mesh.h"
#include "../math.h"
#include <vector>

namespace nm::integrators {
    class Integrator {
    public:
        real dt = 1.0 / 60.0;

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
        int currentTimestep_;

        real time_;

        real rayleighAlpha_ = 0.01;
        real rayleighBeta_ = 0.01;

        vec vDelta_;
    };
}// namespace nm::integrators
