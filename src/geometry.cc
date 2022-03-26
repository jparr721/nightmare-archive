#include "geometry.h"


namespace nm {
    vec3r sphericalToCartesian(real r, real theta, real phi) {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return {r * (cos_theta * sin_phi), r * cos_phi, r * (sin_theta * sin_phi)};
    }

    vec3r sphericalToCartesianDPhi(real r, real theta, real phi) {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return {r * (cos_phi * cos_theta), -r * sin_phi, r * (cos_phi * sin_theta)};
    }

    mat4r perspectiveProjection(real near, real far, real fov, real aspect_ratio) {
        const real ymax = near * std::tan(fov * M_PI / static_cast<real>(360.0));
        const real xmax = ymax * aspect_ratio;

        const real left = -xmax;
        const real right = xmax;
        const real bottom = -ymax;
        const real top = ymax;

        const real t1 = static_cast<real>(2.0) * near;
        const real t2 = right - left;
        const real t3 = top - bottom;
        const real t4 = far - near;

        mat4r ret = mat4r::Zero();

        ret(0) = t1 / t2;
        ret(5) = t1/t3;
        ret(8) = (right + left) / t2;
        ret(9) = (top + bottom) / t3;
        ret(10) = (-far - near) / t4;
        ret(11) = static_cast<real>(-1.0);
        ret(14) = (-t1 * far) / t4;

        return ret;
    }

    mat4r lookAt(const vec3r &eye, const vec3r &at, const vec3r &up) {
        const vec3r forward = (at - eye).normalized();
        const vec3r side = forward.cross(up).normalized();
        const vec3r up_ = side.cross(forward).normalized();

        mat4r ret = mat4r::Zero();

        ret.row(0) << side(0), up_(0), -forward(0), 0.0;
        ret.row(1) << side(1), up_(1), -forward(1), 0.0;
        ret.row(2) << side(2), up_(2), -forward(2), 0.0;
        ret.row(3) << -side.dot(eye), -up.dot(eye), forward.dot(eye), 1.0;

        return ret.transpose();
    }
}// namespace nm