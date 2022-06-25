#pragma once

#include "../math.h"
#include "tet_mesh.h"

namespace nm::geometry {
    struct KinematicObject {
        mat3 scale;
        mat3 scaleInverse;
        mat3 rotation;
        vec3 translation;
        TetMesh *tetMesh;
    };

    auto localVertexToWorld(const vec3 &local, const KinematicObject &kinematicObject) -> vec3;
    auto localNormalToWorld(const vec3 &normal, const KinematicObject &kinematicObject) -> vec3;
    auto worldVertexToLocal(const vec3 &world, const KinematicObject &kinematicObject) -> vec3;
}// namespace nm::geometry