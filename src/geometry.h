#pragma once

#include "math.h"
#include <cmath>

namespace nm {
    vec3r sphericalToCartesian(real r, real theta, real phi);
    vec3r sphericalToCartesianDPhi(real r, real theta, real phi);

    mat4r perspectiveProjection(real near, real far, real fov, real aspect);
    mat4r lookAt(const vec3r &eye, const vec3r &at, const vec3r &up);
}// namespace nm
