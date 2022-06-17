#pragma once

#include "../../math.h"
#include "../kinematic_object.h"
#include "../tet_mesh.h"
#include <array>

namespace nm::geometry::primitives {
    void cube_loadGeometry(mat &V, mati &F);

    auto cube_inside(const vec3 &point, const KinematicObject &cube) -> bool;
    auto cube_distance(const vec3 &point, const KinematicObject &cube) -> real;
    auto cube_signedDistance(const vec3 &point, const KinematicObject &cube) -> real;
    void cube_getClosestPoint(const vec3 &queryPoint, const KinematicObject &cube, vec3 &closestPointLocal,
                              vec3 &normalLocal);
}// namespace nm::geometry::primitives