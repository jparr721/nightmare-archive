#pragma once

#include "nm_math.h"
#include <igl/Hit.h>
#include <optional>

namespace nm {
    struct SelectedVertex {
        unsigned int vertexIndex;
        vec3r vertexPosition;
    };

    auto shootRay(const matXr &vertices, const matXi &faces, const vec3r &start, const vec3r &dir)
            -> std::optional<igl::Hit>;

    auto pickNearestVertex(const vec3r &win, const mat4r &view, const mat4r &proj, const vec4r &viewport,
                           const matXr &vertices, const matXi &faces, double radius) -> int;
}// namespace nm