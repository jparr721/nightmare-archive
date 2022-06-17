#pragma once

#include "../math.h"

namespace nm::geometry {
    class TetMesh;

    /**
     * Constrains a vertex to move along with the position of a kinematic shape
     * Stolen from Ted's HOBAK code. Thanks Ted!
     */
    struct KinematicConstraint {
        const TetMesh *tetMesh;
        int vertexId;
        vec3 localPosition;
    };

    /**
     * Constrains a vertex to move alone a plane.
     * Stolen from Ted's HOBAK code. Thanks Ted!
     */
    struct PlaneConstraint {
        const TetMesh *tetMesh;
        int vertexId;
        vec3 localClosestPoint;
        vec3 localNormal;

        // Determines whether or not the object was separating from the plane @ the last timestep.
        // If so, we should delete this constraint.
        bool isSeparating;
    };
}// namespace nm::geometry