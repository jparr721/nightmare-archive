#pragma once

#include "math.h"
#include <cmath>

namespace nm {
    vec3r spherical_to_catesian(real r, real theta, real phi);
    vec3r spherical_to_catesian_dphi(real r, real theta, real phi);

    mat4r perspective_projection(real near, real far, real fov, real aspect);
    mat4r look_at(const vec3r &eye, const vec3r &at, const vec3r &up);
}// namespace nm
